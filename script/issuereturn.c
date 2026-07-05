#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "../include/issuereturn.h"
#include "../include/structs.h"

void issue_book();
void return_book();

void issuereturn()
{
    int choice;

    printf("\n--- Issue & Return Menu ---\n");
    printf("1. Issue Book\n");
    printf("2. Return Book\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

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
    char title[100];
    char memberId[20];
    FILE *fbooks, *fmembers, *fissued;
    int bookFound = 0, memberFound = 0;

    getchar();
    printf("Enter Book Title to Issue: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    printf("Enter Member ID: ");
    fgets(memberId, sizeof(memberId), stdin);
    memberId[strcspn(memberId, "\n")] = 0;

    /* Check if book exists */
    fbooks = fopen("books.dat", "rb");
    if (fbooks) {
        Book rb;
        while (fread(&rb, sizeof(Book), 1, fbooks) == 1) {
            if (strstr(rb.title, title) != NULL) {
                bookFound = 1;
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

    /* Write to issued.dat */
    fissued = fopen("issued.dat", "ab");
    if (!fissued) {
        printf("Unable to open issued records.\n");
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

    printf("Book '%s' issued to member '%s'.\n", title, memberId);
}


void return_book()
{
    char title[100];
    char memberId[20];
    FILE *fissued, *temp;
    int found = 0;
    time_t issueTime = 0;

    getchar();
    printf("Enter Book Title to Return: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    printf("Enter Member ID: ");
    fgets(memberId, sizeof(memberId), stdin);
    memberId[strcspn(memberId, "\n")] = 0;

    fissued = fopen("issued.dat", "rb");
    if (!fissued) {
        printf("No issued records found.\n");
        return;
    }

    temp = fopen("temp_issued.dat", "wb");
    if (!temp) {
        fclose(fissued);
        printf("Unable to create temporary file.\n");
        return;
    }

    IssuedRecord rec;
    while (fread(&rec, sizeof(IssuedRecord), 1, fissued) == 1) {
        if (strcmp(rec.title, title) == 0 && strcmp(rec.member_id, memberId) == 0) {
            issueTime = rec.issue_time;
            found = 1;
            continue; /* skip - book is being returned */
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

        if (days > 14) {
            overdue = days - 14;
            fine = overdue * 5; /* 5 units per day */
        }

        if (fine > 0) {
            printf("Book returned. Overdue by %ld days. Fine: %ld\n", overdue, fine);
        } else {
            printf("Book returned on time. No fine.\n");
        }
    } else {
        remove("temp_issued.dat");
        printf("Matching issued record not found.\n");
    }
}