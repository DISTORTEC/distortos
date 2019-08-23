#!/bin/bash
set -eu
export TEST_FILE=$0
trap 'export TEST_LINE=$LINENO' DEBUG

echo "=== Interspersed tests ==="
rm -rf blocks
scripts/test.py << TEST
    lfs2_format(&lfs2, &cfg) => 0;
TEST

echo "--- Interspersed file test ---"
scripts/test.py << TEST
    lfs2_mount(&lfs2, &cfg) => 0;
    lfs2_file_t files[4];
    lfs2_file_open(&lfs2, &files[0], "a", LFS2_O_WRONLY | LFS2_O_CREAT) => 0;
    lfs2_file_open(&lfs2, &files[1], "b", LFS2_O_WRONLY | LFS2_O_CREAT) => 0;
    lfs2_file_open(&lfs2, &files[2], "c", LFS2_O_WRONLY | LFS2_O_CREAT) => 0;
    lfs2_file_open(&lfs2, &files[3], "d", LFS2_O_WRONLY | LFS2_O_CREAT) => 0;

    for (int i = 0; i < 10; i++) {
        lfs2_file_write(&lfs2, &files[0], (const void*)"a", 1) => 1;
        lfs2_file_write(&lfs2, &files[1], (const void*)"b", 1) => 1;
        lfs2_file_write(&lfs2, &files[2], (const void*)"c", 1) => 1;
        lfs2_file_write(&lfs2, &files[3], (const void*)"d", 1) => 1;
    }

    lfs2_file_close(&lfs2, &files[0]);
    lfs2_file_close(&lfs2, &files[1]);
    lfs2_file_close(&lfs2, &files[2]);
    lfs2_file_close(&lfs2, &files[3]);

    lfs2_dir_open(&lfs2, &dir, "/") => 0;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, ".") => 0;
    info.type => LFS2_TYPE_DIR;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, "..") => 0;
    info.type => LFS2_TYPE_DIR;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, "a") => 0;
    info.type => LFS2_TYPE_REG;
    info.size => 10;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, "b") => 0;
    info.type => LFS2_TYPE_REG;
    info.size => 10;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, "c") => 0;
    info.type => LFS2_TYPE_REG;
    info.size => 10;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, "d") => 0;
    info.type => LFS2_TYPE_REG;
    info.size => 10;
    lfs2_dir_read(&lfs2, &dir, &info) => 0;
    lfs2_dir_close(&lfs2, &dir) => 0;

    lfs2_file_open(&lfs2, &files[0], "a", LFS2_O_RDONLY) => 0;
    lfs2_file_open(&lfs2, &files[1], "b", LFS2_O_RDONLY) => 0;
    lfs2_file_open(&lfs2, &files[2], "c", LFS2_O_RDONLY) => 0;
    lfs2_file_open(&lfs2, &files[3], "d", LFS2_O_RDONLY) => 0;

    for (int i = 0; i < 10; i++) {
        lfs2_file_read(&lfs2, &files[0], buffer, 1) => 1;
        buffer[0] => 'a';
        lfs2_file_read(&lfs2, &files[1], buffer, 1) => 1;
        buffer[0] => 'b';
        lfs2_file_read(&lfs2, &files[2], buffer, 1) => 1;
        buffer[0] => 'c';
        lfs2_file_read(&lfs2, &files[3], buffer, 1) => 1;
        buffer[0] => 'd';
    }

    lfs2_file_close(&lfs2, &files[0]);
    lfs2_file_close(&lfs2, &files[1]);
    lfs2_file_close(&lfs2, &files[2]);
    lfs2_file_close(&lfs2, &files[3]);
    
    lfs2_unmount(&lfs2) => 0;
TEST

