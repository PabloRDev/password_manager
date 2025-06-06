//
// Created by Pablo RD on 5/6/25.
//
// RESPONSIBILITIES: DERIVE KEY, ENCRYPT, DECRYPT, HASH PASSWORD, VERIFY PASSWORD
#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>
#include <stdbool.h>

#define SALT_SIZE 16
#define PBKDF2_ITERATIONS 100000  // 100,000 iterations = 100,000 rounds of PBKDF2
#define KEY_SIZE 32               // AES-256 = 256 bits = 32 bytes
#define IV_SIZE 16
#define HASH_SIZE 32              // SHA-256 output

typedef enum {
    CRYPTO_OK,
    CRYPTO_ERR_OPENSSL_FAIL,
    CRYPTO_ERR_ENCRYPT_FAIL,
    CRYPTO_ERR_DECRYPT_FAIL,
    CRYPTO_ERR_KEY_DERIV_FAIL,
    CRYPTO_ERR_HASH_FAIL,
} CryptoStatus;

// === Key Derivation ===

/**
 * @brief Derives a 256-bit encryption key from a password and salt using PBKDF2 with HMAC-SHA256.
 *
 * @param password Null-terminated master password string.
 * @param salt Random salt (must be SALT_SIZE bytes).
 * @param key_out Output buffer to store the derived key (must be KEY_SIZE bytes).
 * @return CRYPTO_OK on success, CRYPTO_ERR_KEY_DERIV_FAIL on failure.
 */
CryptoStatus derive_key(const char *password, const unsigned char *salt, unsigned char *key_out);

// === Encryption & Decryption ===

/**
 * @brief Encrypts input data using AES-256-CBC.
 *
 * @param data_in Pointer to plaintext input data.
 * @param in_len Length of input data in bytes.
 * @param data_out Output buffer to store encrypted data. Must be large enough to hold ciphertext.
 * @param out_len Pointer to size_t that receives the number of bytes written to data_out.
 * @param key 256-bit encryption key (KEY_SIZE bytes).
 * @param iv Initialization vector (IV_SIZE bytes).
 * @return CRYPTO_OK on success, CRYPTO_ERR_ENCRYPT_FAIL on failure.
 */
CryptoStatus encrypt_data(const unsigned char *data_in, size_t in_len,
                          unsigned char *data_out, size_t *out_len,
                          const unsigned char *key, const unsigned char *iv);

/**
 * @brief Decrypts input data using AES-256-CBC.
 *
 * @param data_in Pointer to ciphertext input data.
 * @param in_len Length of input ciphertext in bytes.
 * @param data_out Output buffer to store decrypted plaintext.
 * @param out_len Pointer to size_t that receives the number of bytes written to data_out.
 * @param key 256-bit decryption key (KEY_SIZE bytes).
 * @param iv Initialization vector used during encryption (IV_SIZE bytes).
 * @return CRYPTO_OK on success, CRYPTO_ERR_DECRYPT_FAIL on failure.
 */
CryptoStatus decrypt_data(const unsigned char *data_in, size_t in_len,
                          unsigned char *data_out, size_t *out_len,
                          const unsigned char *key, const unsigned char *iv);

// === Master Password Hashing ===

/**
 * @brief Hashes a password using SHA-256.
 *
 * @param password Null-terminated string of the password.
 * @param hash_out Output buffer for hash (must be HASH_SIZE bytes).
 * @return CRYPTO_OK on success, CRYPTO_ERR_HASH_FAIL on failure.
 */
CryptoStatus hash_password(const char *password, unsigned char *hash_out);

/**
 * @brief Verifies if a password matches a known hash using constant-time comparison.
 *
 * @param password Null-terminated password to verify.
 * @param expected_hash Expected hash to compare against (HASH_SIZE bytes).
 * @return true if password matches hash, false otherwise.
 */
bool verify_password(const char *password, const unsigned char *expected_hash);

#endif // CRYPTO_H
