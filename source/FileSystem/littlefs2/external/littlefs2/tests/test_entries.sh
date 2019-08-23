#!/bin/bash
set -eu
export TEST_FILE=$0
trap 'export TEST_LINE=$LINENO' DEBUG

echo "=== Entry tests ==="

# Note: These tests are intended for 512 byte inline size at different
# inline sizes they should still pass, but won't be testing anything

rm -rf blocks
function read_file {
cat << TEST

    size = $2;
    lfs2_file_open(&lfs2, &file, "$1", LFS2_O_RDONLY) => 0;
    lfs2_file_read(&lfs2, &file, rbuffer, size) => size;
    memcmp(rbuffer, wbuffer, size) => 0;
    lfs2_file_close(&lfs2, &file) => 0;
TEST
}

function write_file {
cat << TEST

    size = $2;
    lfs2_file_open(&lfs2, &file, "$1",
            LFS2_O_WRONLY | LFS2_O_CREAT | LFS2_O_TRUNC) => 0;
    memset(wbuffer, 'c', size);
    lfs2_file_write(&lfs2, &file, wbuffer, size) => size;
    lfs2_file_close(&lfs2, &file) => 0;
TEST
}

echo "--- Entry grow test ---"
scripts/test.py << TEST
    lfs2_format(&lfs2, &cfg) => 0;

    uint8_t wbuffer[1024];
    uint8_t rbuffer[1024];
    lfs2_size_t size;

    lfs2_mount(&lfs2, &cfg) => 0;
    $(write_file "hi0" 20)
    $(write_file "hi1" 20)
    $(write_file "hi2" 20)
    $(write_file "hi3" 20)

    $(read_file "hi1" 20)
    $(write_file "hi1" 200)

    $(read_file "hi0" 20)
    $(read_file "hi1" 200)
    $(read_file "hi2" 20)
    $(read_file "hi3" 20)
    lfs2_unmount(&lfs2) => 0;
TEST

echo "--- Entry shrink test ---"
scripts/test.py << TEST
    lfs2_format(&lfs2, &cfg) => 0;

    uint8_t wbuffer[1024];
    uint8_t rbuffer[1024];
    lfs2_size_t size;

    lfs2_mount(&lfs2, &cfg) => 0;
    $(write_file "hi0" 20)
    $(write_file "hi1" 200)
    $(write_file "hi2" 20)
    $(write_file "hi3" 20)

    $(read_file "hi1" 200)
    $(write_file "hi1" 20)

    $(read_file "hi0" 20)
    $(read_file "hi1" 20)
    $(read_file "hi2" 20)
    $(read_file "hi3" 20)
    lfs2_unmount(&lfs2) => 0;
TEST

echo "--- Entry spill test ---"
scripts/test.py << TEST
    lfs2_format(&lfs2, &cfg) => 0;

    uint8_t wbuffer[1024];
    uint8_t rbuffer[1024];
    lfs2_size_t size;

    lfs2_mount(&lfs2, &cfg) => 0;
    $(write_file "hi0" 200)
    $(write_file "hi1" 200)
    $(write_file "hi2" 200)
    $(write_file "hi3" 200)

    $(read_file "hi0" 200)
    $(read_file "hi1" 200)
    $(read_file "hi2" 200)
    $(read_file "hi3" 200)
    lfs2_unmount(&lfs2) => 0;
TEST

echo "--- Entry push spill test ---"
scripts/test.py << TEST
    lfs2_format(&lfs2, &cfg) => 0;

    uint8_t wbuffer[1024];
    uint8_t rbuffer[1024];
    lfs2_size_t size;

    lfs2_mount(&lfs2, &cfg) => 0;
    $(write_file "hi0" 200)
    $(write_file "hi1" 20)
    $(write_file "hi2" 200)
    $(write_file "hi3" 200)

    $(read_file "hi1" 20)
    $(write_file "hi1" 200)

    $(read_file "hi0" 200)
    $(read_file "hi1" 200)
    $(read_file "hi2" 200)
    $(read_file "hi3" 200)
    lfs2_unmount(&lfs2) => 0;
TEST

echo "--- Entry push spill two test ---"
scripts/test.py << TEST
    lfs2_format(&lfs2, &cfg) => 0;

    uint8_t wbuffer[1024];
    uint8_t rbuffer[1024];
    lfs2_size_t size;

    lfs2_mount(&lfs2, &cfg) => 0;
    $(write_file "hi0" 200)
    $(write_file "hi1" 20)
    $(write_file "hi2" 200)
    $(write_file "hi3" 200)
    $(write_file "hi4" 200)

    $(read_file "hi1" 20)
    $(write_file "hi1" 200)

    $(read_file "hi0" 200)
    $(read_file "hi1" 200)
    $(read_file "hi2" 200)
    $(read_file "hi3" 200)
    $(read_file "hi4" 200)
    lfs2_unmount(&lfs2) => 0;
