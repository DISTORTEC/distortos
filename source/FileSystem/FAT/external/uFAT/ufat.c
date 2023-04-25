/* uFAT -- small flexible VFAT implementation
 * Copyright (C) 2012 TracMap Holdings Ltd
 *
 * Author: Daniel Beer <dlbeer@gmail.com>, www.dlbeer.co.nz
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include "ufat.h"
#include "ufat_internal.h"

static int cache_flush(struct ufat *uf, unsigned int cache_index)
{
	struct ufat_cache_desc *d = &uf->cache_desc[cache_index];

	if (!(d->flags & UFAT_CACHE_FLAG_DIRTY) ||
	    !(d->flags & UFAT_CACHE_FLAG_PRESENT))
		return 0;

	if (uf->dev->write(uf->dev, d->index, 1,
			   ufat_cache_data(uf, cache_index)) < 0)
		return -UFAT_ERR_IO;

	uf->stat.cache_flush++;
	uf->stat.write++;
	uf->stat.write_blocks++;

	/* If this block is part of the FAT, mirror it to the other FATs. Not
	 * a fatal error if this fails.
	 */
	if (d->index >= uf->bpb.fat_start &&
	    d->index < uf->bpb.fat_start + uf->bpb.fat_size) {
		unsigned int i;
		ufat_block_t b = d->index;

		for (i = 1; i < uf->bpb.fat_count; i++) {
			b += uf->bpb.fat_size;
			uf->dev->write(uf->dev, b, 1,
				       ufat_cache_data(uf, cache_index));

			uf->stat.write++;
			uf->stat.write_blocks++;
		}
	}

	d->flags &= ~UFAT_CACHE_FLAG_DIRTY;
	return 0;
}

int ufat_cache_evict(struct ufat *uf, ufat_block_t start, ufat_block_t count)
{
	unsigned int i;

	for (i = 0; i < uf->cache_size; i++) {
		struct ufat_cache_desc *d = &uf->cache_desc[i];

		if ((d->flags & UFAT_CACHE_FLAG_PRESENT) &&
		    d->index >= start && d->index < start + count) {
			int err = cache_flush(uf, i);

			if (err < 0)
				return err;

			d->flags = 0;
		}
	}

	return 0;
}

void ufat_cache_invalidate(struct ufat *uf, ufat_block_t start,
			   ufat_block_t count)
{
	unsigned int i;

	for (i = 0; i < uf->cache_size; i++) {
		struct ufat_cache_desc *d = &uf->cache_desc[i];

		if ((d->flags & UFAT_CACHE_FLAG_PRESENT) &&
		    d->index >= start && d->index < start + count)
			d->flags = 0;
	}
}

int ufat_cache_open(struct ufat *uf, ufat_block_t blk_index, int skip_read)
{
	unsigned int i;
	int oldest = -1;
	int free = -1;
	int err;
	unsigned int oldest_age = 0;

	/* Scan the cache, looking for:
	 *
	 *   (a) the item, if we already have it.
	 *   (b) a free slot, if one exists.
	 *   (c) the oldest cache item.
	 */
	for (i = 0; i < uf->cache_size; i++) {
		struct ufat_cache_desc *d = &uf->cache_desc[i];
		unsigned int age = uf->next_seq - d->seq;

		if ((d->flags & UFAT_CACHE_FLAG_PRESENT) &&
		    d->index == blk_index) {
			d->seq = uf->next_seq++;
			uf->stat.cache_hit++;
			return i;
		}

		if (!(d->flags & UFAT_CACHE_FLAG_PRESENT))
			free = i;

		if (oldest < 0 || age > oldest_age) {
			oldest_age = age;
			oldest = i;
		}
	}

	/* We don't have the item. Find a place to put it. */
	if (free >= 0) {
		i = free;
	} else {
		err = cache_flush(uf, oldest);
		if (err < 0)
			return err;

		i = oldest;
	}

	if (skip_read == 0) {
		/* Read it in */
		err = uf->dev->read(uf->dev, blk_index, 1,
				    ufat_cache_data(uf, i));
		if (err < 0) {
			uf->cache_desc[i].flags = 0;
			return err;
		}

		uf->stat.read++;
		uf->stat.read_blocks++;
	} else
		memset(ufat_cache_data(uf, i), 0,
		       1 << uf->dev->log2_block_size);

	struct ufat_cache_desc *d = &uf->cache_desc[i];
	d->flags = UFAT_CACHE_FLAG_PRESENT;
	d->index = blk_index;
	d->seq = uf->next_seq++;

	uf->stat.cache_miss++;

	return i;
}

