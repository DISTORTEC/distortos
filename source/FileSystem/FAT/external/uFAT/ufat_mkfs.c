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
	ufat_cluster_t est_clusters;
	uint32_t fat_bytes;

	/* Make sure the block size is greater than our minimum supported
	 * sector size.
	 */
	if (log2_block_size < 9)
		return -UFAT_ERR_BLOCK_SIZE;

	/* Default sector size is 512, but this is increased if we can't
	 * store the total logical sector count in a 32-bit word.
	 */
	fl->log2_sector_size = 9;
	while ((fl->log2_sector_size < log2_block_size) &&
	       (fl->log2_sector_size < 32768) &&
	       (nblk >> (fl->log2_sector_size + 32)))
		fl->log2_sector_size++;

	/* If we still can't fit it, we'll have to chop the device. */
	if (nblk >> (fl->log2_sector_size + 32))
		nblk = 0xffffffffLL << fl->log2_sector_size;

	/* Choose the smallest cluster size greater than 1 kB which keeps
	 * us within the FAT entry count limit.
	 */
	if (log2_block_size > 10)
		fl->log2_bpc = 0;
	else
		fl->log2_bpc = 10 - log2_block_size;

	while (fl->log2_bpc + log2_block_size < fl->log2_sector_size + 7 &&
	       (nblk >> fl->log2_bpc) >= 0x10000000LL)
		fl->log2_bpc++;

	/* If we still can't fit it, we'll have to chop the device. */
	if (nblk >> fl->log2_bpc >= 0x10000000LL)
		nblk = 0xfffffffLL << fl->log2_bpc;

	/* Calculate the number of reserved blocks */
	fl->reserved_blocks = bytes_to_blocks(log2_block_size,
					      2 << fl->log2_sector_size) << 1;

	/* Calculate the minimum size of the root directory (FAT12/FAT16). */
	fl->root_blocks = bytes_to_blocks(log2_block_size, 16384);

	/* Estimate an upper bound on the cluster count and allocate blocks
	 * for the FAT.
	 */
	est_clusters = ((nblk - fl->reserved_blocks) >> fl->log2_bpc) + 2;

	if (est_clusters > UFAT_MAX_FAT16)
		fat_bytes = est_clusters << 2;
	else if (est_clusters >= UFAT_MAX_FAT12)
		fat_bytes = est_clusters << 1;
	else
		fat_bytes = (est_clusters * 3 + 1) >> 1;

	fl->fat_blocks = bytes_to_blocks(log2_block_size, fat_bytes);

	/* Finalize the actual cluster count and FAT type. The final cluster
	 * count can't be greater than the estimate.
	 */
	fl->clusters = ((nblk - fl->reserved_blocks -
			 fl->root_blocks - fl->fat_blocks * 2) >>
			fl->log2_bpc) + 2;

	if (fl->clusters > UFAT_MAX_FAT16)
		fl->type = UFAT_TYPE_FAT32;
	else if (fl->clusters > UFAT_MAX_FAT12)
		fl->type = UFAT_TYPE_FAT16;
	else
		fl->type = UFAT_TYPE_FAT12;

	/* If FAT32, consume root directory blocks for clusters. Otherwise,
	 * expand root directory to fill unusable data space.
	 */
	if (fl->type == UFAT_TYPE_FAT32) {
		fl->root_blocks = 0;
		fl->clusters = ((nblk - fl->reserved_blocks -
				 fl->fat_blocks * 2) >> fl->log2_bpc) + 2;
	} else {
		fl->root_blocks =
			nblk - fl->reserved_blocks - fl->fat_blocks * 2 -
			((fl->clusters - 2) << fl->log2_bpc);
	}

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
	if (fl->logical_blocks <= 0xffff)
		w16(buf + 0x013, fl->logical_blocks << log2_spb);
	else
		w32(buf + 0x020, fl->logical_blocks << log2_spb);
	buf[0x015] = MEDIA_DISK;
	if (fl->type != UFAT_TYPE_FAT32)
		w16(buf + 0x016, fl->fat_blocks << log2_spb);

	if (fl->type == UFAT_TYPE_FAT32) {
		w32(buf + 0x024, fl->fat_blocks << log2_spb);
		w32(buf + 0x02c, 2); /* Root directory cluster */
		w16(buf + 0x030, 1); /* FS informations sector */
		w16(buf + 0x032, backup << log2_spb);
		memcpy(buf + 0x052, type_name, 8);
	}

	memset(buf + 0x2b, ' ', 11);
	memcpy(buf + 0x36, type_name, 8);
	buf[0x042] = 0x29;

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

	for (i = 0; i < fl->fat_blocks; i++) {
		uint8_t buf[block_size];
		unsigned int j;

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

		if (!i) {
			buf[0] = MEDIA_DISK;
			buf[1] = 0x8f;
			buf[2] = 0xff;
		}

		if (dev->write(dev, fl->reserved_blocks + i, 1, buf) < 0 ||
		    dev->write(dev, fl->reserved_blocks + fl->fat_blocks + i,
			       1, buf) < 0)
			return -UFAT_ERR_IO;
	}

	return 0;
}

static int init_fat16(struct ufat_device *dev, const struct fs_layout *fl)
{
	const unsigned int block_size = 1 << dev->log2_block_size;
	ufat_block_t i;
	ufat_cluster_t c = 0;

	for (i = 0; i < fl->fat_blocks; i++) {
		uint8_t buf[block_size];
		unsigned int j;

		memset(buf, 0, block_size);

		for (j = 0; j < block_size; j += 2) {
			if (c >= fl->clusters) {
				buf[j] = 0xf7;
				buf[j + 1] = 0xff;
			}

			c++;
		}

		if (!i) {
			buf[0] = MEDIA_DISK;
			buf[1] = 0xff;
			buf[2] = 0xf8;
			buf[3] = 0xff;
		}

		if (dev->write(dev, fl->reserved_blocks + i, 1, buf) < 0 ||
		    dev->write(dev, fl->reserved_blocks + fl->fat_blocks + i,
			       1, buf) < 0)
			return -UFAT_ERR_IO;
	}

	return 0;
}

static int init_fat32(struct ufat_device *dev, const struct fs_layout *fl)
{
	const unsigned int block_size = 1 << dev->log2_block_size;
	ufat_block_t i;
	ufat_cluster_t c = 0;

	for (i = 0; i < fl->fat_blocks; i++) {
		uint8_t buf[block_size];
		unsigned int j;

		memset(buf, 0, block_size);

		for (j = 0; j < block_size; j += 4) {
			if (c >= fl->clusters) {
				buf[j] = 0xf7;
				buf[j + 1] = 0xff;
				buf[j + 2] = 0xff;
				buf[j + 3] = 0xff;
			}

			c++;
		}

		if (!i) {
			buf[0] = MEDIA_DISK;
			buf[1] = 0xff;
			buf[2] = 0xff;
			buf[3] = 0xff;
			buf[4] = 0xf8;
			buf[5] = 0xff;
			buf[6] = 0xff;
			buf[7] = 0xff;
		}

		if (dev->write(dev, fl->reserved_blocks + i, 1, buf) < 0 ||
		    dev->write(dev, fl->reserved_blocks + fl->fat_blocks + i,
			       1, buf) < 0)
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
