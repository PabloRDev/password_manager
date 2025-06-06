#include "storage.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// === File I/O Helpers ===

StorageStatus read_file(const char *path, void *buffer, size_t length) {
    assert(path != NULL);
    assert(buffer != NULL);

    // Check if a file exists
    FILE *file = fopen(path, "rb"); // rb -> read binary mode
    if (!file) {
        return STORAGE_ERR_OPEN;
    }

    // Read a file into a buffer
    const size_t bytes_read = fread(buffer, 1, length, file);
    fclose(file);

    // Check if all bytes were read
    if (bytes_read != length) {
        return STORAGE_ERR_READ;
    }

    return STORAGE_OK;
}

StorageStatus write_file(const char *path, const void *data, size_t length) {
    assert(path != NULL);
    assert(data != NULL);

    FILE *file = fopen(path, "wb"); // wb -> write binary mode
    if (!file) {
        return STORAGE_ERR_OPEN;
    }

    const size_t bytes_written = fwrite(data, 1, length, file);
    fclose(file);

    if (bytes_written != length) {
        return STORAGE_ERR_WRITE;
    }

    return STORAGE_OK;
}
