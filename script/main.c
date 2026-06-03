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
    // Standard system-independent screen clear or fallback spacing
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clean_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void press_enter(const char *msg) {
    if (msg) {
        printf("\n  %s", msg);
    } else {
        printf("\n  Press [Enter] to return to the menu...");
    }
    getchar();
}

/* ── Banner & menus ────────────────────────────────────────────────── */

static void print_banner(const char *subtitle) {
    printf("\n--- LIBRARY MANAGEMENT SYSTEM v%s ---\n", VERSION);
    if (subtitle) {
        printf("  %s\n", subtitle);
    }
    printf("---------------------------------------\n\n");
}

static void print_login_screen() {
    clear_screen();
    print_banner("Administrator Access");
}

static void print_main_menu(const char *username) {
    clear_screen();
    printf("\n--- LIBRARY MANAGEMENT SYSTEM ---\n");
    printf("  Logged in: %s\n", username);
    printf("---------------------------------\n\n");

    printf("  1. Management (Books & Members)\n");
    printf("  2. Issue & Return (Loan operations)\n");
    printf("  3. Search (Find books or members)\n");
    printf("  4. Fines & Charges (Overdue penalties)\n");
    printf("  5. Display Records (Full system status)\n\n");
    printf("  6. Logout\n");
    printf("  0. Exit\n");
    printf("---------------------------------\n\n");

    printf("  Choice: ");
}

/* ── Feedback helpers ──────────────────────────────────────────────── */

static void print_success(const char *msg) {
    printf("\n  SUCCESS: %s\n", msg);
}

static void print_error(const char *msg) {
    printf("\n  ERROR: %s\n", msg);
}

/* ── Main ──────────────────────────────────────────────────────────── */

int main() {
    char username[50], password[50];
    int choice;
    int system_running = 1;

    while (system_running) {
        print_login_screen();

        printf("  Enter username (or 0 to quit): ");
        if (scanf("%49s", username) != 1) { clean_stdin(); continue; }
        clean_stdin();

        if (strcmp(username, "0") == 0) {
            clear_screen();
            printf("\n  Goodbye!\n\n");
            break;
        }

        printf("  Enter password: ");
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

            if (choice >= 1 && choice <= 5) {
                clear_screen();
            }

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