static int log2_exact(unsigned int e, unsigned int *ret)
{
	unsigned int count = 0;

	if (!e)
		return -1;

	while (e > 1) {
		if (e & 1)
			return -1;

		e >>= 1;
		count++;
	}

	*ret = count;
	return 0;
}

static int parse_bpb(unsigned int log2_bytes_per_block,
		     struct ufat_bpb *ufb, uint8_t *bpb)
{
	const uint16_t bytes_per_sector = r16(bpb + 0x00b);
	const uint8_t sectors_per_cluster = bpb[0x00d];
	const uint16_t reserved_sector_count = r16(bpb + 0x00e);
	const uint16_t root_entries = r16(bpb + 0x011);
	uint32_t sectors_per_fat = r16(bpb + 0x016);
	uint32_t total_logical_sectors = r16(bpb + 0x013);
	const uint8_t number_of_fats = bpb[0x010];
	const uint32_t root_cluster = r32(bpb + 0x02c);
	unsigned int log2_bytes_per_sector = 0;
	unsigned int log2_sectors_per_cluster = 0;
	const unsigned int root_sectors =
		(root_entries * UFAT_DIRENT_SIZE + bytes_per_sector - 1) /
		bytes_per_sector;

	/* Read and check BPB values */
	if (log2_bytes_per_block < 9)
		return -UFAT_ERR_BLOCK_SIZE;

	if (!total_logical_sectors)
		total_logical_sectors = r32(bpb + 0x020);
	if (!sectors_per_fat)
		sectors_per_fat = r32(bpb + 0x024);

	if (log2_exact(bytes_per_sector, &log2_bytes_per_sector) < 0 ||
	    log2_exact(sectors_per_cluster, &log2_sectors_per_cluster) < 0)
		return -UFAT_ERR_INVALID_BPB;

	if (r16(bpb + 0x1fe) != 0xaa55)
		return -UFAT_ERR_INVALID_BPB;

	/* Convert sectors to blocks */
	if (log2_bytes_per_block > log2_bytes_per_sector) {
		const unsigned int shift =
			log2_bytes_per_block - log2_bytes_per_sector;

		if (log2_sectors_per_cluster < shift)
			return -UFAT_ERR_BLOCK_ALIGNMENT;
		ufb->log2_blocks_per_cluster =
			log2_sectors_per_cluster - shift;

		if ((reserved_sector_count | sectors_per_fat | root_sectors) &
		    ((1 << shift) - 1))
			return -UFAT_ERR_BLOCK_ALIGNMENT;
		ufb->fat_start = reserved_sector_count >> shift;
		ufb->fat_size = sectors_per_fat >> shift;
		ufb->root_size = root_sectors >> shift;
	} else {
		const unsigned int shift =
			log2_bytes_per_sector - log2_bytes_per_block;

		ufb->log2_blocks_per_cluster =
			log2_sectors_per_cluster + shift;
		ufb->fat_start = reserved_sector_count << shift;
		ufb->fat_size = sectors_per_fat << shift;
		ufb->root_size = root_sectors << shift;
	}

	if (!number_of_fats)
		return -UFAT_ERR_INVALID_BPB;

	/* Various block-size independent values */
	ufb->fat_count = number_of_fats;
	ufb->num_clusters =
		((total_logical_sectors - reserved_sector_count -
		  sectors_per_fat * number_of_fats -
		  root_sectors) >>
		 log2_sectors_per_cluster) + 2;
	ufb->root_cluster = root_cluster & UFAT_CLUSTER_MASK;
	ufb->root_start = ufb->fat_start + ufb->fat_size * ufb->fat_count;
	ufb->cluster_start = ufb->root_start + ufb->root_size;

