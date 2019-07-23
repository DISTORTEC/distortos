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
#include <stdlib.h>
#include "ufat.h"
#include "ufat_internal.h"

void ufat_open_root(struct ufat *uf, struct ufat_directory *dir)
{
	if (uf->bpb.root_cluster)
		dir->start = cluster_to_block(&uf->bpb, uf->bpb.root_cluster);
	else
		dir->start = uf->bpb.root_start;

	dir->uf = uf;
	dir->cur_block = dir->start;
	dir->cur_pos = 0;
}

int ufat_open_subdir(struct ufat *uf, struct ufat_directory *dir,
		     const struct ufat_dirent *ent)
{
	if (!(ent->attributes & UFAT_ATTR_DIRECTORY))
		return -UFAT_ERR_NOT_DIRECTORY;

	if (ent->dirent_block == UFAT_BLOCK_NONE || !ent->first_cluster) {
		ufat_open_root(uf, dir);
		return 0;
	}

	dir->uf = uf;
	dir->start = cluster_to_block(&uf->bpb, ent->first_cluster);
	dir->cur_block = dir->start;
	dir->cur_pos = 0;

	return 0;
}

void ufat_dir_rewind(struct ufat_directory *dir)
{
	dir->cur_block = dir->start;
	dir->cur_pos = 0;
}

static int format_name(const struct ufat_lfn_parser *lfn,
		       const struct ufat_dirent *inf,
		       char *lfn_buf, int max_len)
{
	if (ufat_lfn_ok(lfn))
		return ufat_ucs2_to_utf8(lfn->buf, lfn->len,
					 lfn_buf, max_len);

	return ufat_format_short(inf->short_name,
				 inf->short_ext,
				 lfn_buf, max_len);
}

int ufat_dir_read(struct ufat_directory *dir, struct ufat_dirent *inf,
		  char *lfn_buf, int max_len)
{
	struct ufat_lfn_parser lfn;

	ufat_lfn_reset(&lfn);

	for (;;) {
		uint8_t data[UFAT_DIRENT_SIZE];
		int err;

		inf->dirent_block = dir->cur_block;
		inf->dirent_pos = dir->cur_pos;

		err = ufat_read_raw_dirent(dir, data);
		if (err)
			return err;

		err = ufat_advance_raw_dirent(dir, 0);
		if (err)
			return err;

		if (data[0x0b] == 0x0f && data[0] != 0xe5) {
			ufat_lfn_parse(&lfn, data,
				       inf->dirent_block, inf->dirent_pos);
		} else if (data[0] && data[0] != 0xe5) {
			ufat_parse_dirent(dir->uf->bpb.type, data, inf);

			if (ufat_short_checksum(inf->short_name,
						inf->short_ext) !=
			    lfn.short_checksum)
				ufat_lfn_reset(&lfn);

			if (inf->attributes & 0x08) {
				ufat_lfn_reset(&lfn);
				continue;
			}

			if (ufat_lfn_ok(&lfn)) {
				inf->lfn_block = lfn.start_block;
				inf->lfn_pos = lfn.start_pos;
			} else {
				inf->lfn_block = UFAT_BLOCK_NONE;
				inf->lfn_pos = 0;
			}

			if (lfn_buf)
				return format_name(&lfn, inf, lfn_buf, max_len);

			return 0;
		} else {
			ufat_lfn_reset(&lfn);
		}
	}

	return 0;
}

static int verify_empty_dir(struct ufat *uf, struct ufat_dirent *ent)
{
	struct ufat_directory dir;
	int err;

	err = ufat_open_subdir(uf, &dir, ent);
	if (err < 0)
		return err;

	for (;;) {
		struct ufat_dirent e;

		err = ufat_dir_read(&dir, &e, NULL, 0);
		if (err < 0)
			return err;

		if (err)
			break;

		if (e.short_name[0] != '.')
			return -UFAT_ERR_DIRECTORY_NOT_EMPTY;
	}

	return 0;
}

static int delete_entry(struct ufat *uf, struct ufat_dirent *ent)
{
	static const uint8_t del_marker = 0xe5;
	struct ufat_directory dir;

	dir.uf = uf;

	if (ent->lfn_block == UFAT_BLOCK_NONE) {
		dir.cur_block = ent->dirent_block;
		dir.cur_pos = ent->dirent_pos;
	} else {
		dir.cur_block = ent->lfn_block;
		dir.cur_pos = ent->lfn_pos;
	}

	for (;;) {
		int err;

		err = ufat_write_raw_dirent(&dir, &del_marker, 1);
		if (err < 0)
			return err;

		if (dir.cur_block == ent->dirent_block &&
		    dir.cur_pos == ent->dirent_pos)
			break;

		err = ufat_advance_raw_dirent(&dir, 0);
		if (err < 0)
			return err;
	}

	return 0;
}

