#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/display.h"

/* Safe truncating copy — copies at most (dst_size - 1) bytes and null-terminates */
static void strtrunc(char *dst, const char *src, size_t dst_size) {
    strncpy(dst, src, dst_size - 1);
    dst[dst_size - 1] = '\0';
}

/* Prints a horizontal rule with corner/junction chars at each column boundary */
static void hline(const char *left, const char *mid, const char *right,
                  const int *widths, int ncols) {
    printf("%s", left);
    for (int c = 0; c < ncols; c++) {
        for (int i = 0; i < widths[c] + 2; i++) printf("─");
        printf("%s", c < ncols - 1 ? mid : right);
    }
    printf("\n");
}

/* Prints a formatted table row, padding each cell to its column width */
static void row(const char **cells, const int *widths, int ncols) {
    printf("│");
    for (int c = 0; c < ncols; c++)
        printf(" %-*s │", widths[c], cells[c]);
    printf("\n");
}

void display() {

    /* ── 1. BOOKS CATALOG ─────────────────────────────────────────── */
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║           LIBRARY RECORDS — FULL SYSTEM STATUS           ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");

    printf("\n  BOOKS CATALOG\n");

    int bw[] = {4, 30, 22};
    hline("┌", "┬", "┐", bw, 3);
    const char *bhead[] = {"ID", "Title", "Author"};
    row(bhead, bw, 3);
    hline("├", "┼", "┤", bw, 3);

    FILE *fb = fopen("books.txt", "r");
    int book_count = 0;
    char line[512];
    if (fb) {
        while (fgets(line, sizeof(line), fb)) {
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) == 0) continue;

            char id[8] = "", title[32] = "", author[24] = "";
            if (sscanf(line, "ID=%7[^;];Title=%31[^;];Author=%23[^\n]",
                       id, title, author) == 3) {
                const char *cells[] = {id, title, author};
                row(cells, bw, 3);
            } else {
                char truncated[31];
                strtrunc(truncated, line, sizeof(truncated));
                const char *cells[] = {"—", truncated, ""};
                row(cells, bw, 3);
            }
            book_count++;
        }
        fclose(fb);
    }
    if (book_count == 0) {
        const char *cells[] = {"", "(no books found)", ""};
        row(cells, bw, 3);
    }
    hline("└", "┴", "┘", bw, 3);

    /* ── 2. MEMBERS ───────────────────────────────────────────────── */
    printf("\n  MEMBERS\n");

    int mw[] = {5, 28, 24};
    hline("┌", "┬", "┐", mw, 3);
    const char *mhead[] = {"ID", "Name", "Contact"};
    row(mhead, mw, 3);
    hline("├", "┼", "┤", mw, 3);

    FILE *fm = fopen("members.txt", "r");
    int member_count = 0;
    if (fm) {
        while (fgets(line, sizeof(line), fm)) {
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) == 0) continue;

            char id[8] = "", name[30] = "", contact[26] = "";
            if (sscanf(line, "ID=%7[^;];Name=%29[^;];Contact=%25[^\n]",
                       id, name, contact) == 3) {
                const char *cells[] = {id, name, contact};
                row(cells, mw, 3);
            } else {
                char truncated[29];
                strtrunc(truncated, line, sizeof(truncated));
                const char *cells[] = {"—", truncated, ""};
                row(cells, mw, 3);
            }
            member_count++;
        }
        fclose(fm);
    }
    if (member_count == 0) {
        const char *cells[] = {"", "(no members found)", ""};
        row(cells, mw, 3);
    }
    hline("└", "┴", "┘", mw, 3);

    /* ── 3. ISSUED RECORDS ────────────────────────────────────────── */
    printf("\n  ISSUED RECORDS\n");

    int iw[] = {27, 8, 20};
    hline("┌", "┬", "┐", iw, 3);
    const char *ihead[] = {"Book Title", "Member", "Issued Date/Time"};
    row(ihead, iw, 3);
    hline("├", "┼", "┤", iw, 3);

    FILE *fi = fopen("issued.txt", "r");
    int loan_count = 0;
    if (fi) {
        while (fgets(line, sizeof(line), fi)) {
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) == 0) continue;

            char title[100] = "", id[50] = "";
            long int timestamp = 0;
            if (sscanf(line, "Title=%99[^;];ID=%49[^;];Time=%ld",
                       title, id, &timestamp) == 3) {
                time_t rawtime = (time_t)timestamp;
                struct tm *timeinfo = localtime(&rawtime);
                char time_buf[22];
                if (timeinfo)
                    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
                else
                    strcpy(time_buf, "Invalid time");

                char short_title[28];
                strtrunc(short_title, title, sizeof(short_title));

                const char *cells[] = {short_title, id, time_buf};
                row(cells, iw, 3);
            } else {
                char truncated[28];
                strtrunc(truncated, line, sizeof(truncated));
                const char *cells[] = {truncated, "", ""};
                row(cells, iw, 3);
            }
            loan_count++;
        }
        fclose(fi);
    }
    if (loan_count == 0) {
        const char *cells[] = {"(no issued records)", "", ""};
        row(cells, iw, 3);
    }
    hline("└", "┴", "┘", iw, 3);

    /* ── Summary footer ───────────────────────────────────────────── */
    printf("\n  %d active loan(s)  ·  %d book(s)  ·  %d member(s)\n\n",
           loan_count, book_count, member_count);
}