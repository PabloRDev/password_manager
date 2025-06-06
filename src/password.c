//
// Created by Pablo RD on 5/6/25.
//

#include "password.h"
#include "utils.h"
#include "crypto.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <openssl/rand.h>

// === Authenticate Master Password ===

PasswordStatus authenticate_master_password(bool *is_new_password) {
    assert(is_new_password != NULL);

    unsigned char stored_key[KEY_SIZE];
    unsigned char stored_salt[SALT_SIZE];

    // Vault does not exist —> create new password
    if (!vault_file_exists()) {
        char password[PASSWORD_MAX_LENGTH];

        if (!read_password(password, sizeof(password))) {
            return PASSWORD_ERR_STORAGE;
        }

        // Generate random salt
        if (!RAND_bytes(stored_salt, SALT_SIZE)) {
            return PASSWORD_ERR_CRYPTO;
        }

        // Derive key from password and salt
        if (derive_key(password, stored_salt, stored_key) != CRYPTO_OK) {
            secure_zero(password, sizeof(password));
            secure_zero(stored_salt, sizeof(stored_salt));

            return PASSWORD_ERR_CRYPTO;
        }

        if (save_password_data(stored_key, stored_salt) != PASSWORD_OK) {
            secure_zero(password, sizeof(password));
            secure_zero(stored_salt, sizeof(stored_salt));

            return PASSWORD_ERR_STORAGE;
        }

        secure_zero(password, sizeof(password));
        secure_zero(stored_salt, sizeof(stored_salt));
        *is_new_password = true;

        return PASSWORD_OK;
    }

    // Vault exists —> verify password
    if (load_password_data(stored_key, stored_salt) != PASSWORD_OK) {
        return PASSWORD_ERR_STORAGE;
    }

    char input[PASSWORD_MAX_LENGTH];
    if (!read_password(input, sizeof(input))) {
        secure_zero(input, sizeof(input));

        return PASSWORD_ERR_STORAGE;
    }

    unsigned char derived_key[KEY_SIZE]; // 256-bit key (32 bytes)
    // Derive key from password and salt
    if (derive_key(input, stored_salt, derived_key) != CRYPTO_OK) {
        secure_zero(input, sizeof(input));
        secure_zero(stored_key, sizeof(stored_key));

        return PASSWORD_ERR_CRYPTO;
    }

    bool ok = (CRYPTO_memcmp(derived_key, stored_key, KEY_SIZE) == 0); // Compare derived key with stored hash
    secure_zero(input, sizeof(input));
    secure_zero(derived_key, sizeof(derived_key));
    secure_zero(stored_key, sizeof(stored_key));
    secure_zero(stored_salt, sizeof(stored_salt));

    if (!ok) {
        return PASSWORD_ERR_HASH_MISMATCH;
    }

    *is_new_password = false;

    return PASSWORD_OK;
}

// === Load Password Data ===

PasswordStatus load_password_data(unsigned char *hash_out, unsigned char *salt_out) {
    assert(hash_out != NULL);
    assert(salt_out != NULL);

    // Check if a vault file exists
    FILE *file = fopen(get_vault_file_path(), "rb"); // rb -> read binary mode
    if (!file) return PASSWORD_ERR_STORAGE;

    // Read hash from a file
    if (fread(hash_out, 1, KEY_SIZE, file) != KEY_SIZE) {
        fclose(file);

        return PASSWORD_ERR_STORAGE;
    }

    // Read salt from a file
    if (fread(salt_out, 1, SALT_SIZE, file) != SALT_SIZE) {
        fclose(file);

        return PASSWORD_ERR_STORAGE;
    }

    fclose(file);

    return PASSWORD_OK;
}

// === Save Password Data ===

PasswordStatus save_password_data(const unsigned char *key, const unsigned char *salt) {
    assert(key != NULL);
    assert(salt != NULL);

    // Check if a vault file exists
    FILE *file = fopen(get_vault_file_path(), "wb"); // wb -> write binary mode
    if (!file) return PASSWORD_ERR_STORAGE;

    // Write hash to a file
    if (fwrite(key, 1, KEY_SIZE, file) != KEY_SIZE) {
        fclose(file);

        return PASSWORD_ERR_STORAGE;
    }

    // Write salt to a file
    if (fwrite(salt, 1, SALT_SIZE, file) != SALT_SIZE) {
        fclose(file);

        return PASSWORD_ERR_STORAGE;
    }

    fclose(file);

    return PASSWORD_OK;
}