int ufat_dir_delete(struct ufat *uf, struct ufat_dirent *ent)
{
	int err;

	if (ent->dirent_block == UFAT_BLOCK_NONE || ent->short_name[0] == '.')
		return -UFAT_ERR_IMMUTABLE;

	if (ent->attributes & UFAT_ATTR_DIRECTORY) {
		err = verify_empty_dir(uf, ent);
		if (err < 0)
			return err;
	}

	err = delete_entry(uf, ent);
	if (err < 0)
		return err;

	return ufat_free_chain(uf, ent->first_cluster);
}

/* Create a new empty subdirectory and return it. */
static int create_empty_dir(struct ufat_directory *parent,
			    ufat_cluster_t *out,
			    const struct ufat_dirent *downptr)
{
	struct ufat_dirent ent;
	ufat_cluster_t c;
	int err = ufat_alloc_chain(parent->uf, 1, &c);
	int idx;

	if (err < 0)
		return err;

	/* Clear all entries */
	err = ufat_init_dirent_cluster(parent->uf, c);
	if (err < 0) {
		ufat_free_chain(parent->uf, c);
		return err;
	}

	/* Get the first block of the dirent */
	idx = ufat_cache_open(parent->uf,
			      cluster_to_block(&parent->uf->bpb, c));
	if (idx < 0) {
		ufat_free_chain(parent->uf, c);
		return idx;
	}

	ufat_cache_write(parent->uf, idx);

	/* Create "." */
	memcpy(&ent, downptr, sizeof(ent));
	ent.short_name[0] = '.';
	ent.short_name[1] = 0;
	ent.short_ext[0] = 0;
	ent.attributes = UFAT_ATTR_DIRECTORY;
	ent.first_cluster = c;
	ent.file_size = 0;
	ufat_pack_dirent(&ent, ufat_cache_data(parent->uf, idx));

	/* Create ".." */
	ent.short_name[1] = '.';
	ent.short_name[2] = 0;

	if (parent->start >= parent->uf->bpb.cluster_start)
		ent.first_cluster = block_to_cluster(&parent->uf->bpb,
						     parent->start);
	else
		ent.first_cluster = 0;

	ufat_pack_dirent(&ent,
			 ufat_cache_data(parent->uf, idx) + UFAT_DIRENT_SIZE);

	*out = c;
	return 0;
}

static int insert_dirent(struct ufat_directory *dir, struct ufat_dirent *ent,
			 const char *long_name)
{
	uint16_t ucs2_name[UFAT_LFN_MAX_CHARS];
	uint8_t data[UFAT_DIRENT_SIZE];
	uint8_t checksum;
	int ucs2_len = ufat_utf8_to_ucs2(long_name, ucs2_name);
	int num_lfn_frags;
	int err;
	int i;

	/* Check that the UTF8 was encoded correctly */
	if (ucs2_len < 0)
		return ucs2_len;
	num_lfn_frags = (ucs2_len + 12) / 13;

	/* Choose a suitable short-name */
	ufat_short_first(long_name, ent->short_name, ent->short_ext);
	for (;;) {
		err = ufat_short_exists(dir, ent->short_name, ent->short_ext);
		if (err < 0)
			return err;

		if (!err)
			break;

		ufat_short_next(ent->short_name);
	}

	checksum = ufat_short_checksum(ent->short_name, ent->short_ext);

	/* Find a space in the directory */
	err = ufat_allocate_raw_dirent(dir, num_lfn_frags + 1);
	if (err < 0)
		return err;

	ent->lfn_block = dir->cur_block;
	ent->lfn_pos = dir->cur_pos;

	/* Write LFN fragments and the DOS dirent */
	for (i = 0; i < num_lfn_frags; i++) {
		ufat_lfn_pack_fragment(ucs2_name + (num_lfn_frags - i - 1) * 13,
				       num_lfn_frags - i, !i,
				       data, checksum);

		err = ufat_write_raw_dirent(dir, data, sizeof(data));
		if (err < 0)
			return err;

		err = ufat_advance_raw_dirent(dir, 0);
		if (err < 0)
			return err;
	}

	ufat_pack_dirent(ent, data);
	ent->dirent_block = dir->cur_block;
	ent->dirent_pos = dir->cur_pos;

	return ufat_write_raw_dirent(dir, data, sizeof(data));
}

