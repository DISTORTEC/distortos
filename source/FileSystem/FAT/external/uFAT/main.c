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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include "ufat.h"

struct command;

#define OPTION_STATISTICS	0x01
#define OPTION_RANDOMIZE	0x02
#define OPTION_MKFS		0x04

struct options {
	int			flags;
	unsigned int		log2_bs;
	unsigned int		seed;
	ufat_block_t		num_blocks;

	const char		*in_file;
	const char		*out_file;

	const char		*filename;
	const struct command	*command;
	char			**argv;
	int			argc;
};

struct command {
	const char		*name;
	int			(*func)(struct ufat *uf,
					const struct options *opt);
};

struct file_device {
	struct ufat_device	base;
	FILE			*f;
	int			is_read_only;
};

static int file_device_read(const struct ufat_device *dev, ufat_block_t start,
			    ufat_block_t count, void *buffer)
{
	struct file_device *f = (struct file_device *)dev;

	if (fseek(f->f, start << f->base.log2_block_size, SEEK_SET) < 0) {
		perror("file_device_read: fseek");
		return -1;
	}

	if (fread(buffer, 1 << f->base.log2_block_size,
		  count, f->f) != count) {
		if (feof(f->f)) {
			memset(buffer, 0xcc,
			       (1 << f->base.log2_block_size) * count);
		} else {
			perror("file_device_read: fread");
			return -1;
		}
	}

	return 0;
}

static int file_device_write(const struct ufat_device *dev, ufat_block_t start,
			     ufat_block_t count, const void *buffer)
{
	struct file_device *f = (struct file_device *)dev;

	if (f->is_read_only) {
		fprintf(stderr, "file_device_write: read-only device\n");
		return -1;
	}

	if (fseek(f->f, start << f->base.log2_block_size, SEEK_SET) < 0) {
		perror("file_device_write: fseek");
		return -1;
	}

	if (fwrite(buffer, 1 << f->base.log2_block_size,
		   count, f->f) != count) {
		perror("file_device_write: fwrite");
		return -1;
	}

	return 0;
}

static int file_device_open(struct file_device *dev, const char *fname,
			    unsigned int log2_bs, int create)
{
	dev->base.log2_block_size = log2_bs;
	dev->base.read = file_device_read;
	dev->base.write = file_device_write;
	dev->is_read_only = 0;

	if (create) {
		dev->f = fopen(fname, "wb+");
	} else {
		dev->f = fopen(fname, "rb+");
		if (!dev->f && errno == EACCES) {
			dev->is_read_only = 1;
			dev->f = fopen(fname, "rb");
		}
	}

	if (!dev->f) {
		perror("open");
		return -1;
	}

	return 0;
}

static void file_device_close(struct file_device *dev)
{
	fclose(dev->f);
}

static FILE *open_input(const char *fname)
{
	FILE *in;

	if (!fname)
		return stdin;

	in = fopen(fname, "rb");
	if (!in) {
		fprintf(stderr, "Failed to open %s for reading: %s\n",
			fname, strerror(errno));
		return NULL;
	}

	return in;
}

static void close_input(const char *fname, FILE *f)
{
	if (fname)
		fclose(f);
}

static FILE *open_output(const char *fname)
{
	FILE *out;

	if (!fname)
		return stdout;

	out = fopen(fname, "wb");
	if (!out) {
		fprintf(stderr, "Failed to open %s for writing: %s\n",
			fname, strerror(errno));
		return NULL;
	}

	return out;
}

static int close_output(const char *fname, FILE *f)
{
	if (fname && fclose(f) < 0) {
		fprintf(stderr, "Error on closing %s: %s\n",
			fname, strerror(errno));
		return -1;
	}

	return 0;
}

static void print_date(FILE *out, ufat_date_t d)
{
	fprintf(out, "%04d-%02d-%02d",
		UFAT_DATE_Y(d), UFAT_DATE_M(d), UFAT_DATE_D(d));
}

static void print_time(FILE *out, ufat_time_t t)
{
	fprintf(out, "%2d:%02d:%02d",
		UFAT_TIME_H(t), UFAT_TIME_M(t), UFAT_TIME_S(t));
}

