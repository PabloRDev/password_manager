//
// Created by Pablo RD on 5/6/25.
//
#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdbool.h>

#define ASK_YES_NO_MAX_PROMPT_LENGTH 4

typedef enum {
    READ_OK,
    READ_ERROR
} ReadStatus;

// === Input Helpers ===

/**
 * @brief Reads a line of text from stdin into a buffer.
 *
 * Removes the newline character if present and null-terminates the result.
 *
 * @param buffer Pointer to the destination buffer.
 * @param size Size of the buffer.
 * @return READ_OK on success, READ_ERROR on input failure.
 */
ReadStatus read_line(char *buffer, size_t size);

/**
 * @brief Reads a password from stdin without echoing it (if supported by the terminal).
 *
 * @param buffer Pointer to the destination buffer.
 * @param size Size of the buffer.
 * @return Pointer to the buffer, or NULL on failure.
 */
char *read_password(char *buffer, size_t size);

/**
 * @brief Prompts the user with a yes/no question and reads the response.
 *
 * @param prompt Text to display (e.g., "Continue?").
 * @return true if user answered yes, false otherwise.
 */
bool ask_yes_no(const char *prompt);

// === String Manipulation ===

/**
 * @brief Removes a trailing newline character from a string if present.
 *
 * @param str The string to modify.
 */
void remove_newline(char *str);

/**
 * @brief Removes leading and trailing whitespace from a string in place.
 *
 * @param str The string to trim.
 */
void trim_whitespace(char *str);

/**
 * @brief Converts all ASCII characters in a string to lowercase.
 *
 * @param str The string to convert.
 */
void to_lowercase(char *str);

/**
 * @brief Compares two strings case-insensitively.
 *
 * @param a First string.
 * @param b Second string.
 * @return 0 if equal (case-insensitive), otherwise a non-zero value.
 */
int str_casecmp(const char *a, const char *b);

// === Buffer & Memory ===

/**
 * @brief Securely zeroes out a buffer to erase sensitive data.
 *
 * @param buffer The buffer to zero out.
 * @param size The number of bytes to zero.
 */
void secure_zero(char *buffer, size_t size);

/**
 * @brief Copies a string to a destination buffer safely.
 *
 * Ensures the destination is always null-terminated.
 *
 * @param dest Destination buffer.
 * @param src Source string.
 * @param dest_size Size of the destination buffer.
 */
void safe_strcpy(char *dest, const char *src, size_t dest_size);

// === Miscellaneous ===

/**
 * @brief Waits for the user to press Enter.
 *
 * Useful for pausing the program after a message.
 */
void press_enter_to_continue(void);

/**
 * @brief Gets the file path used to store the encrypted vault.
 *
 * @return Path to the vault file as a constant string.
 */
const char *get_vault_file_path(void);

/**
 * @brief Checks whether the encrypted vault file exists on disk.
 *
 * @return true if the vault file exists, false otherwise.
 */
static bool vault_file_exists(void);

#endif // UTILS_H
