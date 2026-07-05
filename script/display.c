#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/display.h"
#include "../include/structs.h"

void display() {
    int book_count = 0;
    int member_count = 0;
    int loan_count = 0;

    printf("\n================================= LIBRARY RECORDS =================================\n");

    /* ── 1. BOOKS CATALOG (BINARY) ─────────────────────────────────────────── */
    printf("\n[ BOOKS ]\n");
    FILE *fb = fopen("books.dat", "rb");
    if (fb) {
        printf("+------------+-------------------------------------+----------------------+\n");
        printf("| %-10s | %-35s | %-20s |\n", "Book ID", "Title", "Author");
        printf("+------------+-------------------------------------+----------------------+\n");

        Book b;
        while (fread(&b, sizeof(Book), 1, fb)) {
            printf("| %-10d | %-35.35s | %-20.20s |\n", b.id, b.title, b.author);
            book_count++;
        }
        fclose(fb);
        if (book_count > 0) {
            printf("+------------+-------------------------------------+----------------------+\n");
        }
    }
    if (book_count == 0) printf(" (No books found)\n");

    /* ── 2. MEMBERS (BINARY) ───────────────────────────────────────────────── */
    printf("\n[ MEMBERS ]\n");
    FILE *fm = fopen("members.dat", "rb");
    if (fm) {
        printf("+------------+-------------------------------------+----------------------+\n");
        printf("| %-10s | %-35s | %-20s |\n", "Member ID", "Name", "Contact");
        printf("+------------+-------------------------------------+----------------------+\n");

        Member m;
        while (fread(&m, sizeof(Member), 1, fm)) {
            printf("| %-10.10s | %-35.35s | %-20.20s |\n", m.id, m.name, m.contact);
            member_count++;
        }
        fclose(fm);
        if (member_count > 0) {
            printf("+------------+-------------------------------------+----------------------+\n");
        }
    }
    if (member_count == 0) printf(" (No members found)\n");

    /* ── 3. ISSUED RECORDS (BINARY) ────────────────────────────────────────── */
    printf("\n[ ISSUED RECORDS ]\n");
    FILE *fi = fopen("issued.dat", "rb");
    if (fi) {
        printf("+-------------------------------------+------------+---------------------------+\n");
        printf("| %-35s | %-10s | %-25s |\n", "Book Title", "Member ID", "Issue Date");
        printf("+-------------------------------------+------------+---------------------------+\n");

        IssuedRecord ir;
        while (fread(&ir, sizeof(IssuedRecord), 1, fi)) {
            char timestr[64] = "";
            if (ir.issue_time > 0) {
                time_t tt = ir.issue_time;
                struct tm *tm = localtime(&tt);
                if (tm) strftime(timestr, sizeof(timestr), "%Y-%m-%d", tm);
            }
            printf("| %-35.35s | %-10.10s | %-25.25s |\n", ir.title, ir.member_id, timestr);
            loan_count++;
        }
        fclose(fi);
        if (loan_count > 0) {
            printf("+-------------------------------------+------------+---------------------------+\n");
        }
    }
    if (loan_count == 0) printf(" (No issued records found)\n");

    /* ── Summary ──────────────────────────────────────────────────── */
    printf("\n===================================================================================\n");
    printf(" Summary Data >>  Total Books: %d  |  Total Members: %d  |  Active Loans: %d\n", 
           book_count, member_count, loan_count);
    printf("===================================================================================\n\n");
}