static void print_attributes(FILE *out, ufat_attr_t a)
{
	fprintf(out, "%c%c%c%c%c",
		(a & UFAT_ATTR_ARCHIVE) ? 'A' : ' ',
		(a & UFAT_ATTR_SYSTEM) ? 'S' : ' ',
		(a & UFAT_ATTR_HIDDEN) ? 'H' : ' ',
		(a & UFAT_ATTR_READONLY) ? 'R' : ' ',
		(a & UFAT_ATTR_DIRECTORY) ? 'D' : ' ');
}

static int list_dir(FILE *out, struct ufat_directory *dir)
{
	for (;;) {
		struct ufat_dirent inf;
		char lfn[UFAT_LFN_MAX_UTF8];
		int err;

		err = ufat_dir_read(dir, &inf, lfn, sizeof(lfn));
		if (err < 0) {
			fprintf(stderr, "list_dir: ufat_dir_read: %s\n",
				ufat_strerror(err));
			return err;
		}

		if (err)
			break;

		print_date(out, inf.modify_date);
		fprintf(out, " ");
		print_time(out, inf.modify_time);
		fprintf(out, " ");
		print_attributes(out, inf.attributes & ~UFAT_ATTR_DIRECTORY);
		if (inf.attributes & UFAT_ATTR_DIRECTORY)
			fprintf(out, " %9s", "<DIR>");
		else
			fprintf(out, " %9u", inf.file_size);
		fprintf(out, " %s\n", lfn);
	}

	return 0;
}

static int find_path(struct ufat *uf,
		     struct ufat_directory *dir, const char *path,
		     struct ufat_dirent *inf, const char **path_out)
{
	int err;

	ufat_open_root(uf, dir);
	err = ufat_dir_find_path(dir, path, inf, path_out);

	if (err < 0)
		fprintf(stderr, "ufat_dir_find_path: %s\n",
			ufat_strerror(err));

	return err;
}

static int require_file(struct ufat *uf, const char *path,
			struct ufat_dirent *ent)
{
	struct ufat_directory dir;
	int err = find_path(uf, &dir, path, ent, NULL);

	if (err < 0)
		return err;

	if (err) {
		fprintf(stderr, "No such file or directory: %s\n", path);
		return -1;
	}

	return 0;
}

static int cmd_dir(struct ufat *uf, const struct options *opt)
{
	struct ufat_directory dir;
	FILE *out;

	if (opt->argc) {
		struct ufat_dirent ent;
		int err;

		if (require_file(uf, opt->argv[0], &ent) < 0)
			return -1;

		err = ufat_open_subdir(uf, &dir, &ent);
		if (err < 0) {
			fprintf(stderr, "ufat_open_subdir: %s\n",
				ufat_strerror(err));
			return -1;
		}
	} else {
		ufat_open_root(uf, &dir);
	}

	out = open_output(opt->out_file);
	if (!out)
		return -1;

	if (list_dir(out, &dir) < 0) {
		close_output(opt->out_file, out);
		return -1;
	}

	return close_output(opt->out_file, out);
}

static int cmd_fstat(struct ufat *uf, const struct options *opt)
{
	FILE *out;
	struct ufat_dirent ent;
	char can_name[UFAT_LFN_MAX_UTF8];
	int err;

	if (!opt->argc) {
		fprintf(stderr, "You must specify a file path\n");
		return -1;
	}

	if (require_file(uf, opt->argv[0], &ent) < 0)
		return -1;

	out = open_output(opt->out_file);
	if (!out)
		return -1;

	err = ufat_get_filename(uf, &ent, can_name, sizeof(can_name));
	if (err < 0)
		fprintf(stderr, "ufat_get_filename: %s\n",
			ufat_strerror(err));
	else
		fprintf(out, "Filename:               %s\n", can_name);

	fprintf(out, "Entry block/offset:     %llu/%d\n",
		ent.dirent_block, ent.dirent_pos);

	if (ent.lfn_block != UFAT_BLOCK_NONE)
		fprintf(out, "LFN start block/offset: %llu/%d\n",
			ent.lfn_block, ent.lfn_pos);

	fprintf(out, "Short name:             %s", ent.short_name);
	if (ent.short_ext[0])
		fprintf(out, ".%s\n", ent.short_ext);
	else
		fprintf(out, "\n");
	fprintf(out, "Attributes:             0x%02x (", ent.attributes);
	print_attributes(out, ent.attributes);
	fprintf(out, ")\n");
	fprintf(out, "Creation date/time:     ");
	print_date(out, ent.create_date);
	fprintf(out, " ");
	print_time(out, ent.create_time);
	fprintf(out, "\n");
	fprintf(out, "Modification date/time: ");
	print_date(out, ent.modify_date);
	fprintf(out, " ");
	print_time(out, ent.modify_time);
	fprintf(out, "\n");
	fprintf(out, "Last access date:       ");
	print_date(out, ent.access_date);
	fprintf(out, "\n");
	fprintf(out, "First cluster:          %u\n", ent.first_cluster);
	fprintf(out, "Size:                   %u\n", ent.file_size);

	return close_output(opt->out_file, out);
}

