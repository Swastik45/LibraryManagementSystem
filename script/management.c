#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/management.h"

// Struct definitions
struct Book {
    char title[100];
    char author[100];
};

struct Member {
    char name[100];
    char id[20];
};

// Function declaration
void management() {
    int choice;
    FILE *file, *tempFile;
    struct Book b;
    struct Member m;
    char tempSearch[100];
    char line[250];
    int found = 0;

    printf("\n--- Management System Menu ---\n");
    printf("1. Add Book\n");
    printf("2. Remove Book\n");
    printf("3. Add Member\n");
    printf("4. Remove Member\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar(); 

    switch(choice) {
        case 1: // ADD BOOK (write as ID=...;Title=...;Author=...)
            ;
            int nextId = 1;
            file = fopen("books.txt", "r");
            if (file) {
                while (fgets(line, sizeof(line), file)) {
                    if (strlen(line) > 1) nextId++;
                }
                fclose(file);
            }

            file = fopen("books.txt", "a");
            if (file == NULL) {
                printf("Error opening file!\n");
                return;
            }
            printf("Enter Book Title: ");
            fgets(b.title, sizeof(b.title), stdin);
            b.title[strcspn(b.title, "\n")] = 0; // Remove trailing newline

            printf("Enter Book Author: ");
            fgets(b.author, sizeof(b.author), stdin);
            b.author[strcspn(b.author, "\n")] = 0;

            fprintf(file, "ID=%d;Title=%s;Author=%s\n", nextId, b.title, b.author);
            fclose(file);
            printf("Book added successfully!\n");
            break;

        case 2: // REMOVE BOOK
            file = fopen("books.txt", "r");
            if (file == NULL) {
                printf("No books record found.\n");
                return;
            }

            printf("Enter Book Title to Remove: ");
            fgets(tempSearch, sizeof(tempSearch), stdin);
            tempSearch[strcspn(tempSearch, "\n")] = 0; 

            tempFile = fopen("temp.txt", "w");
            if (tempFile == NULL) {
                printf("Error creating temporary file!\n");
                fclose(file);
                return;
            }

            found = 0;
            while (fgets(line, sizeof(line), file)) {
                char bid[32] = "", btitle[200] = "", bauthor[200] = "";
                if (sscanf(line, "ID=%31[^;];Title=%199[^;];Author=%199[^\\n]", bid, btitle, bauthor) >= 2) {
                    if (strcmp(btitle, tempSearch) == 0) {
                        found = 1;
                        continue; /* skip writing this line */
                    }
                }
                /* fallback: if not parseable, keep previous substring check */
                if (strstr(line, tempSearch) == NULL) {
                    fputs(line, tempFile);
                } else {
                    found = 1;
                }
            }
            fclose(file);
            fclose(tempFile);

            // Replace the old file with the updated temporary file
            if (found) {
                remove("books.txt");
                rename("temp.txt", "books.txt");
                printf("Book '%s' removed successfully!\n", tempSearch);
            } else {
                remove("temp.txt"); // Clean up temp file if nothing changed
                printf("Book not found.\n");
            }
            break;

        case 3: // ADD MEMBER
            file = fopen("members.txt", "a");
            if (file == NULL) {
                printf("Error opening file!\n");
                return;
            }
            printf("Enter Member Name: ");
            fgets(m.name, sizeof(m.name), stdin);
            m.name[strcspn(m.name, "\n")] = 0;

            printf("Enter Member ID: ");
            fgets(m.id, sizeof(m.id), stdin);
            m.id[strcspn(m.id, "\n")] = 0;

            char contact[100] = "";
            printf("Enter Member Contact (phone/email): ");
            fgets(contact, sizeof(contact), stdin);
            contact[strcspn(contact, "\n")] = 0;

            fprintf(file, "ID=%s;Name=%s;Contact=%s\n", m.id, m.name, contact);
            fclose(file);
            printf("Member added successfully!\n");
            break;

        case 4: // REMOVE MEMBER
            file = fopen("members.txt", "r");
            if (file == NULL) {
                printf("No members record found.\n");
                return;
            }

            printf("Enter Member ID to Remove: ");
            fgets(tempSearch, sizeof(tempSearch), stdin);
            tempSearch[strcspn(tempSearch, "\n")] = 0;

            tempFile = fopen("temp.txt", "w");
            if (tempFile == NULL) {
                printf("Error creating temporary file!\n");
                fclose(file);
                return;
            }

            found = 0;
            while (fgets(line, sizeof(line), file)) {
                char mid[64] = "", mname[200] = "", mcontact[200] = "";
                if (sscanf(line, "ID=%63[^;];Name=%199[^;];Contact=%199[^\\n]", mid, mname, mcontact) >= 1) {
                    if (strcmp(mid, tempSearch) == 0) {
                        found = 1;
                        continue; /* skip writing this line */
                    }
                }
                if (strstr(line, tempSearch) == NULL) {
                    fputs(line, tempFile);
                } else {
                    found = 1;
                }
            }
            fclose(file);
            fclose(tempFile);

            if (found) {
                remove("members.txt");
                rename("temp.txt", "members.txt");
                printf("Member with ID '%s' removed successfully!\n", tempSearch);
            } else {
                remove("temp.txt");
                printf("Member ID not found.\n");
            }
            break;

        default:
            printf("Invalid choice! Please select 1-4.\n");
    }
}