	/* Figure out filesystem type */
	if (!root_sectors) {
		ufb->type = UFAT_TYPE_FAT32;
	} else {
		ufb->root_cluster = 0;
		if (ufb->num_clusters <= UFAT_MAX_FAT12)
			ufb->type = UFAT_TYPE_FAT12;
		else
			ufb->type = UFAT_TYPE_FAT16;
	}

	return 0;
}

static int read_bpb(struct ufat *uf)
{
	int idx;

	idx = ufat_cache_open(uf, 0, 0);
	if (idx < 0)
		return idx;

	return parse_bpb(uf->dev->log2_block_size, &uf->bpb,
			 ufat_cache_data(uf, idx));
}

int ufat_open(struct ufat *uf, const struct ufat_device *dev)
{
	uf->dev = dev;

	uf->next_seq = 0;
	uf->cache_size = UFAT_CACHE_BYTES >> dev->log2_block_size;

	if (uf->cache_size > UFAT_CACHE_MAX_BLOCKS)
		uf->cache_size = UFAT_CACHE_MAX_BLOCKS;

	if (!uf->cache_size)
		return -UFAT_ERR_BLOCK_SIZE;

	uf->alloc_ptr = 0;
	memset(&uf->stat, 0, sizeof(uf->stat));
	memset(&uf->cache_desc, 0, sizeof(uf->cache_desc));

	return read_bpb(uf);
}

int ufat_sync(struct ufat *uf)
{
	unsigned int i;
	int ret = 0;

	for (i = 0; i < uf->cache_size; i++) {
		int err = cache_flush(uf, i);

		if (err)
			ret = err;
	}

	return ret;
}

int ufat_count_free_clusters(struct ufat *uf, ufat_cluster_t *free_clusters)
{
	ufat_cluster_t idx;
	ufat_cluster_t local_free_clusters = 0;
	const ufat_cluster_t total = uf->bpb.num_clusters;

	/* Skip first two "special" clusters */
	for (idx = 2; idx < total; idx++) {
		ufat_cluster_t c;
		int err;

		/* Never use this cluster index in a FAT12 system */
		if (idx == 0xff0 && uf->bpb.type == UFAT_TYPE_FAT12)
			continue;

		err = ufat_read_fat(uf, idx, &c);
		if (err < 0)
			return err;

		if (c == UFAT_CLUSTER_FREE)
			local_free_clusters++;
	}

	*free_clusters = local_free_clusters;
	return 0;
}

void ufat_close(struct ufat *uf)
{
	ufat_sync(uf);
}

const char *ufat_strerror(int err)
{
	static const char *const text[UFAT_MAX_ERR] = {
		[UFAT_OK] = "No error",
		[UFAT_ERR_IO] = "IO error",
		[UFAT_ERR_BLOCK_SIZE] = "Invalid block size",
		[UFAT_ERR_INVALID_BPB] = "Invalid BPB",
		[UFAT_ERR_BLOCK_ALIGNMENT] =
		"Filesystem is not aligned for this block size",
		[UFAT_ERR_INVALID_CLUSTER] = "Invalid cluster index",
		[UFAT_ERR_NAME_TOO_LONG] = "Filename too long",
		[UFAT_ERR_NOT_DIRECTORY] = "Not a directory",
		[UFAT_ERR_NOT_FILE] = "Not a file",
		[UFAT_ERR_IMMUTABLE] = "Can't delete/modify this entry",
		[UFAT_ERR_DIRECTORY_NOT_EMPTY] = "Directory not empty",
		[UFAT_ERR_ILLEGAL_NAME] = "Illegal filename",
		[UFAT_ERR_FILE_EXISTS] = "File already exists",
		[UFAT_ERR_BAD_ENCODING] = "Bad encoding",
		[UFAT_ERR_DIRECTORY_FULL] = "Directory is full",
		[UFAT_ERR_NO_CLUSTERS] = "No free clusters"
	};

	if (err < 0)
		err = -err;

	if (err >= UFAT_MAX_ERR)
		return "Invalid error code";

	return text[err];
}

