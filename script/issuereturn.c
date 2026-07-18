#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/issuereturn.h"
#include "../include/structs.h"

/* ── ANSI Dynamic Color Scheme ────────────────────────────────────── */
#define CLR_RESET   "\x1b[0m"
#define CLR_MUTED   "\x1b[90m"   // Dark Gray Borders/Brackets
#define CLR_TITLE   "\x1b[1;36m" // Bold Cyan Subheadings
#define CLR_ACCENT  "\x1b[1;33m" // Bold Yellow Highlights
#define CLR_DATA    "\x1b[37m"   // Soft White text
#define CLR_SUCCESS "\x1b[1;32m" // Green Messages
#define CLR_ERROR   "\x1b[1;31m" // Red Messages

void issue_book();
void return_book();

void issuereturn() {
    int choice;

    // Submenu Header Layout
    printf("\n%s┌───────────────────────────────────────────────────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
    printf("%s│                    🔄  LOAN & TRANSACTION ENGINE                  │%s\n", CLR_TITLE, CLR_RESET);
    printf("%s├───────────────────────────────────────────────────────────────────┤%s\n", CLR_MUTED, CLR_RESET);
    printf("  %s[%s1%s]%s Authorize Book Issue %s───────%s %s[%s2%s]%s Process Book Return\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA, CLR_MUTED, CLR_RESET, CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA);
    printf("%s└───────────────────────────────────────────────────────────────────┘%s\n\n", CLR_MUTED, CLR_RESET);

    printf("  %s⚡ Select Transaction Action >> %s", CLR_TITLE, CLR_RESET);
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("\n  %s✘ ERROR:%s Invalid input format.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
        return;
    }

    switch(choice) {
        case 1:
            issue_book();
            break;
        case 2:
            return_book();
            break;
        default:
            printf("\n  %s✘ SELECTION OUT OF BOUNDS:%s Choice maps outside active branches.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
    }
}

void issue_book() {
    char title[100];
    char memberId[20];
    FILE *fbooks, *fmembers, *fissued;
    int bookFound = 0, memberFound = 0;

    getchar(); // Clear input stream buffer
    printf("\n  %s◆ INITIATE CHECK-OUT DISPATCH%s\n", CLR_TITLE, CLR_RESET);
    printf("  %s├─ Target Book Title: %s", CLR_MUTED, CLR_DATA);
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    printf("  %s└─ Operator Member ID: %s", CLR_MUTED, CLR_DATA);
    fgets(memberId, sizeof(memberId), stdin);
    memberId[strcspn(memberId, "\n")] = 0;

    /* Check if book exists */
    fbooks = fopen("books.dat", "rb");
    if (fbooks) {
        Book rb;
        while (fread(&rb, sizeof(Book), 1, fbooks) == 1) {
            if (strstr(rb.title, title) != NULL) {
                bookFound = 1;
                // Grab exact database title casing to keep logs standardized
                strncpy(title, rb.title, sizeof(title) - 1); 
                break;
            }
        }
        fclose(fbooks);
    }

    /* Check if member exists */
    fmembers = fopen("members.dat", "rb");
    if (fmembers) {
        Member rm;
        while (fread(&rm, sizeof(Member), 1, fmembers) == 1) {
            if (strcmp(rm.id, memberId) == 0 || strstr(rm.name, memberId) != NULL) {
                memberFound = 1;
                strncpy(memberId, rm.id, sizeof(memberId) - 1); // Normalize identifier to pure Card ID
                break;
            }
        }
        fclose(fmembers);
    }

    if (!bookFound) {
        printf("\n  %s✘ TRANSACTION ABORTED:%s Title signature not cataloged in index.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
        return;
    }
    if (!memberFound) {
        printf("\n  %s✘ TRANSACTION ABORTED:%s Authorized Membership profile not verified.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
        return;
    }

    /* Write to issued.dat */
    fissued = fopen("issued.dat", "ab");
    if (!fissued) {
        printf("\n  %s✘ SYSTEM FAULT:%s Unable to commit structural changes to disk records.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
        return;
    }

    IssuedRecord rec;
    strncpy(rec.title, title, sizeof(rec.title) - 1);
    rec.title[sizeof(rec.title) - 1] = 0;
    strncpy(rec.member_id, memberId, sizeof(rec.member_id) - 1);
    rec.member_id[sizeof(rec.member_id) - 1] = 0;
    rec.issue_time = time(NULL);

    fwrite(&rec, sizeof(IssuedRecord), 1, fissued);
    fclose(fissued);

    printf("\n  %s✔ SUCCESS:%s Manifest locked. Title '%s' deployed to ID '%s'.%s\n", CLR_SUCCESS, CLR_RESET, title, memberId, CLR_RESET);
}

void return_book() {
    char title[100];
    char memberId[20];
    FILE *fissued, *temp;
    int found = 0;
    time_t issueTime = 0;

    getchar(); // Clear input stream buffer
    printf("\n  %s◆ PROCESS ARRIVAL RETURN%s\n", CLR_TITLE, CLR_RESET);
    printf("  %s├─ Returning Book Title: %s", CLR_MUTED, CLR_DATA);
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    printf("  %s└─ Cardholder Member ID: %s", CLR_MUTED, CLR_DATA);
    fgets(memberId, sizeof(memberId), stdin);
    memberId[strcspn(memberId, "\n")] = 0;

    fissued = fopen("issued.dat", "rb");
    if (!fissued) {
        printf("\n  %s⚠ EMPTY STATE:%s No active outbound assets are recorded.%s\n", CLR_ACCENT, CLR_RESET, CLR_RESET);
        return;
    }

    temp = fopen("temp_issued.dat", "wb");
    if (!temp) {
        fclose(fissued);
        printf("\n  %s✘ CRITICAL SECTOR FAULT:%s Could not build high-speed cache buffer.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
        return;
    }

    IssuedRecord rec;
    while (fread(&rec, sizeof(IssuedRecord), 1, fissued) == 1) {
        if (strcmp(rec.title, title) == 0 && strcmp(rec.member_id, memberId) == 0) {
            issueTime = rec.issue_time;
            found = 1;
            continue; /* Skip layout clone to finalize data purge tracking */
        }
        fwrite(&rec, sizeof(IssuedRecord), 1, temp);
    }

    fclose(fissued);
    fclose(temp);

    if (found) {
        remove("issued.dat");
        rename("temp_issued.dat", "issued.dat");

        time_t now = time(NULL);
        long days = (now - issueTime) / 86400;
        long overdue = 0;
        long fine = 0;

        // Standard 14-day validation bracket
        if (days > 14) {
            overdue = days - 14;
            fine = overdue * 5; 
        }

        if (fine > 0) {
            printf("\n  %s⚠ RETURNING ASSET OVERDUE:%s Balance outstanding. Days: %s%ld%s | Penalty Fine: %s$%ld%s\n", 
                   CLR_ERROR, CLR_RESET, CLR_ACCENT, overdue, CLR_RESET, CLR_ERROR, fine, CLR_RESET);
        } else {
            printf("\n  %s✔ SUCCESS:%s Asset returned inside standard timeframe. Accounts clean.%s\n", CLR_SUCCESS, CLR_RESET, CLR_RESET);
        }
    } else {
        remove("temp_issued.dat");
        printf("\n  %s✘ EXCEPTION MATCH FAILURE:%s No active matching dispatch token found.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
    }
}