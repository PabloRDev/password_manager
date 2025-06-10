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

bool add_vault_entry(Vault *vault, const VaultEntry *entry) {
    if (vault->count >= MAX_ENTRIES) {
        printf("❌ Vault is full! You need to delete one password before.\n");

        return false;
    }

    // Copy service name, username, and password safely
    strncpy(vault->entries[vault->count].service, entry->service, SERVICE_NAME_MAX - 1);
    vault->entries[vault->count].service[SERVICE_NAME_MAX - 1] = '\0';

    strncpy(vault->entries[vault->count].username, entry->username, USERNAME_MAX - 1);
    vault->entries[vault->count].username[USERNAME_MAX - 1] = '\0';

    strncpy(vault->entries[vault->count].password, entry->password, PASSWORD_MAX - 1);
    vault->entries[vault->count].password[PASSWORD_MAX - 1] = '\0';

    strncpy(vault->entries[vault->count].notes, entry->notes, NOTES_MAX - 1);
    vault->entries[vault->count].notes[NOTES_MAX - 1] = '\0';

    vault->count++;

    return true;
}

void get_vault_entry(VaultEntry *entry) {
    if (!entry) return;

    printf("Enter Service:\n");
    if (read_line(entry->service, SERVICE_NAME_MAX) != READ_OK) {
        entry->service[0] = '\0'; // If the user enters nothing, set to empty string
    }

    printf("Enter Username:\n");
    if (read_line(entry->username, USERNAME_MAX) != READ_OK) {
        entry->username[0] = '\0';
    }

    printf("Enter Password:\n");
    if (read_password(entry->password, PASSWORD_MAX) == NULL) {
        entry->password[0] = '\0';
    }

    printf("Enter Notes (optional):\n");
    if (read_line(entry->notes, NOTES_MAX) != READ_OK) {
        entry->notes[0] = '\0';
    }
}

VaultEntry *search_vault_entry(Vault *vault, const char *service) {
    if (!vault || !service) return NULL;

    for (int i = 0; i < vault->count; ++i) {
        if (strcmp(vault->entries[i].service, service) == 0) {
            return &vault->entries[i];
        }
    }

    return NULL;
}

// === Vault Persistence ===

StorageStatus load_vault(Vault *vault) {
    const char *vault_path = get_file_path("vault");

    if (vault_path == NULL || vault == NULL) return STORAGE_ERR_OPEN;

    return read_file(vault_path, vault, sizeof(Vault));
}

StorageStatus save_vault(const Vault *vault) {
    assert(vault != NULL);

    const char *vault_path = get_file_path("vault");

    if (vault_path == NULL) return STORAGE_ERR_OPEN;

    return write_file(vault_path, vault, sizeof(Vault));
}
