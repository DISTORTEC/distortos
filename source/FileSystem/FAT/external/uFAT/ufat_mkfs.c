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

#define MEDIA_DISK	0xf8

struct fs_layout {
	unsigned int	log2_sector_size;
	unsigned int	log2_bpc;

	ufat_block_t	reserved_blocks;
	ufat_block_t	root_blocks;
	ufat_block_t	fat_blocks;
	ufat_block_t	logical_blocks;

	ufat_cluster_t	clusters;
	ufat_fat_type_t type;
};

static inline ufat_block_t bytes_to_blocks(unsigned int log2_block_size,
					   uint32_t bytes)
{
	return (bytes + (1 << log2_block_size) - 1) >> log2_block_size;
}

static int calculate_layout(struct fs_layout *fl,
			    ufat_block_t nblk,
			    unsigned int log2_block_size)
{
	unsigned int log2_bps;
	unsigned int log2_spc;
	uint32_t nsect;
	ufat_cluster_t clusters_threshold;
	ufat_cluster_t est_clusters;
	uint32_t fat_bytes;

	/* Make sure the block size is less than or equal to maximum sector
	 * size (4 kB, log2(4096) = 12).
	 */
	if (log2_block_size > 12)
		return -UFAT_ERR_BLOCK_SIZE;

	/* Minimum sector size is 512 B (log2(512) = 9), but it cannot be
	 * smaller than block size.
	 */
	log2_bps = log2_block_size < 9 ? 9 - log2_block_size : 0;

	/* Increase sector size if we can't store the total logical sector count
	 * in a 32-bit variable.
	 */
	while (log2_block_size + log2_bps < 12 &&
	       nblk >> log2_bps > UINT32_MAX)
		++log2_bps;

	/* If we still can't fit it, we'll have to chop the device. */
	if (nblk >> log2_bps > UINT32_MAX)
		nblk = (ufat_block_t)UINT32_MAX << log2_bps;

	fl->log2_sector_size = log2_block_size + log2_bps;

	/* Calculate total logical sector count. */
	nsect = nblk >> log2_bps;

	/* Calculate the number of reserved blocks. */
	fl->reserved_blocks = bytes_to_blocks(log2_block_size,
					      2 << fl->log2_sector_size) << 1;

	/* Threshold values taken from "fatgen103.pdf" -
	 * https://staff.washington.edu/dittrich/misc/fatgen103.pdf - "FAT
	 * Volume Initialization" chapter.
	 *
	 * For a device with typical 512 B block size this selects FAT12 for
	 * device size less than ~4.1 MB, FAT16 for device size less than 512 MB
	 * and FAT32 otherwise.
	 */
	if (nsect < 8400) {
		fl->type = UFAT_TYPE_FAT12;
		clusters_threshold = 1 << 12;
		log2_spc = 1;
	} else if (nsect < 1048576) {
		fl->type = UFAT_TYPE_FAT16;
		clusters_threshold = 1 << 16;
		log2_spc = 1;
	} else {
		fl->type = UFAT_TYPE_FAT32;
		clusters_threshold = 2097152;
		log2_spc = 3;
	}

	/* Increase cluster size if the resulting number of clusters would be
	 * above the threshold, but keep it below 32 kB (log2(32768) = 15).
	 */
	while (log2_spc < 7 &&
	       fl->log2_sector_size + log2_spc < 15 &&
	       nsect >> log2_spc > clusters_threshold)
		++log2_spc;

	fl->log2_bpc = log2_bps + log2_spc;

	/* Estimate an upper bound on the cluster count and allocate blocks
	 * for the FAT.
	 */
	est_clusters = ((nblk - fl->reserved_blocks) >> fl->log2_bpc) + 2;

	if (fl->type == UFAT_TYPE_FAT32)
		fat_bytes = est_clusters << 2;
	else if (fl->type == UFAT_TYPE_FAT16)
		fat_bytes = est_clusters << 1;
	else
		fat_bytes = (est_clusters * 3 + 1) >> 1;

	fl->fat_blocks = bytes_to_blocks(log2_block_size, fat_bytes);

	/* Calculate the minimum size of the root directory. */
	fl->root_blocks = fl->type != UFAT_TYPE_FAT32 ?
	                  bytes_to_blocks(log2_block_size, 16384) : 0;

	/* Finalize the actual cluster count - it can't be greater than the
	 * estimate.
	 */
	fl->clusters = ((nblk - fl->reserved_blocks -
			 fl->root_blocks - fl->fat_blocks * 2) >>
			fl->log2_bpc) + 2;

	/* Expand root directory to fill unusable data space for FAT12/FAT16. */
	if (fl->type != UFAT_TYPE_FAT32)
		fl->root_blocks =
			nblk - fl->reserved_blocks - fl->fat_blocks * 2 -
			((fl->clusters - 2) << fl->log2_bpc);

	/* Set the block count to exactly fit the filesystem. */
	fl->logical_blocks = ((fl->clusters - 2) << fl->log2_bpc) +
		fl->fat_blocks * 2 + fl->reserved_blocks + fl->root_blocks;

	return 0;
}

