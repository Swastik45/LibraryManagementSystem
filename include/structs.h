#ifndef STRUCTS_H
#define STRUCTS_H

#include <time.h>

/* Book structure for binary storage */
typedef struct {
    int id;
    char title[100];
    char author[100];
} Book;

/* Member structure for binary storage */
typedef struct {
    char id[20];
    char name[100];
    char contact[100];
} Member;

/* Issued record structure for binary storage */
typedef struct {
    char title[100];
    char member_id[20];
    time_t issue_time;
} IssuedRecord;

#endif
