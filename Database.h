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

typedef struct CSGLinkedList {
    struct CSGLinkedListNode *first;
    struct CSGLinkedListNode *last;
} CSGLinkedList;

typedef struct CSGLinkedListNode {
    CSG *data;
    struct CSGLinkedListNode *next;
    struct CSGLinkedListNode *prev;
} CSGLinkedListNode;

extern CSGBase *CSGBase_new();
extern CSG *CSG_new();
extern int CSGHash(char* course, char *studentID);
extern int CSGHash_id(char *studentID);
extern int CSGHash_course(char *course);
extern bool isTaken_CSG(CSGTable table, int index);
extern void insert_CSG(CSGBase *table, char *course, char *studentID, char *grade);
extern bool CSG_add(CSGBase *database, int index, CSG *toAdd, int kind);
extern CSGLinkedList *lookup_CSG(CSGBase *database, char *course, char *studentID, char *grade);
extern void printList_CSG(CSGLinkedList *list);
extern CSGLinkedList *CSGLinkedList_new();
extern CSGLinkedListNode *CSGLinkedListNode_new(CSG *data);
extern void CSGLinkedList_add(CSGLinkedList *list, CSG *data);
extern void delete_CSG(CSGBase *database, char *course, char *studentID, char *grade);
extern void deleteFromIndices_CSG(CSGBase *database, char *course, char *studentID, char *grade);


#endif