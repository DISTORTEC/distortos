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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ufat.h"
#include "ufat_internal.h"

int ufat_write_raw_dirent(struct ufat_directory *dir,
			  const uint8_t *data, unsigned int len)
{
	int idx;

	if (dir->cur_block == UFAT_BLOCK_NONE)
		return -UFAT_ERR_IO;

	idx = ufat_cache_open(dir->uf, dir->cur_block);
	if (idx < 0)
		return idx;

	ufat_cache_write(dir->uf, idx);
	memcpy(ufat_cache_data(dir->uf, idx) +
	       dir->cur_pos * UFAT_DIRENT_SIZE,
	       data, len);

	return 0;
}

static int advance_block_in_chain(struct ufat_directory *dir, int can_alloc)
{
	const struct ufat_bpb *bpb = &dir->uf->bpb;
	const ufat_cluster_t cur_cluster =
		block_to_cluster(bpb, dir->cur_block);
	ufat_block_t next_block = dir->cur_block + 1;
	ufat_cluster_t next_cluster = block_to_cluster(bpb, next_block);
	int err;

	if (cur_cluster == next_cluster) {
		dir->cur_block = next_block;
		return 0;
	}

	/* We've crossed a cluster boundary. Look up the next cluster in
	 * the FAT.
	 */
	err = ufat_read_fat(dir->uf, cur_cluster, &next_cluster);
	if (err < 0)
		return err;

	if (UFAT_CLUSTER_IS_PTR(next_cluster)) {
		dir->cur_block = cluster_to_block(bpb, next_cluster);
		return 0;
	}

	/* This is the end of the chain. If we can't allocate, we're done. */
	if (!can_alloc) {
		dir->cur_block = UFAT_BLOCK_NONE;
		return 0;
	}

	/* Try to get a new cluster */
	err = ufat_alloc_chain(dir->uf, 1, &next_cluster);
	if (err < 0)
		return err;

	err = ufat_init_dirent_cluster(dir->uf, next_cluster);
	if (err < 0) {
		ufat_free_chain(dir->uf, next_cluster);
		return err;
	}

	err = ufat_write_fat(dir->uf, cur_cluster, next_cluster);
	if (err < 0) {
		ufat_free_chain(dir->uf, next_cluster);
		return err;
	}

	dir->cur_block = cluster_to_block(bpb, next_cluster);
	return 0;
}

int ufat_advance_raw_dirent(struct ufat_directory *dir, int can_alloc)
{
	/* Advance the dirent pointer and check for a block overrun */
	dir->cur_pos++;
	if (dir->cur_pos * UFAT_DIRENT_SIZE >=
	    (1u << dir->uf->dev->log2_block_size)) {
		dir->cur_pos = 0;

		if (dir->cur_block < dir->uf->bpb.cluster_start) {
			/* FAT12/16 root directory */
			dir->cur_block++;
			if (dir->cur_block >= dir->uf->bpb.cluster_start)
				dir->cur_block = UFAT_BLOCK_NONE;

			return 0;
		}

		return advance_block_in_chain(dir, can_alloc);
	}

	return 0;
}

int ufat_read_raw_dirent(struct ufat_directory *dir, uint8_t *data)
{
	int idx;

	if (dir->cur_block == UFAT_BLOCK_NONE)
		return 1;

	idx = ufat_cache_open(dir->uf, dir->cur_block);
	if (idx < 0)
		return idx;

	memcpy(data, ufat_cache_data(dir->uf, idx) +
	       dir->cur_pos * UFAT_DIRENT_SIZE,
	       UFAT_DIRENT_SIZE);

	return 0;
}

int ufat_allocate_raw_dirent(struct ufat_directory *dir, unsigned int count)
{
	ufat_block_t empty_start = UFAT_BLOCK_NONE;
	unsigned int empty_pos = 0;
	unsigned int empty_count = 0;

	ufat_dir_rewind(dir);

	for (;;) {
		uint8_t data[UFAT_DIRENT_SIZE];
		int err;

		err = ufat_read_raw_dirent(dir, data);
		if (err < 0)
			return err;

		/* Check to see if this raw dirent is empty. Keep track
		 * of contiguous chains of empty dirents.
		 */
		if (!data[0] || data[0] == 0xe5) {
			if (!empty_count) {
				empty_start = dir->cur_block;
				empty_pos = dir->cur_pos;
			}

			empty_count++;
		} else {
			empty_count = 0;
		}

		/* Is this chain long enough? */
		if (empty_count >= count)
			break;

		err = ufat_advance_raw_dirent(dir, 1);
		if (err < 0)
			return err;

		if (dir->cur_block == UFAT_BLOCK_NONE)
			return -UFAT_ERR_DIRECTORY_FULL;
	}

	/* Reposition at the start of the empty chain */
	dir->cur_block = empty_start;
	dir->cur_pos = empty_pos;

	return 0;
}

