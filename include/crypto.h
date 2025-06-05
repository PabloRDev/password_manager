//
// Created by Pablo RD on 5/6/25.
//
// RESPONSIBILITIES: DERIVE KEY, ENCRYPT, DECRYPT, HASH PASSWORD, VERIFY PASSWORD
#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>
#include <stdbool.h>

#define SALT_SIZE 16
#define PBKDF2_ITERATIONS 100000 // 100,000 iterations = 100,000 rounds of PBKDF2
#define KEY_SIZE 32      // AES-256 = 256 bits = 32 bytes
#define IV_SIZE 16
#define HASH_SIZE 32     // SHA-256 output

typedef enum {
    CRYPTO_OK,
    CRYPTO_ERR_OPENSSL_FAIL,
    CRYPTO_ERR_ENCRYPT_FAIL,
    CRYPTO_ERR_DECRYPT_FAIL,
    CRYPTO_ERR_KEY_DERIV_FAIL,
    CRYPTO_ERR_HASH_FAIL,
} CryptoStatus;

// === Key Derivation ===

// Creates a 256-bit key using master password + salt
CryptoStatus derive_key(const char *password, const unsigned char *salt, unsigned char *key_out);

// === Encryption & Decryption ===

// Encrypts data_in into data_out using AES-256-CBC with key and an initialization vector (iv)
CryptoStatus encrypt_data(const unsigned char *data_in, size_t in_len, unsigned char *data_out, size_t *out_len,
                  const unsigned char *key, const unsigned char *iv);

// Decrypts data_in into data_out using AES-256-CBC with key and an initialization vector (iv)
CryptoStatus decrypt_data(const unsigned char *data_in, size_t in_len,
                  unsigned char *data_out, size_t *out_len,
                  const unsigned char *key, const unsigned char *iv);

// === Master Password Hashing ===

// Hashes master password to store it securely
CryptoStatus hash_password(const char *password, unsigned char *hash_out);

// Verifies that a password matches a stored hash
bool verify_password(const char *password, const unsigned char *expected_hash);

#endif // CRYPTO_H
