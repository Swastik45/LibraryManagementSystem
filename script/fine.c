#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/fine.h"
#include "../include/structs.h" // Linked to access core data layouts

/* ── ANSI Dynamic Color Scheme ────────────────────────────────────── */
#define CLR_RESET   "\x1b[0m"
#define CLR_MUTED   "\x1b[90m"   // Dark Gray Borders/Brackets
#define CLR_TITLE   "\x1b[1;36m" // Bold Cyan Subheadings
#define CLR_ACCENT  "\x1b[1;33m" // Bold Yellow Highlights
#define CLR_DATA    "\x1b[37m"   // Soft White text
#define CLR_SUCCESS "\x1b[1;32m" // Green Messages
#define CLR_ERROR   "\x1b[1;31m" // Red Messages

void fine() {
    char memberId[50];
    FILE *f = fopen("issued.dat", "rb"); // Fixed to match binary system format
    
    if (!f) {
        printf("\n%s┌───────────────────────────────────────────────────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
        printf("%s│                  💸  FINES & BALANCE TRACKING                     │%s\n", CLR_TITLE, CLR_RESET);
        printf("%s├───────────────────────────────────────────────────────────────────┤%s\n", CLR_MUTED, CLR_RESET);
        printf("  %s⚠ DATABASE OFFLINE:%s No active loan histories exist on disk.%s\n", CLR_ACCENT, CLR_RESET, CLR_RESET);
        printf("%s└───────────────────────────────────────────────────────────────────┘%s\n", CLR_MUTED, CLR_RESET);
        return;
    }

    getchar(); // Clear remaining line buffers from previous menus
    printf("\n%s┌───────────────────────────────────────────────────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
    printf("%s│                  💸  FINES & BALANCE TRACKING                     │%s\n", CLR_TITLE, CLR_RESET);
    printf("%s└───────────────────────────────────────────────────────────────────┘%s\n", CLR_MUTED, CLR_RESET);
    
    printf("\n  %s💳 Enter Operator Card ID to Calculate Fines: %s", CLR_TITLE, CLR_DATA);
    fgets(memberId, sizeof(memberId), stdin);
    memberId[strcspn(memberId, "\n")] = 0;

    long totalFine = 0;
    long active_overdue_books = 0;
    time_t now = time(NULL);
    IssuedRecord rec;

    // Scan through binary stream matching against active ledger nodes
    while (fread(&rec, sizeof(IssuedRecord), 1, f) == 1) {
        if (strcmp(rec.member_id, memberId) == 0) {
            long days = (long)(now - rec.issue_time) / 86400;
            
            // Standard library 14-day clearance frame rule
            if (days > 14) {
                totalFine += (days - 14) * 5; // 5 units accrued daily
                active_overdue_books++;
            }
        }
    }
    fclose(f);

    printf("\n%s─────────────────────────────────────────────────────────────────────%s\n", CLR_MUTED, CLR_RESET);
    if (totalFine > 0) {
        printf("  %s⚠ ACCOUNT SUSPENDED / OVERDUE%s\n", CLR_ERROR, CLR_RESET);
        printf("  %s├─ Profile Reference ID: %s%s%s\n", CLR_MUTED, CLR_DATA, memberId, CLR_RESET);
        printf("  %s├─ Delinquent Records:   %s%ld active item(s)%s\n", CLR_MUTED, CLR_ACCENT, active_overdue_books, CLR_RESET);
        printf("  %s└─ Total Accrued Fine:   %s$%ld.00 USD%s\n", CLR_MUTED, CLR_ERROR, totalFine, CLR_RESET);
    } else {
        printf("  %s✔ STANDING STATUS VERIFIED%s\n", CLR_SUCCESS, CLR_RESET);
        printf("  %s├─ Profile Reference ID: %s%s%s\n", CLR_MUTED, CLR_DATA, memberId, CLR_RESET);
        printf("  %s└─ Account Balance:       %s$0.00 (Clear)%s\n", CLR_MUTED, CLR_SUCCESS, CLR_RESET);
    }
    printf("%s─────────────────────────────────────────────────────────────────────%s\n", CLR_MUTED, CLR_RESET);
}