int ufat_init_dirent_cluster(struct ufat *uf, ufat_cluster_t c)
{
	const struct ufat_bpb *bpb = &uf->bpb;
	const ufat_block_t start = cluster_to_block(bpb, c);
	const unsigned int count =
		1 << bpb->log2_blocks_per_cluster;
	const unsigned int block_size = 1 << uf->dev->log2_block_size;
	int i;

	for (i = count - 1; i >= 0; i--) {
		int idx = ufat_cache_open(uf, start + i);

		if (idx < 0)
			return idx;

		ufat_cache_write(uf, idx);
		memset(ufat_cache_data(uf, idx), 0, block_size);
	}

	return 0;
}

static void sn_copy(const uint8_t *src, char *dst, int len)
{
	int i;

	for (i = 0; i < len && src[i] > ' '; i++)
		dst[i] = src[i];

	dst[i] = 0;
}

void ufat_parse_dirent(ufat_fat_type_t type,
		       const uint8_t *data, struct ufat_dirent *inf)
{
	sn_copy(data, inf->short_name, 8);
	sn_copy(data + 0x08, inf->short_ext, 3);

	inf->attributes = data[0x0b];
	inf->create_time = r16(data + 0x0e);
	inf->create_date = r16(data + 0x10);
	inf->access_date = r16(data + 0x12);
	inf->modify_time = r16(data + 0x16);
	inf->modify_date = r16(data + 0x18);
	inf->file_size = r32(data + 0x1c);
	inf->first_cluster = r16(data + 0x1a);

	if (type == UFAT_TYPE_FAT32) {
		uint32_t high = r16(data + 0x14);

		inf->first_cluster |= high << 16;
	}
}

void ufat_pack_dirent(const struct ufat_dirent *ent, uint8_t *data)
{
	memset(data, 0x20, 11);
	memcpy(data, ent->short_name, strlen(ent->short_name));
	memcpy(data + 0x08, ent->short_ext, strlen(ent->short_ext));

	data[0x0b] = ent->attributes;
	data[0x0c] = 0;
	data[0x0d] = 0;
	w16(data + 0x0e, ent->create_time);
	w16(data + 0x10, ent->create_date);
	w16(data + 0x12, ent->access_date);
	w16(data + 0x14, ent->first_cluster >> 16);
	w16(data + 0x16, ent->modify_time);
	w16(data + 0x18, ent->modify_date);
	w16(data + 0x1a, ent->first_cluster & 0xffff);
	w32(data + 0x1c, ent->file_size);
}

/************************************************************************
 * Name/LFN manipulation
 */

int ufat_lfn_is_legal(const char *name)
{
	if (!*name)
		return 0;

	if (name[0] == '.') {
		if (!name[1])
			return 0;
		if (name[1] == '.' && !name[2])
			return 0;
	}

	while (*name) {
		if (*name == '\\' || *name == '/' || *name < ' ')
			return 0;

		name++;
	}

	return 1;
}

static int is_legal_dos_char(char c)
{
	return isalnum(c) ||
		(c >= '!' && c <= ')') ||
		(c == '-') ||
		(c == '@') ||
		(c >= '^' && c <= '`') ||
		(c == '{' || c == '}' || c == '~');
}

void ufat_short_first(const char *long_name,
		      char *short_name, char *ext_text)
{
	int len = strlen(long_name);
	int ext = len;
	int i, j;

	while (ext >= 0 && long_name[ext] != '.')
		ext--;

	if (ext > 0)
		ext++;
	else
		ext = len;

	j = 0;
	for (i = ext; j < 3 && long_name[i]; i++) {
		char c = long_name[i];

		if (is_legal_dos_char(c))
			ext_text[j++] = toupper(c);
	}
	ext_text[j] = 0;

	j = 0;
	for (i = 0; j < 8 && i < ext; i++) {
		char c = long_name[i];

		if (is_legal_dos_char(c))
			short_name[j++] = toupper(c);
	}
	short_name[j] = 0;

	if (!j) {
		for (i = 0; i < 8; i++)
			short_name[i] = '~';
		short_name[8] = 0;
	}
}

