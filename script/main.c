#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/management.h"
#include "../include/issuereturn.h"
#include "../include/search.h"
#include "../include/fine.h"
#include "../include/display.h"

// Clears the entire terminal window cleanly using standard ANSI escape codes
void clear_screen() {
    // \e[1H moves cursor to top left, \e[2J clears the screen
    printf("\033[1H\033[2J"); 
    fflush(stdout);
}

void clean_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void press_any_key() {
    printf("\nPress [Enter] to return to the Main Menu...");
    getchar(); 
}

int main()
{
    char username[50], password[50];
    int choice;
    int system_running = 1;

    while (system_running)
    {
        clear_screen(); // Clear terminal before showing Login
        printf("=======================================\n");
        printf("      LIBRARY MANAGEMENT SYSTEM        \n");
        printf("=======================================\n");
        printf("\n--- LOGIN SCREEN (Enter 0 as Username to Exit) ---\n");
        printf("Enter Username: ");
        if (scanf("%49s", username) != 1) {
            clean_stdin();
            continue;
        }

        if (strcmp(username, "0") == 0) {
            clear_screen();
            printf("\nExiting System. Goodbye!\n");
            break;
        }

        printf("Enter Password: ");
        scanf("%49s", password);
        clean_stdin(); 

        if (strcmp(username, "admin") == 0 && strcmp(password, "password123") == 0)
        {
            printf("\n[Success] Access Granted! Welcome, Admin.\n");
            
            int logged_in = 1;
            while (logged_in)
            {
                clear_screen(); // Clear terminal before showing the Main Menu layout
                printf("====================================\n");
                printf("             MAIN MENU              \n");
                printf("====================================\n");
                printf("1. Management (Books & Members)\n");
                printf("2. Issue & Return Operations\n");
                printf("3. Search System\n");
                printf("4. Fine & Charges\n");
                printf("5. Display All Records & Status\n");
                printf("6. Logout\n");
                printf("0. Exit Entire Application\n");
                printf("------------------------------------\n");
                printf("Enter choice: ");
                
                if (scanf("%d", &choice) != 1) {
                    printf("\nInvalid input! Please enter a number.\n");
                    clean_stdin();
                    press_any_key();
                    continue;
                }
                clean_stdin(); 

                // Clear screen right before showing the output of the chosen function
                if (choice >= 1 && choice <= 5) {
                    clear_screen(); 
                }

                switch (choice)
                {
                    case 1:
                        management();
                        press_any_key(); 
                        break;
                    case 2:
                        issuereturn();
                        press_any_key(); 
                        break;
                    case 3:
                        search();
                        press_any_key(); 
                        break;
                    case 4:
                        fine();
                        press_any_key(); 
                        break;
                    case 5:
                        display();
                        press_any_key(); 
                        break;
                    case 6:
                        printf("\nLogging out...\n");
                        logged_in = 0; 
                        break;
                    case 0:
                        printf("\nShutting down system safely...\n");
                        logged_in = 0;
                        system_running = 0; 
                        break;
                    default:
                        printf("\n[Error] Invalid choice! Select between 0 and 6.\n");
                        press_any_key(); 
                }
            }
        }
        else
        {
            printf("\n[Access Denied] Incorrect username or password.\n");
            press_any_key(); // Pause so they can read the error before login screen wipes
        }
    }

    return 0;
}