static int write_bpb(struct ufat_device *dev, const struct fs_layout *fl)
{
	static const uint8_t boot_header[11] = {
		0xeb, 0xfe, /* jmp $ */
		0x90,       /* nop */
		'u', 'f', 'a', 't', ' ', ' ', ' ', ' '
	};
	const char *type_name = "FAT     ";
	const unsigned int log2_spb =
		dev->log2_block_size - fl->log2_sector_size;
	const unsigned int block_size = 1 << dev->log2_block_size;
	const ufat_block_t backup = fl->reserved_blocks >> 1;
	uint8_t buf[block_size];
	ufat_block_t i;

	switch (fl->type) {
	case UFAT_TYPE_FAT12:
		type_name = "FAT12   ";
		break;

	case UFAT_TYPE_FAT16:
		type_name = "FAT16   ";
		break;

	case UFAT_TYPE_FAT32:
		type_name = "FAT32   ";
		break;
	}

	/* All reserved blocks are zeroed except for the one containing the
	 * boot sector.
	 */
	memset(buf, 0, sizeof(buf));
	for (i = 1; i < backup; i++)
		if (dev->write(dev, i, 1, buf) < 0 ||
		    dev->write(dev, i + backup, 1, buf) < 0)
			return -UFAT_ERR_IO;

	/* Boot sector signature */
	memcpy(buf, boot_header, sizeof(boot_header));
	buf[0x1fe] = 0x55;
	buf[0x1ff] = 0xaa;

	/* BIOS Parameter Block */
	w16(buf + 0x00b, 1 << fl->log2_sector_size);
	buf[0x00d] = 1 << (fl->log2_bpc - log2_spb);
	w16(buf + 0x00e, fl->reserved_blocks << log2_spb);
	buf[0x010] = 2; /* 2 FATs */
	w16(buf + 0x011, fl->root_blocks << (dev->log2_block_size - 5));
	if (fl->type != UFAT_TYPE_FAT32 && fl->logical_blocks <= UINT16_MAX)
		w16(buf + 0x013, fl->logical_blocks << log2_spb);
	else
		w32(buf + 0x020, fl->logical_blocks << log2_spb);
	buf[0x015] = MEDIA_DISK;

	if (fl->type != UFAT_TYPE_FAT32) {
		w16(buf + 0x016, fl->fat_blocks << log2_spb);
		buf[0x026] = 0x29; /* Extended boot signature */
		memset(buf + 0x02b, ' ', 11); /* Volume label */
		memcpy(buf + 0x036, type_name, 8);
	} else {
		w32(buf + 0x024, fl->fat_blocks << log2_spb);
		w32(buf + 0x02c, 2); /* Root directory cluster */
		w16(buf + 0x030, 1); /* FS informations sector */
		w16(buf + 0x032, backup << log2_spb);
		buf[0x042] = 0x29; /* Extended boot signature */
		memset(buf + 0x047, ' ', 11); /* Volume label */
		memcpy(buf + 0x052, type_name, 8);
	}

	/* Write boot sector and backup */
	if (dev->write(dev, 0, 1, buf) < 0 ||
	    dev->write(dev, backup, 1, buf) < 0)
		return -UFAT_ERR_IO;

	return 0;
}

static int init_fat12(struct ufat_device *dev, const struct fs_layout *fl)
{
	const unsigned int block_size = 1 << dev->log2_block_size;
	unsigned int minor_byte = 0;
	unsigned int cluster_pair = 0;
	ufat_block_t i;
	ufat_block_t block;

	for (i = 0, block = 0; i < fl->fat_blocks * 2; i++, block++) {
		uint8_t buf[block_size];
		unsigned int j;

		if (block == fl->fat_blocks) {
			block = 0;
			minor_byte = 0;
			cluster_pair = 0;
		}

		memset(buf, 0, block_size);

		for (j = 0; j < block_size; j++) {
			uint32_t pair_data = 0;

			if ((cluster_pair << 1) >= fl->clusters)
				pair_data = 0xff7ff7;
			else if ((cluster_pair << 1) + 1 >= fl->clusters)
				pair_data = 0xff7000;

			buf[j] = pair_data >> (minor_byte << 3);

			if (++minor_byte >= 3) {
				minor_byte = 0;
				cluster_pair++;
			}
		}

		if (block == 0) {
			buf[0] = MEDIA_DISK;
			buf[1] = 0x8f;
			buf[2] = 0xff;
		}

		if (dev->write(dev, fl->reserved_blocks + i, 1, buf) < 0)
			return -UFAT_ERR_IO;
	}

	return 0;
}

