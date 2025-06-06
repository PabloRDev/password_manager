//
// Created by Pablo RD on 5/6/25.
//
// RESPONSIBILITIES: AUTHENTICATION, LOADING PASSWORD, SAVING PASSWORD
#ifndef PASSWORD_H
#define PASSWORD_H

#include <stdbool.h>

#define PASSWORD_METADATA_SIZE (HASH_SIZE + SALT_SIZE)
#define PASSWORD_MAX_LENGTH 256

typedef enum {
    PASSWORD_OK,
    PASSWORD_ERR_STORAGE,
    PASSWORD_ERR_HASH_MISMATCH,
    PASSWORD_ERR_CRYPTO,
} PasswordStatus;

// Verifies the user's master password or creates a new one if no vault exists
PasswordStatus authenticate_master_password(bool *is_new_password);

// Loads the stored hash and salt from the vault metadata
PasswordStatus load_password_data(unsigned char *hash_out, unsigned char *salt_out);

// Saves a new hash and salt to the vault metadata
PasswordStatus save_password_data(const unsigned char *key, const unsigned char *salt);

#endif // PASSWORD_H
