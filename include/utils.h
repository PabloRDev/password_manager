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

// Read a line of text from stdin into a buffer (null-terminated, no newline)
ReadStatus read_line(char *buffer, size_t size);

// Read a password without showing it to the terminal (if possible)
char *read_password(char *buffer, size_t size);

// Prompt the user with a yes/no question (returns true for yes)
bool ask_yes_no(const char *prompt);

// === String Manipulation ===

// Remove trailing newline character from a string
void remove_newline(char *str);

// Trim leading and trailing whitespace in place
void trim_whitespace(char *str);

// Convert a string to lowercase (ASCII only)
void to_lowercase(char *str);

// Compare two strings case-insensitively (returns 0 if equal)
int str_casecmp(const char *a, const char *b);

// === Buffer & Memory ===

// Clear the content of a char buffer securely
void secure_zero(char *buffer, size_t size);

// Safely copy one string to another (like strncpy, but always null-terminates)
void safe_strcpy(char *dest, const char *src, size_t dest_size);

// === Miscellaneous ===

// Pause and wait for the user to press Enter
void press_enter_to_continue(void);

// Get the file path for the encrypted vault file
const char *get_vault_file_path(void);

#endif // UTILS_H
