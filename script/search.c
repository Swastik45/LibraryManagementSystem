#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/search.h"
#include "../include/structs.h"

/* ── ANSI Dynamic Color Scheme ────────────────────────────────────── */
#define CLR_RESET   "\x1b[0m"
#define CLR_MUTED   "\x1b[90m"   // Dark Gray Borders/Brackets
#define CLR_TITLE   "\x1b[1;36m" // Bold Cyan Subheadings
#define CLR_ACCENT  "\x1b[1;33m" // Bold Yellow Highlights
#define CLR_DATA    "\x1b[37m"   // Soft White text
#define CLR_SUCCESS "\x1b[1;32m" // Green Messages
#define CLR_ERROR   "\x1b[1;31m" // Red Messages

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

void search() {
    int choice;
    char query[200];

    // Submenu Header Layout
    printf("\n%s┌───────────────────────────────────────────────────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
    printf("%s│                       🔍 CENTRAL SEARCH ENGINE                     │%s\n", CLR_TITLE, CLR_RESET);
    printf("%s├───────────────────────────────────────────────────────────────────┤%s\n", CLR_MUTED, CLR_RESET);
    printf("  %s[%s1%s]%s Query Book Catalog %s─────────%s %s[%s2%s]%s Query Membership Registry\n", CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA, CLR_MUTED, CLR_RESET, CLR_MUTED, CLR_ACCENT, CLR_MUTED, CLR_DATA);
    printf("%s└───────────────────────────────────────────────────────────────────┘%s\n\n", CLR_MUTED, CLR_RESET);

    printf("  %s⚡ Select Target Index >> %s", CLR_TITLE, CLR_RESET);
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("\n  %s✘ ERROR:%s Invalid input format.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
        return;
    }
    getchar(); // Clear trailing newline

    if (choice == 1) {
        FILE *f = fopen("books.dat", "rb");
        if (f == NULL) {
            printf("\n  %s✘ SYSTEM FAULT:%s Unable to read books dataset matrix.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
            return;
        }

        printf("\n  %s🔍 Enter Catalog Search Term (Case-Insensitive): %s", CLR_TITLE, CLR_DATA);
        fgets(query, sizeof(query), stdin);
        query[strcspn(query, "\n")] = '\0';

        if (strlen(query) == 0) {
            printf("\n  %s⚠ ABORTED:%s Search token constraint cannot be empty.%s\n", CLR_ACCENT, CLR_RESET, CLR_RESET);
            fclose(f);
            return;
        }

        printf("\n%s┌────┬─────────────────────────────────────┬──────────────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
        printf("%s│ %-2s │ %-35s │ %-28s │%s\n", CLR_MUTED, "ID", "Book Title", "Author", CLR_RESET);
        printf("%s├────┼─────────────────────────────────────┼──────────────────────────────┤%s\n", CLR_MUTED, CLR_RESET);

        Book b;
        int found = 0;
        while (fread(&b, sizeof(Book), 1, f)) {
            if (case_insensitive_search(b.title, query) || case_insensitive_search(b.author, query)) {
                printf("%s│ %s%2d%s │ %s%-35.35s%s │ %s%-28.28s%s │%s\n", 
                       CLR_MUTED, CLR_ACCENT, b.id, CLR_MUTED, CLR_DATA, b.title, CLR_MUTED, CLR_DATA, b.author, CLR_MUTED, CLR_RESET);
                found++;
            }
        }
        printf("%s└────┴─────────────────────────────────────┴──────────────────────────────┘%s\n", CLR_MUTED, CLR_RESET);
        
        if (found == 0) {
            printf("  %s⚠ No registered items matched the parameter '%s'%s\n", CLR_ACCENT, query, CLR_RESET);
        } else {
            printf("  %s✔ Lookup complete. Found %d matched entry segment(s).%s\n", CLR_SUCCESS, found, CLR_RESET);
        }
        fclose(f);

    } else if (choice == 2) {
        FILE *f = fopen("members.dat", "rb");
        if (f == NULL) {
            printf("\n  %s✘ SYSTEM FAULT:%s Unable to read member database structure.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
            return;
        }

        printf("\n  %s🔍 Enter Member Search Term (Case-Insensitive): %s", CLR_TITLE, CLR_DATA);
        fgets(query, sizeof(query), stdin);
        query[strcspn(query, "\n")] = '\0';

        if (strlen(query) == 0) {
            printf("\n  %s⚠ ABORTED:%s Search token constraint cannot be empty.%s\n", CLR_ACCENT, CLR_RESET, CLR_RESET);
            fclose(f);
            return;
        }

        printf("\n%s┌──────────┬──────────────────────────────┬──────────────────────┐%s\n", CLR_MUTED, CLR_RESET);
        printf("%s│ %-8s │ %-28s │ %-20s │%s\n", CLR_MUTED, "Card ID", "Name", "Contact Details", CLR_RESET);
        printf("%s├──────────┼──────────────────────────────┼──────────────────────┤%s\n", CLR_MUTED, CLR_RESET);

        Member m;
        int found = 0;
        while (fread(&m, sizeof(Member), 1, f)) {
            if (case_insensitive_search(m.name, query) || case_insensitive_search(m.id, query)) {
                printf("%s│ %s%-8.8s%s │ %s%-28.28s%s │ %s%-20.20s%s │%s\n", 
                       CLR_MUTED, CLR_ACCENT, m.id, CLR_MUTED, CLR_DATA, m.name, CLR_MUTED, CLR_DATA, m.contact, CLR_MUTED, CLR_RESET);
                found++;
            }
        }
        printf("%s└──────────┴──────────────────────────────┴──────────────────────┘%s\n", CLR_MUTED, CLR_RESET);
        
        if (found == 0) {
            printf("  %s⚠ No operator profiles matched the parameter '%s'%s\n", CLR_ACCENT, query, CLR_RESET);
        } else {
            printf("  %s✔ Lookup complete. Found %d matched profile segment(s).%s\n", CLR_SUCCESS, found, CLR_RESET);
        }
        fclose(f);

    } else {
        printf("\n  %s✘ SELECTION OUT OF BOUNDS:%s Input value mapped outside option limits.%s\n", CLR_ERROR, CLR_RESET, CLR_RESET);
    }
}