static int cmd_read(struct ufat *uf, const struct options *opt)
{
	FILE *out;
	struct ufat_dirent ent;
	struct ufat_file file;
	int err;

	if (!opt->argc) {
		fprintf(stderr, "You must specify a file path\n");
		return -1;
	}

	if (require_file(uf, opt->argv[0], &ent) < 0)
		return -1;

	err = ufat_open_file(uf, &file, &ent);
	if (err < 0) {
		fprintf(stderr, "ufat_open_file: %s\n", ufat_strerror(err));
		return -1;
	}

	out = open_output(opt->out_file);
	if (!out)
		return -1;

	for (;;) {
		char buf[16384];
		int req_size = sizeof(buf);
		int len;

		if (opt->flags & OPTION_RANDOMIZE)
			req_size = random() % sizeof(buf) + 1;

		len = ufat_file_read(&file, buf, req_size);
		if (len < 0) {
			fprintf(stderr, "ufat_file_read: %s\n",
				ufat_strerror(len));
			close_output(opt->out_file, out);
			return -1;
		}

		if (!len)
			break;

		if (fwrite(buf, 1, len, out) != (size_t)len) {
			perror("fwrite");
			close_output(opt->out_file, out);
			return -1;
		}
	}

	return close_output(opt->out_file, out);
}

static int cmd_write(struct ufat *uf, const struct options *opt)
{
	FILE *in;
	const char *basename;
	struct ufat_directory dir;
	struct ufat_dirent ent;
	struct ufat_file file;
	int err;

	if (!opt->argc) {
		fprintf(stderr, "You must specify a file path\n");
		return -1;
	}

	err = find_path(uf, &dir, opt->argv[0], &ent, &basename);
	if (err < 0)
		return -1;

	in = open_input(opt->in_file);
	if (!in)
		return -1;

	if (err) {
		time_t now = time(NULL);
		struct tm *local = localtime(&now);

		ent.attributes = 0;
		ent.create_date = UFAT_DATE(local->tm_year + 1900,
					    local->tm_mon + 1,
					    local->tm_mday);
		ent.create_time = UFAT_TIME(local->tm_hour,
					    local->tm_min,
					    local->tm_sec);
		ent.modify_date = ent.create_date;
		ent.modify_time = ent.create_time;
		ent.access_date = ent.create_date;

		err = ufat_dir_mkfile(&dir, &ent, basename);
		if (err < 0) {
			fprintf(stderr, "Failed to create file: %s: %s\n",
				basename, ufat_strerror(err));
			close_input(opt->in_file, in);
			return -1;
		}
	}

	err = ufat_open_file(uf, &file, &ent);
	if (err < 0) {
		fprintf(stderr, "ufat_open_file: %s\n", ufat_strerror(err));
		close_input(opt->in_file, in);
		return -1;
	}

	for (;;) {
		char buf[16384];
		int req_size = sizeof(buf);
		int len;

		if (opt->flags & OPTION_RANDOMIZE)
			req_size = random() % sizeof(buf) + 1;

		len = fread(buf, 1, req_size, in);
		if (ferror(stdin)) {
			perror("fread");
			close_input(opt->in_file, in);
			return -1;
		}

		if (!len)
			break;

		len = ufat_file_write(&file, buf, len);
		if (len < 0) {
			fprintf(stderr, "ufat_file_write: %s\n",
				ufat_strerror(len));
			close_input(opt->in_file, in);
			return -1;
		}
	}

	err = ufat_file_truncate(&file);
	if (err < 0) {
		fprintf(stderr, "ufat_file_truncate: %s\n",
			ufat_strerror(err));
		close_input(opt->in_file, in);
		return -1;
	}

	close_input(opt->in_file, in);
	return 0;
}