echo "--- Interspersed remove file test ---"
scripts/test.py << TEST
    lfs2_mount(&lfs2, &cfg) => 0;
    lfs2_file_t files[4];
    lfs2_file_open(&lfs2, &files[0], "e", LFS2_O_WRONLY | LFS2_O_CREAT) => 0;

    for (int i = 0; i < 5; i++) {
        lfs2_file_write(&lfs2, &files[0], (const void*)"e", 1) => 1;
    }

    lfs2_remove(&lfs2, "a") => 0;
    lfs2_remove(&lfs2, "b") => 0;
    lfs2_remove(&lfs2, "c") => 0;
    lfs2_remove(&lfs2, "d") => 0;

    for (int i = 0; i < 5; i++) {
        lfs2_file_write(&lfs2, &files[0], (const void*)"e", 1) => 1;
    }

    lfs2_file_close(&lfs2, &files[0]);

    lfs2_dir_open(&lfs2, &dir, "/") => 0;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, ".") => 0;
    info.type => LFS2_TYPE_DIR;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, "..") => 0;
    info.type => LFS2_TYPE_DIR;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, "e") => 0;
    info.type => LFS2_TYPE_REG;
    info.size => 10;
    lfs2_dir_read(&lfs2, &dir, &info) => 0;
    lfs2_dir_close(&lfs2, &dir) => 0;

    lfs2_file_open(&lfs2, &files[0], "e", LFS2_O_RDONLY) => 0;

    for (int i = 0; i < 10; i++) {
        lfs2_file_read(&lfs2, &files[0], buffer, 1) => 1;
        buffer[0] => 'e';
    }

    lfs2_file_close(&lfs2, &files[0]);
    
    lfs2_unmount(&lfs2) => 0;
TEST

echo "--- Remove inconveniently test ---"
scripts/test.py << TEST
    lfs2_mount(&lfs2, &cfg) => 0;
    lfs2_file_t files[4];
    lfs2_file_open(&lfs2, &files[0], "e", LFS2_O_WRONLY | LFS2_O_TRUNC) => 0;
    lfs2_file_open(&lfs2, &files[1], "f", LFS2_O_WRONLY | LFS2_O_CREAT) => 0;
    lfs2_file_open(&lfs2, &files[2], "g", LFS2_O_WRONLY | LFS2_O_CREAT) => 0;

    for (int i = 0; i < 5; i++) {
        lfs2_file_write(&lfs2, &files[0], (const void*)"e", 1) => 1;
        lfs2_file_write(&lfs2, &files[1], (const void*)"f", 1) => 1;
        lfs2_file_write(&lfs2, &files[2], (const void*)"g", 1) => 1;
    }

    lfs2_remove(&lfs2, "f") => 0;

    for (int i = 0; i < 5; i++) {
        lfs2_file_write(&lfs2, &files[0], (const void*)"e", 1) => 1;
        lfs2_file_write(&lfs2, &files[1], (const void*)"f", 1) => 1;
        lfs2_file_write(&lfs2, &files[2], (const void*)"g", 1) => 1;
    }

    lfs2_file_close(&lfs2, &files[0]);
    lfs2_file_close(&lfs2, &files[1]);
    lfs2_file_close(&lfs2, &files[2]);

    lfs2_dir_open(&lfs2, &dir, "/") => 0;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, ".") => 0;
    info.type => LFS2_TYPE_DIR;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, "..") => 0;
    info.type => LFS2_TYPE_DIR;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, "e") => 0;
    info.type => LFS2_TYPE_REG;
    info.size => 10;
    lfs2_dir_read(&lfs2, &dir, &info) => 1;
    strcmp(info.name, "g") => 0;
    info.type => LFS2_TYPE_REG;
    info.size => 10;
    lfs2_dir_read(&lfs2, &dir, &info) => 0;
    lfs2_dir_close(&lfs2, &dir) => 0;

    lfs2_file_open(&lfs2, &files[0], "e", LFS2_O_RDONLY) => 0;
    lfs2_file_open(&lfs2, &files[1], "g", LFS2_O_RDONLY) => 0;

    for (int i = 0; i < 10; i++) {
        lfs2_file_read(&lfs2, &files[0], buffer, 1) => 1;
        buffer[0] => 'e';
        lfs2_file_read(&lfs2, &files[1], buffer, 1) => 1;
        buffer[0] => 'g';
    }

    lfs2_file_close(&lfs2, &files[0]);
    lfs2_file_close(&lfs2, &files[1]);
    
    lfs2_unmount(&lfs2) => 0;
TEST

scripts/results.py
