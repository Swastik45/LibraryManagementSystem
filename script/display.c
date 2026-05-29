#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/display.h"

void display()
{
    char line[512];

    // --- 1. Books Catalog ---
    printf("\n======================================================\n");
    printf("                    BOOKS CATALOG                     \n");
    printf("======================================================\n");
    
    FILE *fb = fopen("books.txt", "r");
    if (fb) {
        while (fgets(line, sizeof(line), fb)) {
            // Removes trailing newline if present
            line[strcspn(line, "\n")] = 0; 
            
            // Assuming your file has "Title: ... | Author: ..."
            // We print it with a slight indentation for readability
            if (strlen(line) > 0) {
                printf("  %s\n", line);
            }
        }
        fclose(fb);
    } else {
        printf("  No books catalog found.\n");
    }

    // --- 2. Members ---
    printf("\n======================================================\n");
    printf("                       MEMBERS                        \n");
    printf("======================================================\n");
    
    FILE *fm = fopen("members.txt", "r");
    if (fm) {
        while (fgets(line, sizeof(line), fm)) {
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) > 0) {
                printf("  %s\n", line);
            }
        }
        fclose(fm);
    } else {
        printf("  No members record found.\n");
    }

    // --- 3. Issued Records (Parsed and Formatted as a Table) ---
    printf("\n====================================================================================\n");
    printf("                                   ISSUED RECORDS                                   \n");
    printf("====================================================================================\n");
    // %-25s means left-align with 25 spaces width
    printf("  %-25s %-12s %-20s\n", "Book Title", "Member ID", "Issued Date/Time");
    printf("  ----------------------------------------------------------------------------------\n");

    FILE *fi = fopen("issued.txt", "r");
    if (fi) {
        while (fgets(line, sizeof(line), fi)) {
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) == 0) continue;

            // Variables to hold parsed data
            char title[100] = "";
            char id[50] = "";
            long int timestamp = 0;

            // Parsing: Title=The Alchimest;ID=1;Time=1780033928
            // sscanf searches for the keys and extracts the values dynamically
            if (sscanf(line, "Title=%[^;];ID=%[^;];Time=%ld", title, id, &timestamp) == 3) {
                
                // Convert UNIX timestamp to a readable local date/time string
                time_t rawtime = (time_t)timestamp;
                struct tm *timeinfo = localtime(&rawtime);
                char time_buffer[26];
                
                if (timeinfo != NULL) {
                    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
                } else {
                    strcpy(time_buffer, "Invalid Time");
                }

                // Print perfectly aligned columns
                printf("  %-25s %-12s %-20s\n", title, id, time_buffer);
            } else {
                // Fallback in case a line doesn't match the format string
                printf("  Raw Record: %s\n", line);
            }
        }
        fclose(fi);
    } else {
        printf("  No issued records found.\n");
    }
    printf("====================================================================================\n\n");
}