int ufat_dir_create(struct ufat_directory *dir, struct ufat_dirent *ent,
		    const char *name)
{
	struct ufat_dirent check;
	int err;

	if (!ufat_lfn_is_legal(name))
		return -UFAT_ERR_ILLEGAL_NAME;

	if (!ufat_dir_find(dir, name, &check))
		return -UFAT_ERR_FILE_EXISTS;

	err = create_empty_dir(dir, &ent->first_cluster, ent);
	if (err < 0)
		return err;

	ent->file_size = 0;
	ent->attributes = (ent->attributes & UFAT_ATTR_USER) |
		UFAT_ATTR_DIRECTORY;

	err = insert_dirent(dir, ent, name);
	if (err < 0) {
		ufat_free_chain(dir->uf, ent->first_cluster);
		return err;
	}

	return 0;
}

int ufat_dir_mkfile(struct ufat_directory *dir, struct ufat_dirent *ent,
		    const char *name)
{
	struct ufat_dirent check;
	int err;

	if (!ufat_lfn_is_legal(name))
		return -UFAT_ERR_ILLEGAL_NAME;

	if (!ufat_dir_find(dir, name, &check))
		return -UFAT_ERR_FILE_EXISTS;

	ent->file_size = 0;
	ent->first_cluster = 0;
	ent->attributes &= UFAT_ATTR_USER;

	err = insert_dirent(dir, ent, name);
	if (err < 0)
		return err;

	return 0;
}

int ufat_dir_find(struct ufat_directory *dir,
		  const char *target, struct ufat_dirent *inf)
{
	ufat_dir_rewind(dir);

	for (;;) {
		char name[UFAT_LFN_MAX_UTF8];
		int err = ufat_dir_read(dir, inf, name, sizeof(name));

		if (err)
			return err;

		if (ufat_compare_name(target, name, 0) >= 0)
			break;
	}

	return 0;
}


int ufat_dir_find_path(struct ufat_directory *dir,
		       const char *path, struct ufat_dirent *ent,
		       const char **path_out)
{
	int at_root = 1;

	ufat_dir_rewind(dir);

	while (*path) {
		int len = 0;

		/* Ignore blank components */
		if (*path == '/' || *path == '\\') {
			path++;
			continue;
		}

		/* Descend if necessary */
		if (!at_root) {
			int err = ufat_open_subdir(dir->uf, dir, ent);

			if (err < 0)
				return err;
		}

		/* Search for this component */
		for (;;) {
			char name[UFAT_LFN_MAX_UTF8];
			int err = ufat_dir_read(dir, ent,
						name, sizeof(name));

			if (err < 0)
				return err;

			if (err) {
				if (path_out)
					*path_out = path;
				return 1;
			}

			len = ufat_compare_name(path, name, 1);
			if (len >= 0)
				break;
		}

		/* Skip over this component */
		path += len;
		if (*path)
			path++;
		at_root = 0;
	}

	if (at_root) {
		/* Pseudo-dirent for root directory */
		memset(ent, 0, sizeof(*ent));
		ent->dirent_block = UFAT_BLOCK_NONE;
		ent->attributes = UFAT_ATTR_DIRECTORY;
	}

	return 0;
}

int ufat_get_filename(struct ufat *uf,
		      const struct ufat_dirent *ent,
		      char *name_buf, int max_len)
{
	struct ufat_lfn_parser lfn;

	ufat_lfn_reset(&lfn);

	if (ent->lfn_block != UFAT_BLOCK_NONE) {
		struct ufat_directory dir;

		dir.uf = uf;
		dir.cur_block = ent->lfn_block;
		dir.cur_pos = ent->lfn_pos;

		while (!(dir.cur_block == ent->dirent_block &&
			 dir.cur_pos == ent->dirent_pos) &&
		       dir.cur_block != UFAT_BLOCK_NONE) {
			uint8_t data[UFAT_DIRENT_SIZE];
			int err;

			err = ufat_read_raw_dirent(&dir, data);
			if (err < 0)
				return err;

			err = ufat_advance_raw_dirent(&dir, 0);
			if (err < 0)
				return err;

			ufat_lfn_parse(&lfn, data,
				       dir.cur_block, dir.cur_pos);
		}
	}

	return format_name(&lfn, ent, name_buf, max_len);
}

int ufat_move(struct ufat_dirent *ent, struct ufat_directory *dst,
	      const char *new_name)
{
	struct ufat_dirent new_ent;
	int err;

	if (!ufat_lfn_is_legal(new_name))
		return -UFAT_ERR_ILLEGAL_NAME;

	if (!ufat_dir_find(dst, new_name, &new_ent))
		return -UFAT_ERR_FILE_EXISTS;

	if (ent->short_name[0] == '.' || ent->dirent_block == UFAT_BLOCK_NONE)
		return -UFAT_ERR_IMMUTABLE;

	memcpy(&new_ent, ent, sizeof(new_ent));
	err = insert_dirent(dst, &new_ent, new_name);
	if (err < 0)
		return err;

	err = delete_entry(dst->uf, ent);
	if (err < 0) {
		delete_entry(dst->uf, &new_ent);
		return err;
	}

	return 0;
}