static int init_fat16(struct ufat_device *dev, const struct fs_layout *fl)
{
	const unsigned int block_size = 1 << dev->log2_block_size;
	ufat_block_t i;
	ufat_block_t block;
	ufat_cluster_t c;

	for (i = 0, block = 0, c = 0; i < fl->fat_blocks * 2; i++, block++) {
		uint8_t buf[block_size];
		unsigned int j;

		if (block == fl->fat_blocks) {
			block = 0;
			c = 0;
		}

		memset(buf, 0, block_size);

		for (j = 0; j < block_size; j += 2) {
			if (c >= fl->clusters)
				w16(buf + j, 0xfff7);

			c++;
		}

		if (block == 0) {
			w16(buf, 0xff00 | MEDIA_DISK);
			w16(buf + 2, 0xfff8);
		}

		if (dev->write(dev, fl->reserved_blocks + i, 1, buf) < 0)
			return -UFAT_ERR_IO;
	}

	return 0;
}

static int init_fat32(struct ufat_device *dev, const struct fs_layout *fl)
{
	const unsigned int block_size = 1 << dev->log2_block_size;
	ufat_block_t i;
	ufat_block_t block;
	ufat_cluster_t c;

	for (i = 0, block = 0, c = 0; i < fl->fat_blocks * 2; i++, block++) {
		uint8_t buf[block_size];
		unsigned int j;

		if (block == fl->fat_blocks) {
			block = 0;
			c = 0;
		}

		memset(buf, 0, block_size);

		for (j = 0; j < block_size; j += 4) {
			if (c >= fl->clusters)
				w32(buf + j, 0xfffffff7);

			c++;
		}

		if (block == 0) {
			w32(buf + 0, 0xffffff00 | MEDIA_DISK);
			w32(buf + 4, 0xfffffff8);
			w32(buf + 8, 0xfffffff8);
		}

		if (dev->write(dev, fl->reserved_blocks + i, 1, buf) < 0)
			return -UFAT_ERR_IO;
	}

	return 0;
}

static int init_root_blocks(struct ufat_device *dev, const struct fs_layout *fl)
{
	const ufat_block_t root_start =
		fl->fat_blocks * 2 + fl->reserved_blocks;
	const unsigned int block_size = 1 << dev->log2_block_size;
	uint8_t buf[block_size];
	ufat_block_t i;

	memset(buf, 0, block_size);
	for (i = 0; i < fl->root_blocks; i++)
		if (dev->write(dev, root_start + i, 1, buf) < 0)
			return -UFAT_ERR_IO;

	return 0;
}

static int init_root_cluster(struct ufat_device *dev,
			     const struct fs_layout *fl)
{
	const ufat_block_t cluster_start =
		fl->fat_blocks * 2 + fl->reserved_blocks + fl->root_blocks;
	const unsigned int block_size = 1 << dev->log2_block_size;
	const unsigned int cluster_blocks = 1 << fl->log2_bpc;
	uint8_t buf[block_size];
	ufat_block_t i;

	memset(buf, 0, block_size);
	for (i = 0; i < cluster_blocks; i++)
		if (dev->write(dev, cluster_start + i, 1, buf) < 0)
			return -UFAT_ERR_IO;

	return 0;
}

int ufat_mkfs(struct ufat_device *dev, ufat_block_t nblk)
{
	struct fs_layout fl;
	int err;

	err = calculate_layout(&fl, nblk, dev->log2_block_size);
	if (err < 0)
		return err;

	err = write_bpb(dev, &fl);
	if (err < 0)
		return err;

	switch (fl.type) {
	case UFAT_TYPE_FAT12:
		err = init_fat12(dev, &fl);
		break;

	case UFAT_TYPE_FAT16:
		err = init_fat16(dev, &fl);
		break;

	case UFAT_TYPE_FAT32:
		err = init_fat32(dev, &fl);
		break;
	}

	if (err < 0)
		return err;

	if (fl.type == UFAT_TYPE_FAT32)
		return init_root_cluster(dev, &fl);

	return init_root_blocks(dev, &fl);
}
