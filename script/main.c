#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/management.h"
#include "../include/issuereturn.h"
#include "../include/search.h"
#include "../include/fine.h"
#include "../include/display.h"

#define VERSION "1.0"

/* ── Terminal helpers ──────────────────────────────────────────────── */

void clear_screen() {
    printf("\033[H\033[2J");
    fflush(stdout);
}

void clean_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void press_enter(const char *msg) {
    if (msg) printf("\n  %s", msg);
    else     printf("\n  Press [Enter] to return to the menu...");
    getchar();
}

/* ── Banner & menus ────────────────────────────────────────────────── */

static void print_banner(const char *subtitle) {
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════╗\n");
    printf("  ║                                                      ║\n");
    printf("  ║     \033[1m📚  LIBRARY MANAGEMENT SYSTEM\033[0m  v%-6s         ║\n", VERSION);
    if (subtitle)
        printf("  ║     \033[2m%-50s\033[0m ║\n", subtitle);
    printf("  ║                                                      ║\n");
    printf("  ╚══════════════════════════════════════════════════════╝\n\n");
}

static void print_login_screen() {
    clear_screen();
    print_banner("Administrator Access");
}

static void print_main_menu(const char *username) {
    clear_screen();
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════╗\n");
    printf("  ║  \033[1m📚  LIBRARY MANAGEMENT SYSTEM\033[0m");
    printf("  \033[2mLogged in: %-8s\033[0m ║\n", username);
    printf("  ╚══════════════════════════════════════════════════════╝\n\n");

    printf("  ┌──────────────────────────────────────────────────────┐\n");
    printf("  │  \033[36m1\033[0m  \033[1mManagement\033[0m          \033[2mBooks & Members\033[0m               │\n");
    printf("  │  \033[36m2\033[0m  \033[1mIssue & Return\033[0m       \033[2mLoan operations\033[0m               │\n");
    printf("  │  \033[36m3\033[0m  \033[1mSearch\033[0m               \033[2mFind books or members\033[0m         │\n");
    printf("  │  \033[36m4\033[0m  \033[1mFines & Charges\033[0m      \033[2mOverdue penalties\033[0m             │\n");
    printf("  │  \033[36m5\033[0m  \033[1mDisplay Records\033[0m      \033[2mFull system status\033[0m            │\n");
    printf("  │                                                      │\n");
    printf("  │  \033[33m6\033[0m  \033[1mLogout\033[0m                                              │\n");
    printf("  │  \033[31m0\033[0m  \033[1mExit\033[0m                                                │\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");

    printf("  Choice:  ");
}

/* ── Feedback helpers ──────────────────────────────────────────────── */

static void print_success(const char *msg) {
    printf("\n  \033[32m╔══════════════════════════════════════════════════════╗\033[0m\n");
    printf(    "  \033[32m║  ✔  %-50s ║\033[0m\n", msg);
    printf(    "  \033[32m╚══════════════════════════════════════════════════════╝\033[0m\n");
}

static void print_error(const char *msg) {
    printf("\n  \033[31m╔══════════════════════════════════════════════════════╗\033[0m\n");
    printf(    "  \033[31m║  ✖  %-50s ║\033[0m\n", msg);
    printf(    "  \033[31m╚══════════════════════════════════════════════════════╝\033[0m\n");
}

/* ── Main ──────────────────────────────────────────────────────────── */

int main() {
    char username[50], password[50];
    int choice;
    int system_running = 1;

    while (system_running) {
        print_login_screen();

        printf("  Enter username \033[2m(or 0 to quit)\033[0m:  ");
        if (scanf("%49s", username) != 1) { clean_stdin(); continue; }
        clean_stdin();

        if (strcmp(username, "0") == 0) {
            clear_screen();
            printf("\n  Goodbye!\n\n");
            break;
        }

        printf("  Enter password:                  ");
        if (scanf("%49s", password) != 1) { clean_stdin(); continue; }
        clean_stdin();

        /* ── Authentication ────────────────────────────────────────── */
        if (strcmp(username, "admin") != 0 || strcmp(password, "password123") != 0) {
            print_error("Access denied. Incorrect username or password.");
            press_enter("Press [Enter] to try again...");
            continue;
        }

        print_success("Access granted. Welcome, Admin.");

        /* ── Session loop ──────────────────────────────────────────── */
        int logged_in = 1;
        while (logged_in) {
            print_main_menu(username);

            if (scanf("%d", &choice) != 1) {
                clean_stdin();
                print_error("Invalid input — please enter a number between 0 and 6.");
                press_enter(NULL);
                continue;
            }
            clean_stdin();

            if (choice >= 1 && choice <= 5) clear_screen();

            switch (choice) {
                case 1:
                    management();
                    press_enter(NULL);
                    break;
                case 2:
                    issuereturn();
                    press_enter(NULL);
                    break;
                case 3:
                    search();
                    press_enter(NULL);
                    break;
                case 4:
                    fine();
                    press_enter(NULL);
                    break;
                case 5:
                    display();
                    press_enter(NULL);
                    break;
                case 6:
                    clear_screen();
                    printf("\n  Logging out...\n\n");
                    logged_in = 0;
                    break;
                case 0:
                    clear_screen();
                    printf("\n  Shutting down safely. Goodbye!\n\n");
                    logged_in = 0;
                    system_running = 0;
                    break;
                default:
                    print_error("Invalid choice — select a number between 0 and 6.");
                    press_enter(NULL);
            }
        }
    }

    return 0;
}