static int cmd_rm(struct ufat *uf, const struct options *opt)
{
	struct ufat_dirent ent;
	int err;

	if (!opt->argc) {
		fprintf(stderr, "You must specify a file path\n");
		return -1;
	}

	if (require_file(uf, opt->argv[0], &ent) < 0)
		return -1;

	err = ufat_dir_delete(uf, &ent);
	if (err < 0) {
		fprintf(stderr, "ufat_dir_delete: %s\n", ufat_strerror(err));
		return -1;
	}

	return 0;
}

static int cmd_mkdir(struct ufat *uf, const struct options *opt)
{
	const char *basename;
	struct ufat_directory dir;
	struct ufat_dirent ent;
	struct tm *local;
	time_t now = time(NULL);
	int err;

	if (!opt->argc) {
		fprintf(stderr, "You must specify a file path\n");
		return -1;
	}

	err = find_path(uf, &dir, opt->argv[0], &ent, &basename);
	if (err < 0)
		return -1;

	if (!err) {
		fprintf(stderr, "Path already exists: %s\n",
			opt->argv[0]);
		return -1;
	}

	local = localtime(&now);
	ent.attributes = 0;
	ent.create_date = UFAT_DATE(local->tm_year + 1900, local->tm_mon + 1,
				    local->tm_mday);
	ent.create_time = UFAT_TIME(local->tm_hour, local->tm_min,
				    local->tm_sec);
	ent.modify_date = ent.create_date;
	ent.modify_time = ent.create_time;
	ent.access_date = ent.create_date;

	err = ufat_dir_create(&dir, &ent, basename);
	if (err < 0) {
		fprintf(stderr, "ufat_dir_create: %s\n", ufat_strerror(err));
		return -1;
	}

	return 0;
}

static int parse_dosattr(const char *attr, ufat_attr_t *a)
{
	ufat_attr_t out = 0;

	while (*attr) {
		switch (*attr) {
		case 'A':
		case 'a':
			out |= UFAT_ATTR_ARCHIVE;
			break;

		case 'H':
		case 'h':
			out |= UFAT_ATTR_HIDDEN;
			break;

		case 'R':
		case 'r':
			out |= UFAT_ATTR_READONLY;
			break;

		case 'S':
		case 's':
			out |= UFAT_ATTR_SYSTEM;
			break;

		default:
			fprintf(stderr, "Unrecognized DOS attribute: %c\n",
				*attr);
			return -1;
		}

		attr++;
	}

	*a = out;
	return 0;
}

static int parse_date(const char *text, ufat_date_t *d)
{
	int bits[3] = {0};
	int b = 0;
	int i;

	if (!strcasecmp(text, "now")) {
		time_t now = time(NULL);
		struct tm *local = localtime(&now);

		*d = UFAT_DATE(local->tm_year + 1900,
			       local->tm_mon + 1,
			       local->tm_mday);
		return 0;
	}

	for (i = 0; text[i]; i++) {
		const int c = text[i];

		if (c == '-') {
			b++;
			if (b >= 3)
				goto bad_format;
		} else if (isdigit(c)) {
			bits[b] = bits[b] * 10 + c - '0';
		} else {
			goto bad_format;
		}
	}

	if (b < 2)
		goto bad_format;

	*d = UFAT_DATE(bits[0], bits[1], bits[2]);
	return 0;

 bad_format:
	fprintf(stderr, "Date must be of the form YYYY-MM-DD: %s\n", text);
	return -1;
}

static int parse_time(const char *text, ufat_time_t *t)
{
	int bits[3] = {0};
	int b = 0;
	int i;

	if (!strcasecmp(text, "now")) {
		time_t now = time(NULL);
		struct tm *local = localtime(&now);

		*t = UFAT_TIME(local->tm_hour,
			       local->tm_min,
			       local->tm_sec);
		return 0;
	}

	for (i = 0; text[i]; i++) {
		const int c = text[i];

		if (c == ':') {
			b++;
			if (b >= 3)
				goto bad_format;
		} else if (isdigit(c)) {
			bits[b] = bits[b] * 10 + c - '0';
		} else {
			goto bad_format;
		}
	}

	if (b < 2)
		goto bad_format;

	*t = UFAT_TIME(bits[0], bits[1], bits[2]);
	return 0;

 bad_format:
	fprintf(stderr, "Time must be of the form HH:MM:SS: %s\n",
		text);
	return -1;
}