static int read_fat_byte(struct ufat *uf, unsigned int offset, uint8_t *out)
{
	ufat_block_t b = offset >> uf->dev->log2_block_size;
	unsigned int r = offset & ((1 << uf->dev->log2_block_size) - 1);
	int idx;

	idx = ufat_cache_open(uf, uf->bpb.fat_start + b, 0);
	if (idx < 0)
		return idx;

	*out = ufat_cache_data(uf, idx)[r];
	return 0;
}

static int read_fat12(struct ufat *uf, ufat_cluster_t index,
		      ufat_cluster_t *out)
{
	unsigned int offset = (index >> 1) * 3;
	uint8_t a;
	uint8_t b;
	uint16_t raw;
	int err;

	err = read_fat_byte(uf, offset + 1, &a);
	if (err < 0)
		return err;

	if (index & 1) {
		err = read_fat_byte(uf, offset + 2, &b);
		if (err < 0)
			return err;

		raw = (((ufat_cluster_t)b) << 4) |
			(((ufat_cluster_t)a) >> 4);
	} else {
		err = read_fat_byte(uf, offset, &b);
		if (err < 0)
			return err;

		raw = ((ufat_cluster_t)b) |
			((((ufat_cluster_t)a) & 0xf) << 8);
	}

	if (raw >= 0xff8 || raw == 0xff0) {
		*out = UFAT_CLUSTER_EOC;
		return 0;
	}

	if (raw == 0xff7) {
		*out = UFAT_CLUSTER_BAD;
		return 0;
	}

	*out = raw;
	return 0;
}

static int read_fat16(struct ufat *uf, ufat_cluster_t index,
		      ufat_cluster_t *out)
{
	const unsigned int shift = uf->dev->log2_block_size - 1;
	const unsigned int b = index >> shift;
	const unsigned int r = index & ((1 << shift) - 1);
	int i = ufat_cache_open(uf, uf->bpb.fat_start + b, 0);
	uint16_t raw;

	if (i < 0)
		return i;

	raw = r16(ufat_cache_data(uf, i) + r * 2);

	if (raw >= 0xfff8) {
		*out = UFAT_CLUSTER_EOC;
		return 0;
	}

	if (raw == 0xfff7) {
		*out = UFAT_CLUSTER_BAD;
		return 0;
	}

	*out = raw;
	return 0;
}

static int read_fat32(struct ufat *uf, ufat_cluster_t index,
		      ufat_cluster_t *out)
{
	const unsigned int shift = uf->dev->log2_block_size - 2;
	const unsigned int b = index >> shift;
	const unsigned int r = index & ((1 << shift) - 1);
	int i = ufat_cache_open(uf, uf->bpb.fat_start + b, 0);
	uint32_t raw;

	if (i < 0)
		return i;

	raw = r32(ufat_cache_data(uf, i) + r * 4) & UFAT_CLUSTER_MASK;

	if (raw >= 0xffffff8) {
		*out = UFAT_CLUSTER_EOC;
		return 0;
	}

	if (raw == 0xffffff7) {
		*out = UFAT_CLUSTER_BAD;
		return 0;
	}

	*out = raw;
	return 0;
}

int ufat_read_fat(struct ufat *uf, ufat_cluster_t index,
		  ufat_cluster_t *out)
{
	if (index >= uf->bpb.num_clusters)
		return -UFAT_ERR_INVALID_CLUSTER;

	switch (uf->bpb.type) {
	case UFAT_TYPE_FAT12: return read_fat12(uf, index, out);
	case UFAT_TYPE_FAT16: return read_fat16(uf, index, out);
	case UFAT_TYPE_FAT32: return read_fat32(uf, index, out);
	}

	return 0;
}

static int write_fat_byte(struct ufat *uf, unsigned int offset,
			  uint8_t byte, uint8_t mask)
{
	ufat_block_t b = offset >> uf->dev->log2_block_size;
	unsigned int r = offset & ((1 << uf->dev->log2_block_size) - 1);
	int idx;
	uint8_t *data;

	idx = ufat_cache_open(uf, uf->bpb.fat_start + b, 0);
	if (idx < 0)
		return idx;

	data = ufat_cache_data(uf, idx);
	ufat_cache_write(uf, idx);

	data[r] = (data[r] & ~mask) | (byte & mask);
	return 0;
}

