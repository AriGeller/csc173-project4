/*
 * File: Database.c
 * Creator: Ari Geller and Adam Kravitz
 *
 * Header file of for database
 */


#ifndef _Database_h_
#define _Database_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define TABLE_LENGTH 1009

typedef struct CSG *CSGList
typedef struct CSG {
	char course[5];
	int studentID;
	char grade[2];
	CSGList next;
} CSG;
typedef CSGList CSGTable[1009];

extern *CSGList CSGList_new();
extern *CSG CSG_new();
extern int CSGHash(char* course, int studentID);
extern bool isTaken_CSG(CSGTable *table, int index);
extern void insert_CSG(CSGTable *table, char *course, int studentID, char *grade);


#endif