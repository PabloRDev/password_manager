//
// Created by Pablo RD on 5/6/25.
//
#include "crypto.h"

#include <assert.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>

#include "utils.h"

// === Key Derivation ===

CryptoStatus derive_key(const char *password, const unsigned char *salt, unsigned char *key_out) {
    assert(password != NULL);
    assert(salt != NULL);
    assert(key_out != NULL);

    // PKCS5_PBKDF2_HMAC -> implements PBKDF2 (Password-Based Key Derivation Function 2) with HMAC-SHA256 (Hash-based Message Authentication Code)
    if (
        !PKCS5_PBKDF2_HMAC(
            password,
            strlen(password),
            salt,
            SALT_SIZE,
            PBKDF2_ITERATIONS,
            EVP_sha256(),
            KEY_SIZE,
            key_out
        )) {
        return CRYPTO_ERR_KEY_DERIV_FAIL;
    }

    return CRYPTO_OK;
}

// === Encryption ===

CryptoStatus encrypt_data(
    const unsigned char *data_in,
    size_t in_len,
    unsigned char *data_out,
    size_t *out_len,
    const unsigned char *key,
    const unsigned char *iv
) {
    assert(data_in != NULL);
    assert(data_out != NULL);
    assert(out_len != NULL);
    assert(key != NULL);
    assert(iv != NULL);

    int len = 0;
    int ciphertext_len = 0;

    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new(); // Allocate memory for cipher context
    if (!cipher_ctx) {
        return CRYPTO_ERR_OPENSSL_FAIL;
    }

    // Initialize cipher context
    if (!EVP_EncryptInit_ex(cipher_ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(cipher_ctx);

        return CRYPTO_ERR_ENCRYPT_FAIL;
    }

    // Encrypt data in chunks
    if (!EVP_EncryptUpdate(cipher_ctx, data_out, &len, data_in, (int) in_len)) {
        EVP_CIPHER_CTX_free(cipher_ctx);

        return CRYPTO_ERR_ENCRYPT_FAIL;
    }
    ciphertext_len = len;

    // Finalize encryption and write remaining ciphertext to output buffer
    if (!EVP_EncryptFinal_ex(cipher_ctx, data_out + len, &len)) {
        EVP_CIPHER_CTX_free(cipher_ctx);

        return CRYPTO_ERR_ENCRYPT_FAIL;
    }
    ciphertext_len += len;

    *out_len = (size_t) ciphertext_len;

    EVP_CIPHER_CTX_free(cipher_ctx);

    return CRYPTO_OK;
}

// === Decryption ===

CryptoStatus decrypt_data(
    const unsigned char *data_in,
    size_t in_len,
    unsigned char *data_out,
    size_t *out_len,
    const unsigned char *key,
    const unsigned char *iv
) {
    assert(data_in != NULL);
    assert(data_out != NULL);
    assert(out_len != NULL);
    assert(key != NULL);
    assert(iv != NULL);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return CRYPTO_ERR_OPENSSL_FAIL;
    }

    int len = 0;
    int plaintext_len = 0;

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);

        return CRYPTO_ERR_DECRYPT_FAIL;
    }

    if (!EVP_DecryptUpdate(ctx, data_out, &len, data_in, (int) in_len)) {
        EVP_CIPHER_CTX_free(ctx);

        return CRYPTO_ERR_DECRYPT_FAIL;
    }
    plaintext_len = len;

    if (!EVP_DecryptFinal_ex(ctx, data_out + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);

        return CRYPTO_ERR_DECRYPT_FAIL;
    }
    plaintext_len += len;

    *out_len = (size_t) plaintext_len;

    EVP_CIPHER_CTX_free(ctx);

    return CRYPTO_OK;
}

/// === Password Hashing ===

CryptoStatus hash_password(const char *password, unsigned char *hash_out) {
    assert(password != NULL);
    assert(hash_out != NULL);

    // 256-bit hash (32 bytes)
    if (!SHA256((const unsigned char *) password, strlen(password), hash_out)) {
        return CRYPTO_ERR_HASH_FAIL;
    }

    return CRYPTO_OK;
}

// === Password Verification ===

bool verify_password(const char *password, const unsigned char *expected_hash) {
    assert(password != NULL);
    assert(expected_hash != NULL);

    unsigned char computed_hash[HASH_SIZE];
    if (hash_password(password, computed_hash) != CRYPTO_OK) {
        return false;
    }

    bool match = CRYPTO_memcmp(computed_hash, expected_hash, HASH_SIZE) == 0;

    secure_zero(computed_hash, HASH_SIZE); // Zero sensitive information

    return match;
}
#include "crypto.h"

