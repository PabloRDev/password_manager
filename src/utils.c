//
// Created by Pablo RD on 5/6/25.
//
#include "utils.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h> // API to control terminal behavior
#include <unistd.h>
#include <sys/stat.h>

// === Input ===
ReadStatus read_line(char *buffer, size_t size) {
    assert(buffer != NULL);
    if (size == 0) {
        return READ_ERROR;
    }
    // Error or end-of-file reached (fgets -> read line from input into a string)
    if (fgets(buffer, (int) size, stdin) == NULL) {
        return READ_ERROR;
    }

    // REMOVE NEWLINE AFTER USER INPUT
    remove_newline(buffer);

    return READ_OK;
}

char *read_password(char *buffer, size_t size) {
    struct termios term_settings, new_term_settings;

    // PROMPT USER FOR PASSWORD
    printf("Password: ");
    fflush(stdout); // forces output to be written to stdout

    // DISABLE ECHO
    tcgetattr(STDIN_FILENO, &term_settings);
    new_term_settings = term_settings;
    new_term_settings.c_lflag &= ~(ECHO); // disable echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term_settings);

    // READ PASSWORD
    const ReadStatus read_result = read_line(buffer, size);

    // REACTIVATE ECHO
    tcsetattr(STDIN_FILENO, TCSANOW, &term_settings);
    printf("\n");

    return (read_result == READ_OK) ? buffer : NULL;
}

bool ask_yes_no(const char *prompt) {
    assert(prompt != NULL);
    if (prompt[0] == '\0') return false;

    char response[ASK_YES_NO_MAX_PROMPT_LENGTH]; // "y\n\0" or "yes\0"

    // PROMPT UNTIL VALID RESPONSE
    while (1) {
        printf("%s (y/n): ", prompt);
        if (read_line(response, sizeof(response)) != READ_OK) {
            continue;
        }

        trim_whitespace(response);
        to_lowercase(response);
        if (strcmp(response, "y") == 0 || strcmp(response, "yes") == 0) return true;
        if (strcmp(response, "n") == 0 || strcmp(response, "no") == 0) return false;

        printf("Please enter 'y' or 'n'.\n");
    }
}

// === String Manipulation ===

void remove_newline(char *str) {
    if (!str) return;
    size_t len = strlen(str);
    if (len == 0) return;
    if (str[len - 1] == '\n') str[len - 1] = '\0';
}

void trim_whitespace(char *str) {
    if (!str) return;

    // TRIM LEADING (START)
    char *start = str;
    while (*start && isspace((unsigned char) *start)) start++; // isspace -> check if char is whitespace

    if (start != str)
        memmove(str, start, strlen(start) + 1); // memmove -> move data protecting against overlapping addresses

    // TRIM TRAILING (END)
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char) str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
}

void to_lowercase(char *str) {
    if (!str) return;
    for (size_t i = 0; str[i]; i++) {
        str[i] = (char) tolower((unsigned char) str[i]);
    }
}

int str_casecmp(const char *a, const char *b) {
    return strcasecmp(a, b);
}

// === Buffer & Memory ===

void secure_zero(char *buffer, size_t size) {
    if (!buffer) return;

    volatile char *p = buffer; // volatile to prevent compiler from optimizing out the memset
    // CLEARS BUFFER WITH ZEROES
    while (size--) {
        *p++ = 0;
    }
}

void safe_strcpy(char *dest, const char *src, size_t dest_size) {
    if (dest_size == 0) return;
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

// === Miscellaneous ===

void press_enter_to_continue(void) {
    printf("Press Enter to continue...");
    while (getchar() != '\n') {
        ; // discard
    }
}

const char *get_vault_file_path(void) {
    // You can customize this to return a fixed path or dynamically get home directory
    // For simplicity, return a relative file name
    return "vault.dat";
}

bool vault_file_exists(void) {
    struct stat st;

    return stat(get_vault_file_path(), &st) == 0;
}
