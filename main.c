//
// Created by Pablo RD on 5/6/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "password.h"
#include "utils.h"
#include "crypto.h"

int main(void) {
    printf("🔐 Welcome to Password Manager 🔐\n First, enter your Master Password to continue:\n");

    bool is_new_password = false;
    const PasswordStatus auth_status = authenticate_master_password(&is_new_password);

    if (auth_status != PASSWORD_OK) {
        if (auth_status == PASSWORD_ERR_STORAGE) {
            fprintf(stderr, "❌ ERROR: Failed to read/write to the vault file. \n Check the error with the admin.\n");
        }
        if (auth_status == PASSWORD_ERR_CRYPTO) {
            fprintf(
                stderr, "❌ ERROR: Failed to perform cryptographic operations. \n Check the error with the admin.\n");
        }
        if (auth_status == PASSWORD_ERR_HASH_MISMATCH) {
            fprintf(stderr, "🚫 Nope... Master Password is incorrect. Try again.\n");
        }

        press_enter_to_continue();

        return EXIT_FAILURE;
    }

    if (is_new_password) {
        printf("🙉 First timer! \n ✅ New Master Password set, welcome!\n");
        press_enter_to_continue();
    } else {
        printf("🔓 You're in, welcome again! 🤟 \n\n Loading your data... 🔎 \n");
        sleep(2);
    }

    // Check menu options
    while (true) {
        // loops until user chooses to quit
        show_menu();

        char option[MENU_INPUT_BUFFER_SIZE];
        if (read_line(option, sizeof(option)) != READ_OK) {
            fprintf(stderr, "❌ ERROR: Failed to read input. \n Check the error with the admin.\n");
            press_enter_to_continue();

            return EXIT_FAILURE;
        }

        trim_whitespace(option);
        to_lowercase(option);

        if (strlen(option) != 1) {
            printf("⚠️ Invalid option. Please try again.\n");
            sleep(2);

            continue;
        }

        char choice = option[0];

        if (choice == 'q') {
            printf("👋 Ok, see you next time!\n");
            sleep(2);

            break;
        }
        //  TODO: Add menu options
        handle_menu_option(choice);
    }

    return EXIT_SUCCESS;
}
