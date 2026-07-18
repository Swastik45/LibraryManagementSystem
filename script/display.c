#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/display.h"
#include "../include/structs.h"

// ANSI Color Theme Customization
#define CLR_RESET   "\x1b[0m"
#define CLR_HEADER  "\x1b[1;36m" // Bold Cyan
#define CLR_TITLE   "\x1b[1;33m" // Bold Yellow
#define CLR_MUTED   "\x1b[90m"   // Dark Gray (Borders)
#define CLR_DATA    "\x1b[37m"   // Soft White
#define CLR_ALERT   "\x1b[31m"   // Red (Empty state)
#define CLR_SUCCESS "\x1b[1;32m" // Bold Green (Summary values)

void display() {
    int book_count = 0;
    int member_count = 0;
    int loan_count = 0;

    // Outer Main Header
    printf("\n%s╔═══════════════════════════════════════════════════════════════════════════════════╗%s\n", CLR_MUTED, CLR_RESET);
    printf("%s║                               LIBRARY RECORDS REPORT                              ║%s\n", CLR_HEADER, CLR_RESET);
    printf("%s╚═══════════════════════════════════════════════════════════════════════════════════╝%s\n", CLR_MUTED, CLR_RESET);

    /* ── 1. BOOKS CATALOG ──────────────────────────────────────────────────── */
    printf("\n%s◆ CATALOGED BOOKS%s\n", CLR_TITLE, CLR_RESET);
    FILE *fb = fopen("books.dat", "rb");
    if (fb) {
        printf("%s┌────────────┬─────────────────────────────────────┬──────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
        printf("%s│ %-10s │ %-35s │ %-20s │%s\n", CLR_MUTED, "Book ID", "Title", "Author", CLR_RESET);
        printf("%s├────────────┼─────────────────────────────────────┼──────────────────────┤%s\n", CLR_MUTED, CLR_RESET);

        Book b;
        while (fread(&b, sizeof(Book), 1, fb)) {
            printf("%s│ %s%-10d%s │ %s%-35.35s%s │ %s%-20.20s%s │%s\n", 
                   CLR_MUTED, CLR_DATA, b.id, CLR_MUTED, CLR_DATA, b.title, CLR_MUTED, CLR_DATA, b.author, CLR_MUTED, CLR_RESET);
            book_count++;
        }
        fclose(fb);
        if (book_count > 0) {
            printf("%s└────────────┴─────────────────────────────────────┴──────────────────────┘%s\n", CLR_MUTED, CLR_RESET);
        }
    }
    if (book_count == 0) printf("  %s⚠ No registered books found.%s\n", CLR_ALERT, CLR_RESET);

    /* ── 2. MEMBERS ────────────────────────────────────────────────────────── */
    printf("\n%s◆ ACTIVE MEMBERS%s\n", CLR_TITLE, CLR_RESET);
    FILE *fm = fopen("members.dat", "rb");
    if (fm) {
        printf("%s┌────────────┬─────────────────────────────────────┬──────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
        printf("%s│ %-10s │ %-35s │ %-20s │%s\n", CLR_MUTED, "Member ID", "Full Name", "Contact Details", CLR_RESET);
        printf("%s├────────────┼─────────────────────────────────────┼──────────────────────┤%s\n", CLR_MUTED, CLR_RESET);

        Member m;
        while (fread(&m, sizeof(Member), 1, fm)) {
            printf("%s│ %s%-10.10s%s │ %s%-35.35s%s │ %s%-20.20s%s │%s\n", 
                   CLR_MUTED, CLR_DATA, m.id, CLR_MUTED, CLR_DATA, m.name, CLR_MUTED, CLR_DATA, m.contact, CLR_MUTED, CLR_RESET);
            member_count++;
        }
        fclose(fm);
        if (member_count > 0) {
            printf("%s└────────────┴─────────────────────────────────────┴──────────────────────┘%s\n", CLR_MUTED, CLR_RESET);
        }
    }
    if (member_count == 0) printf("  %s⚠ No registered members found.%s\n", CLR_ALERT, CLR_RESET);

    /* ── 3. ISSUED RECORDS ─────────────────────────────────────────────────── */
    printf("\n%s◆ ACTIVE LOANS & TRACKING%s\n", CLR_TITLE, CLR_RESET);
    FILE *fi = fopen("issued.dat", "rb");
    if (fi) {
        printf("%s┌─────────────────────────────────────┬────────────┬───────────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
        printf("%s│ %-35s │ %-10s │ %-25s │%s\n", CLR_MUTED, "Book Title", "Member ID", "Issue Date", CLR_RESET);
        printf("%s├─────────────────────────────────────┼────────────┼───────────────────────────┤%s\n", CLR_MUTED, CLR_RESET);

        IssuedRecord ir;
        while (fread(&ir, sizeof(IssuedRecord), 1, fi)) {
            char timestr[64] = "Pending...";
            if (ir.issue_time > 0) {
                time_t tt = ir.issue_time;
                struct tm *tm = localtime(&tt);
                if (tm) strftime(timestr, sizeof(timestr), "%b %d, %Y (%I:%M %p)", tm);
            }
            printf("%s│ %s%-35.35s%s │ %s%-10.10s%s │ %s%-25.25s%s │%s\n", 
                   CLR_MUTED, CLR_DATA, ir.title, CLR_MUTED, CLR_DATA, ir.member_id, CLR_MUTED, CLR_DATA, timestr, CLR_MUTED, CLR_RESET);
            loan_count++;
        }
        fclose(fi);
        if (loan_count > 0) {
            printf("%s└─────────────────────────────────────┴────────────┴───────────────────────────┘%s\n", CLR_MUTED, CLR_RESET);
        }
    }
    if (loan_count == 0) printf("  %s⚠ No books are currently checked out.%s\n", CLR_ALERT, CLR_RESET);

    /* ── Summary Dashboard ─────────────────────────────────────────────────── */
    printf("\n%s┌─────────────────────────────────────────────────────────────────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
    printf("  %sSYSTEM SUMMARY%s  >>   Books: %s%d%s   │   Members: %s%d%s   │   Active Loans: %s%d%s\n", 
           CLR_HEADER, CLR_RESET,
           CLR_SUCCESS, book_count, CLR_RESET, 
           CLR_SUCCESS, member_count, CLR_RESET, 
           CLR_SUCCESS, loan_count, CLR_RESET);
    printf("%s└─────────────────────────────────────────────────────────────────────────────────┘%s\n\n", CLR_MUTED, CLR_RESET);
}