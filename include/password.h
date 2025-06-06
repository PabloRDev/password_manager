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

/**
 * @brief Authenticates the user by verifying the master password.
 *
 * If no vault metadata exists, prompts for and sets a new password.
 * If metadata exists, asks for a password and verifies it using stored hash and salt.
 *
 * @param is_new_password Pointer to bool that will be set to true if a new password was created.
 * @return PASSWORD_OK on success, PASSWORD_ERR_HASH_MISMATCH if password is incorrect,
 *         PASSWORD_ERR_CRYPTO on crypto error, PASSWORD_ERR_STORAGE on I/O failure.
 */
PasswordStatus authenticate_master_password(bool *is_new_password);

/**
 * @brief Loads the stored password hash and salt from persistent storage (vault metadata).
 *
 * @param hash_out Output buffer to receive the hash (must be HASH_SIZE bytes).
 * @param salt_out Output buffer to receive the salt (must be SALT_SIZE bytes).
 * @return PASSWORD_OK on success, PASSWORD_ERR_STORAGE if the data could not be loaded.
 */
PasswordStatus load_password_data(unsigned char *hash_out, unsigned char *salt_out);

/**
 * @brief Saves a password hash and salt into persistent storage (vault metadata).
 *
 * @param key The password hash to store (must be HASH_SIZE bytes).
 * @param salt The salt to store (must be SALT_SIZE bytes).
 * @return PASSWORD_OK on success, PASSWORD_ERR_STORAGE if saving fails.
 */
PasswordStatus save_password_data(const unsigned char *key, const unsigned char *salt);

#endif // PASSWORD_H
