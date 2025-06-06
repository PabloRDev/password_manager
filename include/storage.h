//
// Created by Pablo RD on 5/6/25.
//
// RESPONSIBILITIES: FILE I/O
#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stddef.h>

#define VAULT_FILE_PATH "vault.dat"

typedef enum {
    STORAGE_OK,
    STORAGE_ERR_OPEN,
    STORAGE_ERR_READ,
    STORAGE_ERR_WRITE
} StorageStatus;

// === Secure File I/O ===

/**
 * Reads binary data from a file.
 *
 * @param path     Path to the file.
 * @param buffer   Output buffer to store read data.
 * @param length   Number of bytes to read.
 * @return       StorageResult with status and message.
 */
StorageStatus read_file(const char *path, void *buffer, size_t length);

/**
 * Writes binary data to a file.
 *
 * @param path     Path to the file.
 * @param data     Pointer to the data to write.
 * @param length   Number of bytes to write.
 * @return       StorageResult with status and message.
 */
StorageStatus write_file(const char *path, const void *data, size_t length);

#endif // STORAGE_H
