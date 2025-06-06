//
// Created by Pablo RD on 5/6/25.
//

#include <stdio.h>
#include <stdlib.h>

#include "password.h"
#include "utils.h"

int main(void) {
    printf("🔐 Welcome to Password Manager\n");

    bool is_new_password = false;
    const PasswordStatus auth_status = authenticate_master_password(&is_new_password);

    if (auth_status != PASSWORD_OK) {
        fprintf(stderr, "❌ Authentication failed (error code: %d)\n", auth_status);

        return EXIT_FAILURE;
    }

    if (is_new_password) {
        printf("✅ New master password created and saved.\n");
    } else {
        printf("🔓 Master password verified.\n");
    }

    // TODO: Load vault, show options, etc.
    printf("🧠 [TODO] Vault functionality goes here...\n");

    press_enter_to_continue();

    return EXIT_SUCCESS;
}
