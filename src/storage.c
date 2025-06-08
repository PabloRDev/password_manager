#include "storage.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"

// === File I/O ===

StorageStatus read_file(const char *path, void *buffer, size_t length) {
    assert(path != NULL);
    assert(buffer != NULL);

    // Check if a file exists
    FILE *file = fopen(path, "rb"); // rb -> read binary mode
    if (!file) {
        return STORAGE_ERR_OPEN;
    }

    // Get a file size and check if it's valid
    fseek(file, 0, SEEK_END); // seek to the end of the file
    long file_size = ftell(file); // ftell -> get current file position
    rewind(file); // rewind to the beginning of the file

    // Check if the file size is valid
    if (file_size <= 0 || (size_t) file_size > length) {
        fclose(file);

        return STORAGE_ERR_READ;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    fclose(file);

    // Check if all bytes were read
    if (bytes_read != (size_t) file_size) {
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

// === Vault I/O ===

// === Vault Persistence ===

StorageStatus load_vault(Vault *vault) {
    const char *path = get_vault_file_path();
    if (path == NULL || vault == NULL) return STORAGE_ERR_OPEN;

    return read_file(path, vault, sizeof(Vault));
}

StorageStatus save_vault(const Vault *vault) {
    assert(vault != NULL);

    const char *path = get_vault_file_path();
    if (path == NULL) return STORAGE_ERR_OPEN;

    return write_file(path, vault, sizeof(Vault));
}
