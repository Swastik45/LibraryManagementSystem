#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/search.h"
#include "../include/structs.h"

/* Convert string to lowercase for case-insensitive comparison */
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

/* Case-insensitive substring search */
int case_insensitive_search(const char *haystack, const char *needle) {
    char hay[256], ndl[256];
    strncpy(hay, haystack, sizeof(hay) - 1);
    strncpy(ndl, needle, sizeof(ndl) - 1);
    hay[sizeof(hay) - 1] = '\0';
    ndl[sizeof(ndl) - 1] = '\0';
    to_lowercase(hay);
    to_lowercase(ndl);
    return strstr(hay, ndl) != NULL;
}

void search()
{
    int choice;
    char query[200];

    printf("\n--- SEARCH MENU ---\n");
    printf("1. Search Book by Title or Author\n");
    printf("2. Search Member by Name or ID\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    // Clear the leftover 'Enter' key from scanf
    getchar(); 

    if (choice == 1) {
        FILE *f = fopen("books.dat", "rb");
        if (f == NULL) {
            printf("Error: Cannot open books.dat\n");
            return;
        }

        printf("Enter search term (case-insensitive): ");
        fgets(query, sizeof(query), stdin);
        query[strcspn(query, "\n")] = '\0';

        if (strlen(query) == 0) {
            printf("Search term cannot be empty!\n");
            fclose(f);
            return;
        }

        printf("\n+----+-------------------------------------+------------------------------+\n");
        printf("| ID | %-35s | %-28s |\n", "Title", "Author");
        printf("+----+-------------------------------------+------------------------------+\n");

        Book b;
        int found = 0;
        while (fread(&b, sizeof(Book), 1, f)) {
            // Search in both title and author (case-insensitive)
            if (case_insensitive_search(b.title, query) || case_insensitive_search(b.author, query)) {
                printf("| %2d | %-35.35s | %-28.28s |\n", b.id, b.title, b.author);
                found++;
            }
        }
        printf("+----+-------------------------------------+------------------------------+\n");
        
        if (found == 0) {
            printf("No books found matching '%s'\n", query);
        } else {
            printf("Found %d book(s) matching '%s'\n", found, query);
        }
        fclose(f);

    } else if (choice == 2) {
        FILE *f = fopen("members.dat", "rb");
        if (f == NULL) {
            printf("Error: Cannot open members.dat\n");
            return;
        }

        printf("Enter search term (case-insensitive): ");
        fgets(query, sizeof(query), stdin);
        query[strcspn(query, "\n")] = '\0';

        if (strlen(query) == 0) {
            printf("Search term cannot be empty!\n");
            fclose(f);
            return;
        }

        printf("\n+----------+------------------------------+----------------------+\n");
        printf("| %-8s | %-28s | %-20s |\n", "Member ID", "Name", "Contact");
        printf("+----------+------------------------------+----------------------+\n");

        Member m;
        int found = 0;
        while (fread(&m, sizeof(Member), 1, f)) {
            // Search in name and id (case-insensitive)
            if (case_insensitive_search(m.name, query) || case_insensitive_search(m.id, query)) {
                printf("| %-8.8s | %-28.28s | %-20.20s |\n", m.id, m.name, m.contact);
                found++;
            }
        }
        printf("+----------+------------------------------+----------------------+\n");
        
        if (found == 0) {
            printf("No members found matching '%s'\n", query);
        } else {
            printf("Found %d member(s) matching '%s'\n", found, query);
        }
        fclose(f);

    } else {
        printf("Invalid choice!\n");
    }
}