static int write_fat12(struct ufat *uf, ufat_cluster_t index,
		       ufat_cluster_t in)
{
	unsigned int offset = (index >> 1) * 3;
	int err;

	if (index & 1) {
		err = write_fat_byte(uf, offset + 1,
				     (in & 0xf) << 4, 0xf0);
		if (err < 0)
			return err;

		return write_fat_byte(uf, offset + 2,
				      (in & 0xff0) >> 4, 0xff);
	}

	err = write_fat_byte(uf, offset, in & 0xff, 0xff);
	if (err < 0)
		return err;

	return write_fat_byte(uf, offset + 1, (in & 0xf00) >> 8, 0x0f);
}

static int write_fat16(struct ufat *uf, ufat_cluster_t index,
		       ufat_cluster_t in)
{
	const unsigned int shift = uf->dev->log2_block_size - 1;
	const unsigned int b = index >> shift;
	const unsigned int r = index & ((1 << shift) - 1);
	int i = ufat_cache_open(uf, uf->bpb.fat_start + b, 0);

	if (i < 0)
		return i;

	ufat_cache_write(uf, i);
	w16(ufat_cache_data(uf, i) + r * 2, in & 0xffff);

	return 0;
}

static int write_fat32(struct ufat *uf, ufat_cluster_t index,
		       ufat_cluster_t in)
{
	const unsigned int shift = uf->dev->log2_block_size - 2;
	const unsigned int b = index >> shift;
	const unsigned int r = index & ((1 << shift) - 1);
	int i = ufat_cache_open(uf, uf->bpb.fat_start + b, 0);

	if (i < 0)
		return i;

	ufat_cache_write(uf, i);
	w32(ufat_cache_data(uf, i) + r * 4, in);

	return 0;
}

int ufat_write_fat(struct ufat *uf, ufat_cluster_t index,
		   ufat_cluster_t in)
{
	if (index >= uf->bpb.num_clusters)
		return -UFAT_ERR_INVALID_CLUSTER;

	switch (uf->bpb.type) {
	case UFAT_TYPE_FAT12: return write_fat12(uf, index, in);
	case UFAT_TYPE_FAT16: return write_fat16(uf, index, in);
	case UFAT_TYPE_FAT32: return write_fat32(uf, index, in);
	}

	return 0;
}

int ufat_free_chain(struct ufat *uf, ufat_cluster_t c)
{
	while (UFAT_CLUSTER_IS_PTR(c)) {
		ufat_cluster_t next;
		int i = ufat_read_fat(uf, c, &next);

		if (i < 0)
			return i;

		i = ufat_write_fat(uf, c, UFAT_CLUSTER_FREE);
		if (i < 0)
			return i;

		c = next;
	}

	return 0;
}

static int alloc_cluster(struct ufat *uf, ufat_cluster_t *out,
			 ufat_cluster_t tail)
{
	const unsigned int total = uf->bpb.num_clusters - 2;
	unsigned int i;

	for (i = 0; i < total; i++) {
		const ufat_cluster_t idx = uf->alloc_ptr + 2;
		ufat_cluster_t c;
		int err;

		uf->alloc_ptr = (uf->alloc_ptr + 1) % total;

		/* Never use this cluster index in a FAT12 system */
		if (idx == 0xff0 && uf->bpb.type == UFAT_TYPE_FAT12)
			continue;

		err = ufat_read_fat(uf, idx, &c);

		if (err < 0)
			return err;

		if (c == UFAT_CLUSTER_FREE) {
			err = ufat_write_fat(uf, idx, tail);

			if (err < 0)
				return err;

			*out = idx;
			return 0;
		}
	}

	return -UFAT_ERR_NO_CLUSTERS;
}

int ufat_alloc_chain(struct ufat *uf, unsigned int count, ufat_cluster_t *out)
{
	ufat_cluster_t chain = UFAT_CLUSTER_EOC;

	while (count) {
		int err = alloc_cluster(uf, &chain, chain);

		if (err < 0) {
			ufat_free_chain(uf, chain);
			return err;
		}

		count--;
	}

	*out = chain;
	return 0;
}