void ufat_short_next(char *short_name)
{
	int len = strlen(short_name);
	int tilde = strlen(short_name);
	unsigned int suffix_num;
	int i;

	while (tilde >= 0 && short_name[tilde] != '~')
		tilde--;
	if (tilde < 0)
		tilde = len;
	else
		tilde++;

	suffix_num = atoi(short_name + tilde) + 1;
	short_name[8] = 0;

	i = 7;
	while (suffix_num && i >= 0) {
		short_name[i] = (suffix_num % 10) + '0';
		suffix_num /= 10;
		i--;
	}

	if (i >= 0)
		short_name[i--] = '~';

	while (i >= tilde)
		short_name[i--] = '~';
}

int ufat_short_exists(struct ufat_directory *dir, const char *short_name,
		      const char *short_ext)
{
	struct ufat_dirent ent;

	ufat_dir_rewind(dir);
	for (;;) {
		int err = ufat_dir_read(dir, &ent, NULL, 0);

		if (err < 0)
			return err;

		if (err)
			break;

		if (ufat_compare_name(short_name, ent.short_name, 0) >= 0 &&
		    ufat_compare_name(short_ext, ent.short_ext, 0) >= 0)
			return 1;
	}

	return 0;
}

void ufat_lfn_parse(struct ufat_lfn_parser *s, const uint8_t *data,
		    ufat_block_t blk, unsigned int pos)
{
	const int fr_seq = data[0];
	uint16_t frag_data[13];
	int fr_pos;
	int fr_len;
	int i;

	fr_pos = ((fr_seq & 0x3f) - 1) * 13;
	fr_len = 13;
	if (fr_pos + fr_len > UFAT_LFN_MAX_CHARS)
		fr_len = UFAT_LFN_MAX_CHARS - fr_pos;

	/* Check against expected sequence number and checksum */
	if (fr_seq & 0x40) {
		s->start_block = blk;
		s->start_pos = pos;
		s->seq = fr_seq & 0x3f;
		s->len = fr_pos + fr_len;
		s->short_checksum = data[0x0d];
	} else if (fr_seq != s->seq ||
		   s->short_checksum != data[0x0d]) {
		ufat_lfn_reset(s);
		return;
	}
	s->seq--;

	/* Unpack and copy fragment */
	for (i = 0; i < 5; i++)
		frag_data[i] = r16(data + 0x01 + i * 2);
	for (i = 0; i < 6; i++)
		frag_data[i + 5] = r16(data + 0x0e + i * 2);
	frag_data[11] = r16(data + 0x1c);
	frag_data[12] = r16(data + 0x1e);

	for (i = 0; i < fr_len; i++)
		s->buf[fr_pos + i] = frag_data[i];
}

int ufat_ucs2_to_utf8(const uint16_t *src, int src_len,
		      char *dst, int dst_len)
{
	int i;
	int j = 0;

	for (i = 0; i < src_len && src[i]; i++) {
		uint16_t c = src[i];

		if (c >= 0x800) {
			if (j + 3 >= dst_len)
				return -UFAT_ERR_BAD_ENCODING;

			dst[j++] = 0xe0 | (c >> 12);
			dst[j++] = 0x80 | ((c >> 6) & 0x3f);
			dst[j++] = 0x80 | (c & 0x3f);
		} else if (c >= 0x80) {
			if (j + 2 >= dst_len)
				return -UFAT_ERR_BAD_ENCODING;

			dst[j++] = 0xc0 | c >> 6;
			dst[j++] = 0x80 | (c & 0x3f);
		} else {
			if (j + 1 >= dst_len)
				return -UFAT_ERR_BAD_ENCODING;

			dst[j++] = c;
		}
	}

	if (j >= dst_len)
		return -UFAT_ERR_NAME_TOO_LONG;

	dst[j] = 0;
	return 0;
}

static inline uint8_t cksum_next(uint8_t cur, char c)
{
	return (((cur & 1) << 7) | (cur >> 1)) + c;
}

uint8_t ufat_short_checksum(const char *short_name, const char *short_ext)
{
	uint8_t sum = 0;
	int i;

	for (i = 0; short_name[i]; i++)
		sum = cksum_next(sum, short_name[i]);
	for (; i < 8; i++)
		sum = cksum_next(sum, ' ');

	for (i = 0; short_ext[i]; i++)
		sum = cksum_next(sum, short_ext[i]);
	for (; i < 3; i++)
		sum = cksum_next(sum, ' ');

	return sum;
}

