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

// === Menu ===

void show_menu(void) {
    printf("\n===== Vault Menu =====\n");
    printf("L - List all entries (TODO)\n");
    printf("A - Add a new entry (TODO)\n");
    printf("S - Search entries (TODO)\n");
    printf("D - Delete an entry (TODO)\n");
    printf("Q - Quit\n");
    printf("Select an option: ");
}

void handle_menu_option(char option, Vault *vault) {
    switch (option) {
        case 'l':
            list_services(vault);
            press_enter_to_continue();
            break;
        case 'a':
            VaultEntry entry = {0};

            get_vault_entry(&entry);
            if (add_vault_entry(vault, &entry)) {
                StorageStatus save_status = save_vault(vault);
                if (save_status != STORAGE_OK) {
                    fprintf(
                        stderr,
                        "❌ ERROR: Failed to save entry on the vault file. \n Check the error with the admin.\n");

                    press_enter_to_continue();
                }
                printf("✅ Entry added successfully!\n");
                printf("➕ 🔸 %d. Service: %s\n", vault->count, vault->entries[vault->count - 1].service);
                press_enter_to_continue();
            } else {
                printf("❌ Failed to add entry. Please, try again.\n");
                press_enter_to_continue();
            }

            break;
        case 's':
            printf("🔍 Searching entries is not implemented yet.\n");
            break;
        case 'd':
            printf("🗑️ Deleting entries is not implemented yet.\n");
            break;
        default:
            printf("⚠️ Invalid option. Please, try again.\n");
            sleep(2);

            break;
    }
}

void list_services(const Vault *vault) {
    printf("🔐 Stored services passwords:\n");

    if (vault->count == 0) {
        printf("🙂‍↔️ No passwords saved yet. Add a new password with the 'a' option.\n");

        return;
    }

    for (int i = 0; i < vault->count; ++i) {
        printf("-------------------------------------------------\n");
        printf("🔸 %d. Service: %s\n", i + 1, vault->entries[i].service);
    }
    printf("-------------------------------------------------\n");
    printf("Press the number of the service you want to view, or 'q' to quit.\n");
}

// === Miscellaneous ===

void press_enter_to_continue(void) {
    printf("Press Enter to continue...");
    while (getchar() != '\n') {
        ; // discard
    }
    // Clear screen
    printf("\033[2J\033[H");
    fflush(stdout);
}

const char *get_vault_file_path(void) {
    static char path[PATH_MAX];

    const char *home = getenv("HOME"); // home in Unix-like systems (macOS/Linux)
    if (home == NULL) {
        return NULL;
    }

    snprintf(path, sizeof(path), "%s/.vault.dat", home); // hidden file

    return path;
}


bool vault_file_exists(void) {
    struct stat st; // struct to store file information

    return stat(get_vault_file_path(), &st) == 0;
}