#include <assert.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>

#include "utils.h"

// === Key Derivation ===

CryptoStatus derive_key(const char *password, const unsigned char *salt, unsigned char *key_out) {
    assert(password != NULL);
    assert(salt != NULL);
    assert(key_out != NULL);

    // PKCS5_PBKDF2_HMAC -> implements PBKDF2 (Password-Based Key Derivation Function 2) with HMAC-SHA256 (Hash-based Message Authentication Code)
    if (
        !PKCS5_PBKDF2_HMAC(
            password,
            strlen(password),
            salt,
            SALT_SIZE,
            PBKDF2_ITERATIONS,
            EVP_sha256(),
            KEY_SIZE,
            key_out
        )) {
        return CRYPTO_ERR_KEY_DERIV_FAIL;
    }

    return CRYPTO_OK;
}

// === Encryption ===

CryptoStatus encrypt_data(
    const unsigned char *data_in,
    size_t in_len,
    unsigned char *data_out,
    size_t *out_len,
    const unsigned char *key,
    const unsigned char *iv
) {
    assert(data_in != NULL);
    assert(data_out != NULL);
    assert(out_len != NULL);
    assert(key != NULL);
    assert(iv != NULL);

    int len = 0;
    int ciphertext_len = 0;

    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new(); // Allocate memory for cipher context
    if (!cipher_ctx) {
        return CRYPTO_ERR_OPENSSL_FAIL;
    }

    // Initialize cipher context
    if (!EVP_EncryptInit_ex(cipher_ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(cipher_ctx);

        return CRYPTO_ERR_ENCRYPT_FAIL;
    }

    // Encrypt data in chunks
    if (!EVP_EncryptUpdate(cipher_ctx, data_out, &len, data_in, (int) in_len)) {
        EVP_CIPHER_CTX_free(cipher_ctx);

        return CRYPTO_ERR_ENCRYPT_FAIL;
    }
    ciphertext_len = len;

    // Finalize encryption and write remaining ciphertext to output buffer
    if (!EVP_EncryptFinal_ex(cipher_ctx, data_out + len, &len)) {
        EVP_CIPHER_CTX_free(cipher_ctx);

        return CRYPTO_ERR_ENCRYPT_FAIL;
    }
    ciphertext_len += len;

    *out_len = (size_t) ciphertext_len;

    EVP_CIPHER_CTX_free(cipher_ctx);

    return CRYPTO_OK;
}

// === Decryption ===

CryptoStatus decrypt_data(
    const unsigned char *data_in,
    size_t in_len,
    unsigned char *data_out,
    size_t *out_len,
    const unsigned char *key,
    const unsigned char *iv
) {
    assert(data_in != NULL);
    assert(data_out != NULL);
    assert(out_len != NULL);
    assert(key != NULL);
    assert(iv != NULL);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return CRYPTO_ERR_OPENSSL_FAIL;
    }

    int len = 0;
    int plaintext_len = 0;

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);

        return CRYPTO_ERR_DECRYPT_FAIL;
    }

    if (!EVP_DecryptUpdate(ctx, data_out, &len, data_in, (int) in_len)) {
        EVP_CIPHER_CTX_free(ctx);

        return CRYPTO_ERR_DECRYPT_FAIL;
    }
    plaintext_len = len;

    if (!EVP_DecryptFinal_ex(ctx, data_out + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);

        return CRYPTO_ERR_DECRYPT_FAIL;
    }
    plaintext_len += len;

    *out_len = (size_t) plaintext_len;

    EVP_CIPHER_CTX_free(ctx);

    return CRYPTO_OK;
}

/// === Password Hashing ===

CryptoStatus hash_password(const char *password, unsigned char *hash_out) {
    assert(password != NULL);
    assert(hash_out != NULL);

    // 256-bit hash (32 bytes)
    if (!SHA256((const unsigned char *) password, strlen(password), hash_out)) {
        return CRYPTO_ERR_HASH_FAIL;
    }

    return CRYPTO_OK;
}

// === Password Verification ===

bool verify_password(const char *password, const unsigned char *expected_hash) {
    assert(password != NULL);
    assert(expected_hash != NULL);

    unsigned char computed_hash[HASH_SIZE];
    if (hash_password(password, computed_hash) != CRYPTO_OK) {
        return false;
    }

    bool match = CRYPTO_memcmp(computed_hash, expected_hash, HASH_SIZE) == 0;

    secure_zero(computed_hash, HASH_SIZE); // Zero sensitive information

    return match;
}