int ufat_utf8_to_ucs2(const char *src, uint16_t *dst)
{
	int len = 0;
	int i;

	while (*src && len < UFAT_LFN_MAX_CHARS) {
		unsigned int c = *src;

		if ((c & 0xf0) == 0xf0)
			return -UFAT_ERR_ILLEGAL_NAME;

		if ((c & 0xe0) == 0xe0) {
			unsigned int b = src[1];
			unsigned int a = src[2];

			if ((a & 0xc0) != 0xc0 ||
			    (b & 0xc0) != 0xc0)
				return -UFAT_ERR_BAD_ENCODING;

			dst[len++] = ((c & 0xf) << 12) |
				((b & 0x3f) << 6) |
				(a & 0x3f);
			src += 3;
		} else if ((c & 0xc0) == 0xc0) {
			unsigned int b = src[1];

			if ((b & 0xc0) != 0xc0)
				return -UFAT_ERR_BAD_ENCODING;

			dst[len++] = ((c & 0x1f) << 6) | (b & 0x3f);
			src += 2;
		} else if (c & 0x80) {
			return -UFAT_ERR_BAD_ENCODING;
		} else {
			dst[len++] = c;
			src++;
		}
	}

	if (*src)
		return -UFAT_ERR_NAME_TOO_LONG;

	/* Pad out to full length */
	for (i = len; i < UFAT_LFN_MAX_CHARS; i++)
		dst[i] = 0;

	return len;
}

void ufat_lfn_pack_fragment(const uint16_t *ucs, int seq, int is_first,
			    uint8_t *data, uint8_t checksum)
{
	int i;

	data[0] = seq;
	if (is_first)
		data[0] |= 0x40;

	for (i = 0; i < 5; i++)
		w16(data + 0x01 + i * 2, ucs[i]);

	data[0x0b] = UFAT_ATTR_LFN_FRAGMENT;
	data[0x0c] = 0;
	data[0x0d] = checksum;

	for (i = 0; i < 6; i++)
		w16(data + 0x0e + i * 2, ucs[i + 5]);

	data[0x1a] = 0;
	data[0x1b] = 0;

	w16(data + 0x1c, ucs[11]);
	w16(data + 0x1e, ucs[12]);
}

int ufat_format_short(const char *name, const char *ext,
		      char *out, int max_len)
{
	int i = 0;

	while (*name) {
		if (i >= max_len)
			return -UFAT_ERR_NAME_TOO_LONG;

		out[i++] = *(name++);
	}

	if (*ext) {
		if (i >= max_len)
			return -UFAT_ERR_NAME_TOO_LONG;
		out[i++] = '.';

		while (*ext) {
			if (i >= max_len)
				return -UFAT_ERR_NAME_TOO_LONG;
			out[i++] = *(ext++);
		}
	}

	if (i >= max_len)
		return -UFAT_ERR_NAME_TOO_LONG;

	out[i] = 0;
	return 0;
}

int ufat_compare_name(const char *search_name, const char *dir_name,
		      int component_only)
{
	int m = 0;

	while (search_name[m]) {
		int dc = dir_name[m];
		int sc = search_name[m];

		if (component_only && (sc == '/' || sc == '\\'))
			break;

		if (!dc)
			return -1;

		if (sc >= 'a' && sc <= 'z')
			sc -= 32;

		if (dc >= 'a' && dc <= 'z')
			dc -= 32;

		if (sc != dc)
			return -1;

		m++;
	}

	if (dir_name[m])
		return -1;

	return m;
}

int ufat_update_attributes(struct ufat *uf, struct ufat_dirent *ent)
{
	int idx = ufat_cache_open(uf, ent->dirent_block);
	uint8_t *data;
	struct ufat_dirent old;

	if (idx < 0)
		return idx;

	data = ufat_cache_data(uf, idx) + ent->dirent_pos * UFAT_DIRENT_SIZE;
	ufat_parse_dirent(uf->bpb.type, data, &old);

	if (!(old.attributes & UFAT_ATTR_DIRECTORY))
		old.attributes = (old.attributes & ~UFAT_ATTR_USER) |
			(ent->attributes & UFAT_ATTR_USER);

	old.create_date = ent->create_date;
	old.create_time = ent->create_time;
	old.modify_date = ent->modify_date;
	old.modify_time = ent->modify_time;
	old.access_date = ent->access_date;

	ufat_cache_write(uf, idx);
	ufat_pack_dirent(&old, data);

	return 0;
}
