#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/search.h"

void search()
{
    int choice;
    char query[200];
    char line[512];

    printf("\n--- SEARCH MENU ---\n");
    printf("1. Search Book\n");
    printf("2. Search Member\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    // Clear the leftover 'Enter' key from scanf
    getchar(); 

    if (choice == 1) {
        FILE *f = fopen("books.txt", "r");
        if (f == NULL) {
            printf("Error: Cannot open books.txt\n");
            return;
        }

        printf("Enter book title (Case-Sensitive): ");
        fgets(query, sizeof(query), stdin);
        query[strcspn(query, "\n")] = '\0'; // Remove the Enter key from string

        printf("\n--- Search Results ---\n");
        while (fgets(line, sizeof(line), f)) {
            // strstr checks if 'query' exists inside 'line'
            if (strstr(line, query) != NULL) {
                printf("Found: %s", line);
            }
        }
        fclose(f);

    } else if (choice == 2) {
        FILE *f = fopen("members.txt", "r");
        if (f == NULL) {
            printf("Error: Cannot open members.txt\n");
            return;
        }

        printf("Enter member name or ID: ");
        fgets(query, sizeof(query), stdin);
        query[strcspn(query, "\n")] = '\0'; 

        printf("\n--- Search Results ---\n");
        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, query) != NULL) {
                printf("Found: %s", line);
            }
        }
        fclose(f);

    } else {
        printf("Invalid choice!\n");
    }
}