static int parse_attribute(const char *spec, struct ufat_dirent *ent)
{
	char key[64];
	char *value;

	if (*spec == '+' || *spec == '~' || *spec == '=') {
		ufat_attr_t a;

		if (parse_dosattr(spec + 1, &a) < 0)
			return -1;

		if (*spec == '+')
			ent->attributes |= a;
		else if (*spec == '=')
			ent->attributes = a;
		else
			ent->attributes &= ~a;

		return 0;
	}

	/* Try to identify a key=value pair */
	strncpy(key, spec, sizeof(key));
	key[sizeof(key) - 1] = 0;

	value = strchr(key, '=');
	if (!value) {
		fprintf(stderr, "Unknown attribute specification: %s\n",
			spec);
		return -1;
	}

	*(value++) = 0;

	/* Deal with it */
	if (!strcasecmp(key, "create_date"))
		return parse_date(value, &ent->create_date);
	if (!strcasecmp(key, "create_time"))
		return parse_time(value, &ent->create_time);
	if (!strcasecmp(key, "modify_date"))
		return parse_date(value, &ent->modify_date);
	if (!strcasecmp(key, "modify_time"))
		return parse_time(value, &ent->modify_time);
	if (!strcasecmp(key, "access_date"))
		return parse_date(value, &ent->access_date);

	fprintf(stderr, "Unknown attribute: %s\n", key);
	return -1;
}

static int cmd_rename(struct ufat *uf, const struct options *opt)
{
	struct ufat_directory dir;
	struct ufat_dirent ent;
	int err;
	const char *src_path;
	const char *dst_name;

	if (opt->argc < 2) {
		fprintf(stderr, "You must specified a source path and "
			"a new name.\n");
		return -1;
	}

	src_path = opt->argv[0];
	dst_name = opt->argv[1];

	err = find_path(uf, &dir, src_path, &ent, NULL);
	if (err < 0)
		return -1;

	if (err) {
		fprintf(stderr, "No such file or directory: %s\n",
			ufat_strerror(err));
		return -1;
	}

	err = ufat_move(&ent, &dir, dst_name);
	if (err < 0) {
		fprintf(stderr, "ufat_move: %s\n", ufat_strerror(err));
		return -1;
	}

	return 0;
}

static int cmd_move(struct ufat *uf, const struct options *opt)
{
	struct ufat_directory dir;
	struct ufat_dirent src_ent;
	struct ufat_dirent dst_ent;
	int err;
	const char *src_path;
	const char *dst_path;
	const char *new_name;
	char old_name[UFAT_LFN_MAX_UTF8];

	if (opt->argc < 2) {
		fprintf(stderr, "You must specified a source path and "
			"a destination path.\n");
		return -1;
	}

	src_path = opt->argv[0];
	dst_path = opt->argv[1];

	err = find_path(uf, &dir, src_path, &src_ent, NULL);
	if (err < 0)
		return -1;

	if (err) {
		fprintf(stderr, "No such file or directory: %s\n",
			opt->argv[0]);
		return -1;
	}

	ufat_open_root(uf, &dir);
	err = ufat_dir_find_path(&dir, dst_path, &dst_ent, &new_name);

	if (err < 0) {
		fprintf(stderr, "ufat_dir_find_path: %s\n",
			ufat_strerror(err));
		return -1;
	}

	if (!err) {
		/* Is the destination a directory or an existing file? */
		if (!(dst_ent.attributes & UFAT_ATTR_DIRECTORY)) {
			fprintf(stderr, "File already exists: %s\n",
				dst_path);
			return -1;
		}

		/* It's a directory name. Use the old canonical filename. */
		err = ufat_open_subdir(uf, &dir, &dst_ent);
		if (err < 0) {
			fprintf(stderr, "ufat_open_subdir: %s\n",
				ufat_strerror(err));
			return -1;
		}

		err = ufat_get_filename(uf, &src_ent,
					old_name, sizeof(old_name));
		if (err < 0) {
			fprintf(stderr, "ufat_get_filename: %s\n",
				ufat_strerror(err));
			return -1;
		}

		new_name = old_name;
	}

	err = ufat_move(&src_ent, &dir, new_name);
	if (err < 0) {
		fprintf(stderr, "ufat_move: %s\n", ufat_strerror(err));
		return -1;
	}

	return 0;
}