TEST

echo "--- Entry drop test ---"
scripts/test.py << TEST
    lfs2_format(&lfs2, &cfg) => 0;

    uint8_t wbuffer[1024];
    uint8_t rbuffer[1024];
    lfs2_size_t size;

    lfs2_mount(&lfs2, &cfg) => 0;
    $(write_file "hi0" 200)
    $(write_file "hi1" 200)
    $(write_file "hi2" 200)
    $(write_file "hi3" 200)

    lfs2_remove(&lfs2, "hi1") => 0;
    lfs2_stat(&lfs2, "hi1", &info) => LFS2_ERR_NOENT;
    $(read_file "hi0" 200)
    $(read_file "hi2" 200)
    $(read_file "hi3" 200)

    lfs2_remove(&lfs2, "hi2") => 0;
    lfs2_stat(&lfs2, "hi2", &info) => LFS2_ERR_NOENT;
    $(read_file "hi0" 200)
    $(read_file "hi3" 200)

    lfs2_remove(&lfs2, "hi3") => 0;
    lfs2_stat(&lfs2, "hi3", &info) => LFS2_ERR_NOENT;
    $(read_file "hi0" 200)

    lfs2_remove(&lfs2, "hi0") => 0;
    lfs2_stat(&lfs2, "hi0", &info) => LFS2_ERR_NOENT;
    lfs2_unmount(&lfs2) => 0;
TEST

echo "--- Create too big ---"
scripts/test.py << TEST
    lfs2_format(&lfs2, &cfg) => 0;

    lfs2_mount(&lfs2, &cfg) => 0;
    memset(path, 'm', 200);
    path[200] = '\0';

    lfs2_size_t size = 400;
    lfs2_file_open(&lfs2, &file, path,
            LFS2_O_WRONLY | LFS2_O_CREAT | LFS2_O_TRUNC) => 0;
    uint8_t wbuffer[1024];
    memset(wbuffer, 'c', size);
    lfs2_file_write(&lfs2, &file, wbuffer, size) => size;
    lfs2_file_close(&lfs2, &file) => 0;

    size = 400;
    lfs2_file_open(&lfs2, &file, path, LFS2_O_RDONLY) => 0;
    uint8_t rbuffer[1024];
    lfs2_file_read(&lfs2, &file, rbuffer, size) => size;
    memcmp(rbuffer, wbuffer, size) => 0;
    lfs2_file_close(&lfs2, &file) => 0;
    lfs2_unmount(&lfs2) => 0;
TEST

echo "--- Resize too big ---"
scripts/test.py << TEST
    lfs2_format(&lfs2, &cfg) => 0;

    lfs2_mount(&lfs2, &cfg) => 0;
    memset(path, 'm', 200);
    path[200] = '\0';

    lfs2_size_t size = 40;
    lfs2_file_open(&lfs2, &file, path,
            LFS2_O_WRONLY | LFS2_O_CREAT | LFS2_O_TRUNC) => 0;
    uint8_t wbuffer[1024];
    memset(wbuffer, 'c', size);
    lfs2_file_write(&lfs2, &file, wbuffer, size) => size;
    lfs2_file_close(&lfs2, &file) => 0;

    size = 40;
    lfs2_file_open(&lfs2, &file, path, LFS2_O_RDONLY) => 0;
    uint8_t rbuffer[1024];
    lfs2_file_read(&lfs2, &file, rbuffer, size) => size;
    memcmp(rbuffer, wbuffer, size) => 0;
    lfs2_file_close(&lfs2, &file) => 0;

    size = 400;
    lfs2_file_open(&lfs2, &file, path,
            LFS2_O_WRONLY | LFS2_O_CREAT | LFS2_O_TRUNC) => 0;
    memset(wbuffer, 'c', size);
    lfs2_file_write(&lfs2, &file, wbuffer, size) => size;
    lfs2_file_close(&lfs2, &file) => 0;

    size = 400;
    lfs2_file_open(&lfs2, &file, path, LFS2_O_RDONLY) => 0;
    lfs2_file_read(&lfs2, &file, rbuffer, size) => size;
    memcmp(rbuffer, wbuffer, size) => 0;
    lfs2_file_close(&lfs2, &file) => 0;
    lfs2_unmount(&lfs2) => 0;
TEST

scripts/results.py
