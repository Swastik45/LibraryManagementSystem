#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/management.h"
#include "../include/structs.h"

void management() {
    int choice;
    FILE *tempFile;
    Book b;
    Member m;
    char tempSearch[100];
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
        case 1: // ADD BOOK (binary)
            {
                int nextId = 1;
                FILE *fb = fopen("books.dat", "rb");
                if (fb) {
                    fseek(fb, 0, SEEK_END);
                    long sz = ftell(fb);
                    if (sz > 0) nextId = (int)(sz / sizeof(Book)) + 1;
                    fclose(fb);
                }

                FILE *out = fopen("books.dat", "ab");
                if (!out) {
                    printf("Error opening books.dat for writing!\n");
                    break;
                }

                memset(&b, 0, sizeof(Book));
                b.id = nextId;

                printf("Enter Book Title: ");
                fgets(b.title, sizeof(b.title), stdin);
                b.title[strcspn(b.title, "\n")] = 0;

                printf("Enter Book Author: ");
                fgets(b.author, sizeof(b.author), stdin);
                b.author[strcspn(b.author, "\n")] = 0;

                fwrite(&b, sizeof(b), 1, out);
                fclose(out);
                printf("Book added successfully! (ID=%d)\n", b.id);
            }
            break;

        case 2: // REMOVE BOOK (binary)
            {
                FILE *fb = fopen("books.dat", "rb");
                if (!fb) { printf("No books record found.\n"); break; }
                printf("Enter Book Title to Remove: ");
                fgets(tempSearch, sizeof(tempSearch), stdin);
                tempSearch[strcspn(tempSearch, "\n")] = 0;

                tempFile = fopen("temp_books.dat", "wb");
                if (!tempFile) { fclose(fb); printf("Error creating temporary file!\n"); break; }

                found = 0;
                Book rb;
                while (fread(&rb, sizeof(rb), 1, fb) == 1) {
                    if (strcmp(rb.title, tempSearch) == 0) { found = 1; continue; }
                    fwrite(&rb, sizeof(rb), 1, tempFile);
                }

                fclose(fb);
                fclose(tempFile);

                if (found) {
                    remove("books.dat");
                    rename("temp_books.dat", "books.dat");
                    printf("Book '%s' removed successfully!\n", tempSearch);
                } else {
                    remove("temp_books.dat");
                    printf("Book not found.\n");
                }
            }
            break;

        case 3: // ADD MEMBER (binary)
            {
                FILE *fm = fopen("members.dat", "ab");
                if (!fm) { printf("Error opening members.dat for writing!\n"); break; }
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

                strncpy(m.contact, contact, sizeof(m.contact));
                fwrite(&m, sizeof(m), 1, fm);
                fclose(fm);
                printf("Member added successfully!\n");
            }
            break;

        case 4: // REMOVE MEMBER (binary)
            {
                FILE *fm = fopen("members.dat", "rb");
                if (!fm) { printf("No members record found.\n"); break; }

                printf("Enter Member ID to Remove: ");
                fgets(tempSearch, sizeof(tempSearch), stdin);
                tempSearch[strcspn(tempSearch, "\n")] = 0;

                tempFile = fopen("temp_members.dat", "wb");
                if (!tempFile) { fclose(fm); printf("Error creating temporary file!\n"); break; }

                found = 0;
                Member rm;
                while (fread(&rm, sizeof(rm), 1, fm) == 1) {
                    if (strcmp(rm.id, tempSearch) == 0) { found = 1; continue; }
                    fwrite(&rm, sizeof(rm), 1, tempFile);
                }

                fclose(fm);
                fclose(tempFile);

                if (found) {
                    remove("members.dat");
                    rename("temp_members.dat", "members.dat");
                    printf("Member with ID '%s' removed successfully!\n", tempSearch);
                } else {
                    remove("temp_members.dat");
                    printf("Member ID not found.\n");
                }
            }
            break;

        default:
            printf("Invalid choice! Please select 1-4.\n");
    }
}
