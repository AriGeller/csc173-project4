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

typedef struct CSG *CSGList;
typedef struct CSG {
	char course[6];
	char studentID[6];
	char grade[3];
	CSGList next;
	CSGList idIndexNext;
	CSGList courseIndexNext;
} CSG;
typedef CSGList CSGTable[TABLE_LENGTH];
typedef struct {
	CSGTable table;
	CSGTable idIndex;
	CSGTable courseIndex;
} CSGBase;

extern CSGBase *CSGBase_new();
extern CSG *CSG_new();
extern int CSGHash(char* course, char *studentID);
extern int CSGHash_id(char *studentID);
extern int CSGHash_course(char *course);
extern bool isTaken_CSG(CSGTable table, int index);
extern void insert_CSG(CSGBase *table, char *course, char *studentID, char *grade);
extern void lookup_CSG(CSGTable *table, char *course, char *studentID);
extern void CSG_add(CSGTable *table, int index, CSG *toAdd, int kind);



#endif