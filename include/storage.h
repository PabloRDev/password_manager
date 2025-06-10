//
// Created by Pablo RD on 5/6/25.
//
// RESPONSIBILITIES: FILE I/O + Vault persistence
#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_ENTRIES 100
#define SERVICE_NAME_MAX 64
#define USERNAME_MAX 64
#define PASSWORD_MAX 64
#define NOTES_MAX 256

typedef struct {
    char service[SERVICE_NAME_MAX];
    char username[USERNAME_MAX];
    char password[PASSWORD_MAX];
    char notes[NOTES_MAX];
} VaultEntry;

typedef struct {
    VaultEntry entries[MAX_ENTRIES];
    int count;
} Vault;

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
 * @return       StorageStatus with status.
 */
StorageStatus read_file(const char *path, void *buffer, size_t length);

/**
 * Writes binary data to a file.
 *
 * @param path     Path to the file.
 * @param data     Pointer to the data to write.
 * @param length   Number of bytes to write.
 * @return       StorageStatus with status.
 */
StorageStatus write_file(const char *path, const void *data, size_t length);

// === Vault Persistence ===

/**
 * @brief Loads the vault structure from persistent storage.
 *
 * @param vault    Pointer to the Vault structure to load.
 * @return         StorageStatus indicating success or error.
 */
StorageStatus load_vault(Vault *vault);

/**
 * @brief Saves the vault structure to persistent storage.
 *
 * @param vault    Pointer to the Vault structure to save.
 * @return         StorageStatus indicating success or error.
 */
StorageStatus save_vault(const Vault *vault);

// === Vault I/O ===

/** @brief Adds a new vault entry.
 *
 * @param vault Pointer to the Vault structure to modify.
 * @param entry Pointer to the VaultEntry structure to add.
 *
 */
bool add_vault_entry(Vault *vault, const VaultEntry *entry);

/** @brief Gets a vault entry from the user input.
 *
 * @param entry Pointer to the VaultEntry structure to fill.
 */
void get_vault_entry(VaultEntry *entry);

/**
 * @brief Searches for a vault entry by service name.
 *
 * @param vault Pointer to the Vault structure to search.
 * @param service Service name to search for.
 * @return Pointer to the VaultEntry structure if found, NULL otherwise.
 */
VaultEntry *search_vault_entry(Vault *vault, const char *service);

#endif // STORAGE_H
