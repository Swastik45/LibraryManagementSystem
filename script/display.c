#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/display.h"

void display() {
    char line[512];
    int book_count = 0;
    int member_count = 0;
    int loan_count = 0;

    printf("\n=== LIBRARY RECORDS ===\n");

    /* ── 1. BOOKS CATALOG ─────────────────────────────────────────── */
    printf("\n-- BOOKS --\n");
    FILE *fb = fopen("books.txt", "r");
    if (fb) {
        while (fgets(line, sizeof(line), fb)) {
            line[strcspn(line, "\n")] = 0; // Remove trailing newline
            if (strlen(line) == 0) continue;
            
            // Simplest format: just print the line as a basic string
            printf("  %s\n", line);
            book_count++;
        }
        fclose(fb);
    }
    if (book_count == 0) printf("  (No books found)\n");

    /* ── 2. MEMBERS ───────────────────────────────────────────────── */
    printf("\n-- MEMBERS --\n");
    FILE *fm = fopen("members.txt", "r");
    if (fm) {
        while (fgets(line, sizeof(line), fm)) {
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) == 0) continue;

            printf("  %s\n", line);
            member_count++;
        }
        fclose(fm);
    }
    if (member_count == 0) printf("  (No members found)\n");

    /* ── 3. ISSUED RECORDS ────────────────────────────────────────── */
    printf("\n-- ISSUED RECORDS --\n");
    FILE *fi = fopen("issued.txt", "r");
    if (fi) {
        while (fgets(line, sizeof(line), fi)) {
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) == 0) continue;

            printf("  %s\n", line);
            loan_count++;
        }
        fclose(fi);
    }
    if (loan_count == 0) printf("  (No issued records found)\n");

    /* ── Summary ──────────────────────────────────────────────────── */
    printf("\n--------------------------------------------\n");
    printf("Total: %d Books | %d Members | %d Loans\n\n", book_count, member_count, loan_count);
}