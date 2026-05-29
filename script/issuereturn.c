#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "../include/issuereturn.h"

void issue_book();
void return_book();

void issuereturn()
{
    int choice;

    printf("\n1. Issue Book\n");
    printf("2. Return Book\n");

    printf("Enter choice: ");
    scanf("%d",&choice);

    switch(choice)
    {
        case 1:
            issue_book();
            break;

        case 2:
            return_book();
            break;

        default:
            printf("Invalid choice\n");
    }
}

void issue_book()
{
    char title[200];
    char memberId[50];
    FILE *fbooks, *fmembers, *fissued;
    char line[512];
    int bookFound = 0, memberFound = 0;

    getchar();
    printf("Enter Book Title to Issue: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    printf("Enter Member ID: ");
    fgets(memberId, sizeof(memberId), stdin);
    memberId[strcspn(memberId, "\n")] = 0;

    fbooks = fopen("books.txt", "r");
    if (fbooks) {
        while (fgets(line, sizeof(line), fbooks)) {
            if (strstr(line, title) != NULL) {
                bookFound = 1;
                break;
            }
        }
        fclose(fbooks);
    }

    fmembers = fopen("members.txt", "r");
    if (fmembers) {
        while (fgets(line, sizeof(line), fmembers)) {
            if (strstr(line, memberId) != NULL) {
                memberFound = 1;
                break;
            }
        }
        fclose(fmembers);
    }

    if (!bookFound) {
        printf("Book not found in catalog. Issue aborted.\n");
        return;
    }
    if (!memberFound) {
        printf("Member ID not found. Issue aborted.\n");
        return;
    }

    fissued = fopen("issued.txt", "a");
    if (!fissued) {
        printf("Unable to open issued records.\n");
        return;
    }

    long now = (long)time(NULL);
    // store as key=value pairs for simple parsing
    fprintf(fissued, "Title=%s;ID=%s;Time=%ld\n", title, memberId, now);
    fclose(fissued);

    printf("Book '%s' issued to member '%s'.\n", title, memberId);
}


void return_book()
{
    char title[200];
    char memberId[50];
    FILE *fissued, *temp;
    char line[512];
    int found = 0;
    long issueTime = 0;

    getchar();
    printf("Enter Book Title to Return: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    printf("Enter Member ID: ");
    fgets(memberId, sizeof(memberId), stdin);
    memberId[strcspn(memberId, "\n")] = 0;

    fissued = fopen("issued.txt", "r");
    if (!fissued) {
        printf("No issued records found.\n");
        return;
    }

    temp = fopen("temp_issued.txt", "w");
    if (!temp) {
        fclose(fissued);
        printf("Unable to create temporary file.\n");
        return;
    }

    while (fgets(line, sizeof(line), fissued)) {
        int matchTitle = (strstr(line, "Title=") && strstr(line, title));
        int matchId = (strstr(line, "ID=") && strstr(line, memberId));

        if (matchTitle && matchId) {
            // extract Time=
            char *p = strstr(line, "Time=");
            if (p) {
                issueTime = atol(p + 5);
            }
            found = 1;
            // skip writing this line so it gets removed (book returned)
        } else {
            fputs(line, temp);
        }
    }

    fclose(fissued);
    fclose(temp);

    if (found) {
        remove("issued.txt");
        rename("temp_issued.txt", "issued.txt");

        long now = (long)time(NULL);
        long days = (now - issueTime) / 86400;
        long overdue = 0;
        long fine = 0;
        if (days > 14) {
            overdue = days - 14;
            fine = overdue * 5; // 5 currency units per day
        }

        if (fine > 0)
            printf("Book returned. Overdue by %ld days. Fine: %ld\n", overdue, fine);
        else
            printf("Book returned on time. No fine.\n");
    } else {
        remove("temp_issued.txt");
        printf("Matching issued record not found.\n");
    }
}