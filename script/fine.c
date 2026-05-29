#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../include/fine.h"
#include<time.h>


void fine()
{
    char memberId[50];
    char line[512];
    FILE *f = fopen("issued.txt", "r");
    if (!f) {
        printf("\nFine and Charges selected\n");
        printf("No issued records found.\n");
        return;
    }

    getchar();
    printf("Enter Member ID to calculate fines: ");
    fgets(memberId, sizeof(memberId), stdin);
    memberId[strcspn(memberId, "\n")] = 0;

    long totalFine = 0;
    long now = (long)time(NULL);

    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "ID=") && strstr(line, memberId)) {
            char *p = strstr(line, "Time=");
            if (p) {
                long issueTime = atol(p + 5);
                long days = (now - issueTime) / 86400;
                if (days > 14) {
                    totalFine += (days - 14) * 5;
                }
            }
        }
    }

    fclose(f);

    if (totalFine > 0) {
        printf("Total fine for member %s: %ld\n", memberId, totalFine);
    } else {
        printf("No fines for member %s\n", memberId);
    }
}