static int cmd_chattr(struct ufat *uf, const struct options *opt)
{
	struct ufat_dirent ent;
	int err;
	int i;

	if (!opt->argc) {
		fprintf(stderr, "You must specify a file path\n");
		return -1;
	}

	if (require_file(uf, opt->argv[0], &ent) < 0)
		return -1;

	for (i = 1; i < opt->argc; i++)
		if (parse_attribute(opt->argv[i], &ent) < 0)
			return -1;

	err = ufat_update_attributes(uf, &ent);
	if (err < 0) {
		fprintf(stderr, "ufat_dir_update_attributes: %s\n",
			ufat_strerror(err));
		return -1;
	}

	return 0;
}

static void show_info(FILE *out, const struct ufat_bpb *bpb)
{
	fprintf(out, "Type:                       FAT%d\n", bpb->type);
	fprintf(out, "Blocks per cluster:         %u\n",
		1 << bpb->log2_blocks_per_cluster);
	fprintf(out, "FAT size (blocks):          %llu\n", bpb->fat_size);
	fprintf(out, "FAT offset (block):         %llu\n", bpb->fat_start);
	fprintf(out, "FAT count:                  %u\n", bpb->fat_count);
	fprintf(out, "Cluster starting block:     %llu\n", bpb->cluster_start);
	fprintf(out, "Clusters:                   %u\n", bpb->num_clusters);
	fprintf(out, "Root directory block start: %llu\n", bpb->root_start);
	fprintf(out, "Root directory block count: %llu\n", bpb->root_size);
	fprintf(out, "Root directory cluster:     %u\n", bpb->root_cluster);
}

static void usage(const char *progname)
{
	printf(
"Usage: %s [options] <image file> [command [args]]\n"
"\n"
"Options may be any of the following:\n"
"  -b block-size           Set the simulated block size\n"
"  -S                      Show performance statistics\n"
"  -R seed                 Randomize file IO request sizes\n"
"  -i filename             Read input from the given file\n"
"  -o filename             Write output to the given file\n"
"  --mkfs <num blocks>     Initialize the filesystem (WARNING: all existing\n"
"                          data will be lost)\n"
"  --help                  Show this text\n"
"  --version               Show version information\n"
"\n"
"With no command, basic information is printed. Available commands are:\n"
"  dir [directory]         Show a directory listing\n"
"  fstat [path]            Show directory entry details\n"
"  read [file]             Dump the contents of the given file\n"
"  write [file]            Write to a file\n"
"  rm [path]               Remove a directory or file\n"
"  mkdir [directory]       Create a new empty directory\n"
"  chattr [path] [attributes]\n"
"                          Alter file attributes/dates/times (see below)\n"
"  move [src] [dst]        Move a file from one place to another\n"
"  rename [src] [new-name] Rename a file without moving it\n"
"\n"
"Attributes are specified using arguments with a key=value syntax:\n"
"  create_date=YYYY-MM-DD  Creation date\n"
"  create_time=HH:MM:SS    Creation time\n"
"  modify_date=YYYY-MM-DD  Modification date\n"
"  modify_time=HH:MM:SS    Modification time\n"
"  access_date=YYYY-MM-DD  Access date\n"
"  [+/~/=]AHRS             Alter archive/hidden/read-only/system\n"
"\n"
"The special value \"now\" can be used for either dates or times.\n",
progname);
}

static void version_banner(void)
{
	printf(
"ufat version 20120528\n"
"Copyright (C) 2012 TracMap Holdings Ltd\n"
"This is free software; see the source for copying conditions. There is NO\n"
"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
	       );
}

static int parse_blocksize(const char *arg, unsigned int *out)
{
	unsigned int target = atoi(arg);
	unsigned int c = 0;

	if (!target) {
		fprintf(stderr, "Block size must be greater than 0\n");
		return -1;
	}

	while (target > 1) {
		if (target & 1) {
			fprintf(stderr, "Block size must be a power of 2\n");
			return -1;
		}

		target >>= 1;
		c++;
	}

	*out = c;
	return 0;
}

static const struct command command_table[] = {
	{"dir",		cmd_dir},
	{"fstat",	cmd_fstat},
	{"read",	cmd_read},
	{"write",	cmd_write},
	{"rm",		cmd_rm},
	{"mkdir",	cmd_mkdir},
	{"chattr",	cmd_chattr},
	{"move",	cmd_move},
	{"rename",	cmd_rename}
};

