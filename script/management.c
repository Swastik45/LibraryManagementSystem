#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/management.h"
#include "../include/structs.h"

/* ── ANSI Dynamic Color Scheme ────────────────────────────────────── */
#define CLR_RESET   "\x1b[0m"
#define CLR_MUTED   "\x1b[90m"   // Dark Gray Borders/Brackets
#define CLR_TITLE   "\x1b[1;36m" // Bold Cyan Subheadings
#define CLR_ACCENT  "\x1b[1;33m" // Bold Yellow Options
#define CLR_DATA    "\x1b[37m"   // Soft White User Input Text
#define CLR_SUCCESS "\x1b[1;32m" // Green Messages
#define CLR_ERROR   "\x1b[1;31m" // Red Messages

void management() {
    int choice;
    FILE *tempFile;
    Book b;
    Member m;
    char tempSearch[100];
    int found = 0;

    // Submenu Core Header Layout
    printf("\n%s┌───────────────────────────────────────────────────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
    printf("%s│                    🛠  REGISTRY MANAGEMENT ENGINE                  │%s\n", CLR_TITLE, CLR_RESET);
    printf("%s├───────────────────────────────────────────────────────────────────┤%s\n", CLR_MUTED, CLR_RESET);
    printf("  %s[%s1%s]%s Catalog New Book %s────────%s %s[%s3%s]%s Register New Member\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA, CLR_MUTED, CLR_RESET, CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA);
    printf("  %s[%s2%s]%s De-index (Remove) Book %s──%s %s[%s4%s]%s Revoke (Remove) Member\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA, CLR_MUTED, CLR_RESET, CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA);
    printf("%s└───────────────────────────────────────────────────────────────────┘%s\n\n", CLR_MUTED, CLR_RESET);

    printf("  %s⚡ Select Management Action >> %s", CLR_TITLE, CLR_RESET);
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); // Clean standard buffer
        printf("\n  %s✘ ERROR:%s Invalid input format.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
        return;
    }
    getchar(); // Consume remaining trailing newline

    switch(choice) {
        case 1: // ── ADD BOOK (binary tracking) ──
            {
                int nextId = 1;
                FILE *fb = fopen("books.dat", "rb");
                if (fb) {
                    // Robust tracking strategy: read to find the absolute highest active ID
                    Book trackingBook;
                    while (fread(&trackingBook, sizeof(Book), 1, fb)) {
                        if (trackingBook.id >= nextId) {
                            nextId = trackingBook.id + 1;
                        }
                    }
                    fclose(fb);
                }

                FILE *out = fopen("books.dat", "ab");
                if (!out) {
                    printf("\n  %s✘ SYSTEM FAULT:%s Unable to open books database workspace.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
                    break;
                }

                memset(&b, 0, sizeof(Book));
                b.id = nextId;

                printf("\n  %s◆ ENTER METADATA FOR ID %s[%d]%s\n", CLR_TITLE, CLR_ACCENT, b.id, CLR_RESET);
                printf("  %s├─ Title: %s", CLR_MUTED, CLR_DATA);
                fgets(b.title, sizeof(b.title), stdin);
                b.title[strcspn(b.title, "\n")] = 0;

                printf("  %s└─ Author Name: %s", CLR_MUTED, CLR_DATA);
                fgets(b.author, sizeof(b.author), stdin);
                b.author[strcspn(b.author, "\n")] = 0;

                fwrite(&b, sizeof(Book), 1, out);
                fclose(out);
                
                printf("\n  %s✔ SUCCESS:%s New title indexed into system registry (ID: %d).%s\n", CLR_SUCCESS, CLR_RESET, b.id, CLR_RESET);
            }
            break;

        case 2: // ── REMOVE BOOK (binary deletion) ──
            {
                FILE *fb = fopen("books.dat", "rb");
                if (!fb) { 
                    printf("\n  %s✘ EXCEPTION:%s Target data storage configuration missing.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET); 
                    break; 
                }
                
                printf("\n  %s◆ REMOVE BOOK DATA%s\n", CLR_TITLE, CLR_RESET);
                printf("  %s└─ Enter Exact Book Title to Purge: %s", CLR_MUTED, CLR_DATA);
                fgets(tempSearch, sizeof(tempSearch), stdin);
                tempSearch[strcspn(tempSearch, "\n")] = 0;

                tempFile = fopen("temp_books.dat", "wb");
                if (!tempFile) { 
                    fclose(fb); 
                    printf("\n  %s✘ CRITICAL:%s Buffer Allocation Error dynamically streaming filesystem.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET); 
                    break; 
                }

                found = 0;
                Book rb;
                while (fread(&rb, sizeof(Book), 1, fb) == 1) {
                    if (strcmp(rb.title, tempSearch) == 0) { 
                        found = 1; 
                        continue; // Skip writing the book to match removal criteria
                    }
                    fwrite(&rb, sizeof(Book), 1, tempFile);
                }

                fclose(fb);
                fclose(tempFile);

                if (found) {
                    remove("books.dat");
                    rename("temp_books.dat", "books.dat");
                    printf("\n  %s✔ SUCCESS:%s Target record '%s' de-indexed without issues.%s\n", CLR_SUCCESS, CLR_RESET, tempSearch, CLR_RESET);
                } else {
                    remove("temp_books.dat");
                    printf("\n  %s⚠ RECORD MATCH FAILURE:%s Specified book title does not exist within the system.%s\n", CLR_ACCENT, CLR_RESET, CLR_RESET);
                }
            }
            break;

        case 3: // ── REGISTER MEMBER (binary tracking) ──
            {
                FILE *fm = fopen("members.dat", "ab");
                if (!fm) { 
                    printf("\n  %s✘ SYSTEM FAULT:%s Unable to locate membership access records.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET); 
                    break; 
                }
                
                memset(&m, 0, sizeof(Member));
                printf("\n  %s◆ PROFILE REGISTRATION SUBSYSTEM%s\n", CLR_TITLE, CLR_RESET);
                printf("  %s├─ Assignment Unique Card ID: %s", CLR_MUTED, CLR_DATA);
                fgets(m.id, sizeof(m.id), stdin);
                m.id[strcspn(m.id, "\n")] = 0;

                printf("  %s├─ Full Legal Name: %s", CLR_MUTED, CLR_DATA);
                fgets(m.name, sizeof(m.name), stdin);
                m.name[strcspn(m.name, "\n")] = 0;

                char contact[100] = "";
                printf("  %s└─ Primary Digital Contact (Phone/Email): %s", CLR_MUTED, CLR_DATA);
                fgets(contact, sizeof(contact), stdin);
                contact[strcspn(contact, "\n")] = 0;

                strncpy(m.contact, contact, sizeof(m.contact) - 1);
                
                fwrite(&m, sizeof(Member), 1, fm);
                fclose(fm);
                
                printf("\n  %s✔ SUCCESS:%s Account node created securely for Operator ID %s.%s\n", CLR_SUCCESS, CLR_RESET, m.id, CLR_RESET);
            }
            break;

        case 4: // ── REVOKE MEMBER (binary deletion) ──
            {
                FILE *fm = fopen("members.dat", "rb");
                if (!fm) { 
                    printf("\n  %s✘ EXCEPTION:%s Member database architecture array offline.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET); 
                    break; 
                }

                printf("\n  %s◆ PROFILE DE-PROVISIONING SYSTEM%s\n", CLR_TITLE, CLR_RESET);
                printf("  %s└─ Enter Target Member ID to Revoke: %s", CLR_MUTED, CLR_DATA);
                fgets(tempSearch, sizeof(tempSearch), stdin);
                tempSearch[strcspn(tempSearch, "\n")] = 0;

                tempFile = fopen("temp_members.dat", "wb");
                if (!tempFile) { 
                    fclose(fm); 
                    printf("\n  %s✘ CRITICAL:%s Buffer allocation failure writing clean sector tracks.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET); 
                    break; 
                }

                found = 0;
                Member rm;
                while (fread(&rm, sizeof(Member), 1, fm) == 1) {
                    if (strcmp(rm.id, tempSearch) == 0) { 
                        found = 1; 
                        continue; 
                    }
                    fwrite(&rm, sizeof(Member), 1, tempFile);
                }

                fclose(fm);
                fclose(tempFile);

                if (found) {
                    remove("members.dat");
                    rename("temp_members.dat", "members.dat");
                    printf("\n  %s✔ SUCCESS:%s Database records adjusted. Revoked access profile for ID '%s'.%s\n", CLR_SUCCESS, CLR_RESET, tempSearch, CLR_RESET);
                } else {
                    remove("temp_members.dat");
                    printf("\n  %s⚠ IDENTIFICATION MATCH FAILURE:%s Member card ID not found in current sector.%s\n", CLR_ACCENT, CLR_RESET, CLR_RESET);
                }
            }
            break;

        default:
            printf("\n  %s✘ SELECTION OUT OF BOUNDS:%s Please input a choice index strictly mapping between 1 and 4.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
    }
}