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

#ifndef UFAT_INTERNAL_H_
#define UFAT_INTERNAL_H_

#define UFAT_CLUSTER_MASK		0x0fffffff
#define UFAT_DIRENT_SIZE		32

#define UFAT_MAX_FAT12			0xff6
#define UFAT_MAX_FAT16			0xfff6

static inline uint16_t r16(const uint8_t *offset)
{
	const uint16_t l = offset[0];
	const uint16_t h = offset[1];

	return (h << 8) | l;
}

static inline uint32_t r32(const uint8_t *offset)
{
	const uint32_t l = r16(offset);
	const uint32_t h = r16(offset + 2);

	return (h << 16) | l;
}

static inline void w16(uint8_t *offset, uint16_t value)
{
	offset[0] = value & 0xff;
	offset[1] = value >> 8;
}

static inline void w32(uint8_t *offset, uint32_t value)
{
	w16(offset, value & 0xffff);
	w16(offset + 2, value >> 16);
}

static inline ufat_block_t cluster_to_block(const struct ufat_bpb *bpb,
					    ufat_cluster_t c)
{
	return ((c - 2) << bpb->log2_blocks_per_cluster) +
		bpb->cluster_start;
}

static inline ufat_cluster_t block_to_cluster(const struct ufat_bpb *bpb,
					      ufat_block_t b)
{
	return ((b - bpb->cluster_start) >> bpb->log2_blocks_per_cluster) + 2;
}

/* Block IO via internal cache */

/**
 * \brief Opens a block via cache.
 *
 * \pre `uf` is a valid pointer.
 * \pre The filesystem pointed by `uf` is opened.
 *
 * \param [in] uf is a pointer to the filesystem
 * \param [in] blk_index is the index of block which should be opened
 * \param [in] skip_read selects the behavior when block is not present in the
 * cache:
 * - 0 - current contents of the block are read from the device;
 * - not 0 - read operation is skipped and the cached contents are just zeroed -
 * this is useful when the whole block is going to be overwritten anyway;
 *
 * \return cache index on success, negative error code (`ufat_error_t`)
 * otherwise
 */

int ufat_cache_open(struct ufat *uf, ufat_block_t blk_index, int skip_read);

/**
 * \brief Evicts (flushes) cached blocks which overlap with given range.
 *
 * \pre `uf` is a valid pointer.
 * \pre The filesystem pointed by `uf` is opened.
 *
 * \param [in] uf is a pointer to the filesystem
 * \param [in] start is the index of starting block of the range
 * \param [in] count is the number of blocks in the range
 *
 * \return 0 on success, negative error code (`ufat_error_t`) otherwise
 */

int ufat_cache_evict(struct ufat *uf, ufat_block_t start, ufat_block_t count);

/**
 * \brief Invalidates (drops) cached blocks which overlap with given range.
 *
 * \pre `uf` is a valid pointer.
 * \pre The filesystem pointed by `uf` is opened.
 *
 * \param [in] uf is a pointer to the filesystem
 * \param [in] start is the index of starting block of the range
 * \param [in] count is the number of blocks in the range
 */

void ufat_cache_invalidate(struct ufat *uf, ufat_block_t start,
			   ufat_block_t count);

static inline void ufat_cache_write(struct ufat *uf, unsigned int cache_index)
{
	uf->stat.cache_write++;
	uf->cache_desc[cache_index].flags |= UFAT_CACHE_FLAG_DIRTY;
}

static inline uint8_t *ufat_cache_data(struct ufat *uf,
				       unsigned int cache_index)
{
	return uf->cache_data + (cache_index << uf->dev->log2_block_size);
}

/* FAT entry IO */
int ufat_read_fat(struct ufat *uf, ufat_cluster_t index,
		  ufat_cluster_t *out);
int ufat_write_fat(struct ufat *uf, ufat_cluster_t index,
		   ufat_cluster_t in);

/* High-level FAT operations */
int ufat_free_chain(struct ufat *uf, ufat_cluster_t start);
int ufat_alloc_chain(struct ufat *uf, unsigned int count, ufat_cluster_t *out);

/* LFN handling */
struct ufat_lfn_parser {
	ufat_block_t	start_block;
	unsigned int	start_pos;

	uint8_t		short_checksum;

	int		len;
	int		seq;
	uint16_t	buf[UFAT_LFN_MAX_CHARS];
};

static inline void ufat_lfn_reset(struct ufat_lfn_parser *s)
{
	s->seq = -1;
}

static inline int ufat_lfn_ok(const struct ufat_lfn_parser *s)
{
	return s->seq == 0;
}

void ufat_lfn_parse(struct ufat_lfn_parser *s, const uint8_t *data,
		    ufat_block_t blk, unsigned int pos);
int ufat_lfn_is_legal(const char *name);
void ufat_lfn_pack_fragment(const uint16_t *ucs, int seq, int is_first,
			    uint8_t *data, uint8_t checksum);

/* Raw dirent IO */
int ufat_write_raw_dirent(struct ufat_directory *dir,
			  const uint8_t *data, unsigned int len);
int ufat_read_raw_dirent(struct ufat_directory *dir, uint8_t *data);
int ufat_advance_raw_dirent(struct ufat_directory *dir, int can_alloc);
int ufat_allocate_raw_dirent(struct ufat_directory *dir, unsigned int count);
int ufat_init_dirent_cluster(struct ufat *uf, ufat_cluster_t c);

/* Dirent parsing/packing */
void ufat_parse_dirent(ufat_fat_type_t type,
		       const uint8_t *data, struct ufat_dirent *inf);
void ufat_pack_dirent(const struct ufat_dirent *ent, uint8_t *data);

/* Charset conversion */
int ufat_ucs2_to_utf8(const uint16_t *src, int src_len,
		      char *dst, int dst_len);
int ufat_utf8_to_ucs2(const char *src, uint16_t *dst);

/* Short name functions */
void ufat_short_first(const char *long_name,
		      char *short_name, char *ext_text);
void ufat_short_next(char *short_name);
int ufat_short_exists(struct ufat_directory *dir, const char *short_name,
		      const char *short_ext);
uint8_t ufat_short_checksum(const char *short_name, const char *short_ext);
int ufat_format_short(const char *name, const char *ext,
		      char *out, int max_len);

/* Long name comparison.
 *
 * Names are compared without regard to case. If component_only is set,
 * then any '/' or '\\' character in search_name is treated as a
 * terminator.
 *
 * Returns the length of the match, or -1 if the names don't match.
 */
int ufat_compare_name(const char *search_name, const char *dir_name,
		      int component_only);

#endif