static const struct command *find_command(const char *name)
{
	size_t i;

	for (i = 0; i < sizeof(command_table) /
		     sizeof(command_table[0]); i++) {
		const struct command *c = &command_table[i];

		if (!strcasecmp(c->name, name))
			return c;
	}

	return NULL;
}

static int parse_options(int argc, char **argv, struct options *opt)
{
	static const struct option longopts[] = {
		{"help",	0, 0, 'H'},
		{"version",	0, 0, 'V'},
		{"mkfs",	1, 0, 'M'},
		{NULL, 0, 0, 0}
	};
	int o;

	memset(opt, 0, sizeof(*opt));
	opt->log2_bs = 9;

	while ((o = getopt_long(argc, argv, "b:SR:i:o:", longopts, NULL)) >= 0)
		switch (o) {
		case 'i':
			opt->in_file = optarg;
			break;

		case 'o':
			opt->out_file = optarg;
			break;

		case 'M':
			opt->flags |= OPTION_MKFS;
			opt->num_blocks = atoll(optarg);
			break;

		case 'R':
			opt->flags |= OPTION_RANDOMIZE;
			opt->seed = atoi(optarg);
			break;

		case 'S':
			opt->flags |= OPTION_STATISTICS;
			break;

		case 'H':
			usage(argv[0]);
			exit(0);

		case 'V':
			version_banner();
			exit(0);

		case 'b':
			if (parse_blocksize(optarg, &opt->log2_bs) < 0)
				return -1;
			break;

		case '?':
			fprintf(stderr, "Try --help for usage.\n");
			return -1;
		}

	argc -= optind;
	argv += optind;

	if (argc <= 0) {
		fprintf(stderr, "Expected an image name\n");
		return -1;
	}

	opt->filename = argv[0];

	if (argc >= 2) {
		opt->command = find_command(argv[1]);
		opt->argv = argv + 2;
		opt->argc = argc - 2;

		if (!opt->command) {
			fprintf(stderr, "Unknown command: %s\n", argv[1]);
			return -1;
		}
	} else {
		opt->command = NULL;
		opt->argv = NULL;
		opt->argc = 0;
	}

	return 0;
}

static void dump_stats(const struct ufat_stat *st)
{
	fprintf(stderr, "\n");
	fprintf(stderr,
		"Reads:             %6d comprising %6d blocks\n",
		st->read, st->read_blocks);
	fprintf(stderr,
		"Writes:            %6d comprising %6d blocks\n",
		st->write, st->write_blocks);

	fprintf(stderr,	"Cache write/flush: %6d/%6d\n",
		st->cache_write, st->cache_flush);
	fprintf(stderr, "Cache hit/miss:    %6d/%6d",
		st->cache_hit, st->cache_miss);

	if (st->cache_hit + st->cache_miss)
		fprintf(stderr, " (%02d%% hit rate)\n",
			st->cache_hit * 100 / (st->cache_hit + st->cache_miss));
	else
		fprintf(stderr, "\n");
}

int main(int argc, char **argv)
{
	struct file_device dev;
	struct ufat uf;
	struct options opt;
	int err;

	if (parse_options(argc, argv, &opt) < 0)
		return -1;

	srandom(opt.seed);

	if (file_device_open(&dev, opt.filename, opt.log2_bs,
			     opt.flags & OPTION_MKFS) < 0)
		return -1;

	if (opt.flags & OPTION_MKFS) {
		err = ufat_mkfs(&dev.base, opt.num_blocks);
		if (err < 0) {
			fprintf(stderr, "ufat_mkfs: %s\n", ufat_strerror(err));
			file_device_close(&dev);
			return -1;
		}
	}

	err = ufat_open(&uf, &dev.base);
	if (err) {
		fprintf(stderr, "ufat_open: %s\n", ufat_strerror(err));
		file_device_close(&dev);
		return -1;
	}

	if (!opt.command) {
		FILE *out = open_output(opt.out_file);

		if (!out) {
			err = -1;
		} else {
			show_info(out, &uf.bpb);
			err = close_output(opt.out_file, out);
		}
	} else {
		err = opt.command->func(&uf, &opt);
	}

	ufat_close(&uf);
	file_device_close(&dev);

	if (opt.flags & OPTION_STATISTICS)
		dump_stats(&uf.stat);

	return err;
}
