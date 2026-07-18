#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/management.h"
#include "../include/issuereturn.h"
#include "../include/search.h"
#include "../include/fine.h"
#include "../include/display.h"

#define VERSION "1.0"

/* ── ANSI Dynamic Color Scheme ────────────────────────────────────── */
#define CLR_RESET   "\x1b[0m"
#define CLR_MUTED   "\x1b[90m"   // Dark Gray Borders/Brackets
#define CLR_TITLE   "\x1b[1;36m" // Bold Cyan Primary
#define CLR_ACCENT  "\x1b[1;33m" // Bold Yellow Highlights
#define CLR_DATA    "\x1b[37m"   // Soft White text
#define CLR_SUCCESS "\x1b[1;32m" // Green Messages
#define CLR_ERROR   "\x1b[1;31m" // Red Messages

/* ── Terminal helpers ──────────────────────────────────────────────── */

void clear_screen() {
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
        printf("\n  %s Press %s[Enter]%s to return to the dashboard... ", CLR_MUTED, CLR_ACCENT, CLR_MUTED);
    }
    getchar();
}

/* ── Banner & menus ────────────────────────────────────────────────── */

static void print_banner(const char *subtitle) {
    printf("\n%s┌───────────────────────────────────────────────────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
    printf("%s│             ❖  NEO-LIBRARY MANAGEMENT SYSTEM  v%s  ❖             │%s\n", CLR_TITLE, VERSION, CLR_RESET);
    if (subtitle) {
        printf("%s├───────────────────────────────────────────────────────────────────┤%s\n", CLR_MUTED, CLR_RESET);
        printf("  %s%s%s\n", CLR_ACCENT, subtitle, CLR_RESET);
    }
    printf("%s└───────────────────────────────────────────────────────────────────┘%s\n\n", CLR_MUTED, CLR_RESET);
}

static void print_login_screen() {
    clear_screen();
    print_banner("🔑 ADMINISTRATOR ACCESS REQUIRED");
}

static void print_main_menu(const char *username) {
    clear_screen();
    
    printf("\n%s┌───────────────────────────────────────────────────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
    printf("%s│                   ⚙  CORE ENGINE CONTROL PANEL                    │%s\n", CLR_TITLE, CLR_RESET);
    printf("%s├───────────────────────────────────────────────────────────────────┤%s\n", CLR_MUTED, CLR_RESET);
    printf("  %sActive Operator:%s %s%s%s\n", CLR_MUTED, CLR_RESET, CLR_ACCENT, username, CLR_RESET);
    printf("%s└───────────────────────────────────────────────────────────────────┘%s\n\n", CLR_MUTED, CLR_RESET);

    printf("  %s[%s1%s]%s Management %s───────%s %sBooks & Active Registry\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA, CLR_MUTED, CLR_RESET, CLR_DATA);
    printf("  %s[%s2%s]%s Issue & Return %s──%s %sCheck-out & Return Operations\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA, CLR_MUTED, CLR_RESET, CLR_DATA);
    printf("  %s[%s3%s]%s Search Center %s────%s %sLookup Global Catalogs\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA, CLR_MUTED, CLR_RESET, CLR_DATA);
    printf("  %s[%s4%s]%s Fines & Penalties %s─%s %sOverdue Calculations & Balances\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA, CLR_MUTED, CLR_RESET, CLR_DATA);
    printf("  %s[%s5%s]%s System Display %s───%s %sFull System Records Snapshot\n\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA, CLR_MUTED, CLR_RESET, CLR_DATA);
    
    printf("  %s[%s6%s]%s Sign Out\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA);
    printf("  %s[%s0%s]%s Emergency Terminate\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA);
    printf("%s─────────────────────────────────────────────────────────────────────%s\n\n", CLR_MUTED, CLR_RESET);

    printf("  %s⚡ Select Control Action >> %s", CLR_TITLE, CLR_RESET);
}

/* ── Feedback helpers ──────────────────────────────────────────────── */

static void print_success(const char *msg) {
    printf("\n  %s✔ SUCCESS:%s %s\n", CLR_SUCCESS, CLR_RESET, msg);
}

static void print_error(const char *msg) {
    printf("\n  %s✘ ERROR:%s %s\n", CLR_ERROR, CLR_RESET, msg);
}

/* ── Main ──────────────────────────────────────────────────────────── */

int main() {
    char username[50], password[50];
    int choice;
    int system_running = 1;

    while (system_running) {
        print_login_screen();

        printf("  %s👤 User Identity %s(or 0 to exit)%s: ", CLR_DATA, CLR_MUTED, CLR_RESET);
        if (scanf("%49s", username) != 1) { clean_stdin(); continue; }
        clean_stdin();

        if (strcmp(username, "0") == 0) {
            clear_screen();
            printf("\n  %s⚡ Shutting down UI subsystem safely. Goodbye!%s\n\n", CLR_ACCENT, CLR_RESET);
            break;
        }

        printf("  %s🔑 Passphrase: %s", CLR_DATA, CLR_RESET);
        // Softly mask text input processing
        if (scanf("%49s", password) != 1) { clean_stdin(); continue; }
        clean_stdin();

        /* ── Authentication ────────────────────────────────────────── */
        if (strcmp(username, "admin") != 0 || strcmp(password, "password123") != 0) {
            print_error("Security Check Failure: Invalid Credentials.");
            press_enter("Press [Enter] to cycle security login...");
            continue;
        }

        print_success("Access Granted. Initializing admin workplace...");
        #ifdef _WIN32
            _sleep(800); // Small procedural delay for design depth
        #else
            struct timespec ts = {0, 800000000L}; nanosleep(&ts, NULL);
        #endif

        /* ── Session loop ──────────────────────────────────────────── */
        int logged_in = 1;
        while (logged_in) {
            print_main_menu(username);

            if (scanf("%d", &choice) != 1) {
                clean_stdin();
                print_error("Parse Exception: Input must be a valid option code.");
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
                    printf("\n  %s♻  Clearing active environment session variables... Out.%s\n\n", CLR_ACCENT, CLR_RESET);
                    logged_in = 0;
                    break;
                case 0:
                    clear_screen();
                    printf("\n  %s🔌 Terminating process tree layout. Goodbye!%s\n\n", CLR_ACCENT, CLR_RESET);
                    logged_in = 0;
                    system_running = 0;
                    break;
                default:
                    print_error("Invalid Range: Option code maps out of layout spectrum.");
                    press_enter(NULL);
            }
        }
    }

    return 0;
}