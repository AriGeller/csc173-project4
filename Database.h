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


//definitions for CSG
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
extern CSG *CSG_new(char *course, char *studentID, char *grade);
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


// definitions for SNAP
typedef struct SNAP *SNAPList;
typedef struct SNAP {
	char studentID[6];
	char name[21];
	char address[21];
	char phone[11];
	SNAPList next;
	SNAPList nameIndexNext;

} SNAP;
typedef SNAPList SNAPTable[TABLE_LENGTH];
typedef struct {
	SNAPTable table;
	SNAPTable nameIndex;
	
} SNAPBase;

typedef struct SNAPLinkedList {
    struct SNAPLinkedListNode *first;
    struct SNAPLinkedListNode *last;
} SNAPLinkedList;

typedef struct SNAPLinkedListNode {
    SNAP *data;
    struct SNAPLinkedListNode *next;
    struct SNAPLinkedListNode *prev;
} SNAPLinkedListNode;


extern SNAPBase *SNAPBase_new();
extern SNAP *SNAP_new(char *studentID, char *name, char *address, char *phone);
extern int SNAPHash(char *studentID);
extern int SNAPHash_name(char *name);
extern bool isTaken_SNAP(SNAPTable table, int index);
extern void insert_SNAP(SNAPBase *table, char *studentID, char *name, char *address, char *phone);
extern bool SNAP_add(SNAPBase *database, int index, SNAP *toAdd, int kind);
extern SNAPLinkedList *lookup_SNAP(SNAPBase *database, char *studentID, char *name);
extern void printList_SNAP(SNAPLinkedList *list);
extern SNAPLinkedList *SNAPLinkedList_new();
extern SNAPLinkedListNode *SNAPLinkedListNode_new(SNAP *data);
extern void SNAPLinkedList_add(SNAPLinkedList *list, SNAP *data);
extern void delete_SNAP(SNAPBase *database, char *studentID, char *name);
extern void deleteFromIndices_SNAP(SNAPBase *database, char *studentID, char *name);


//definitions for CP

typedef struct CP *CPList;
typedef struct CP {
	char course[6];
	char prereq[6];
	CPList next;
} CP;

typedef CPList CPTable[TABLE_LENGTH];
typedef struct {
	CPTable table;
} CPBase;

typedef struct CPLinkedList {
    struct CPLinkedListNode *first;
    struct CPLinkedListNode *last;
} CPLinkedList;

typedef struct CPLinkedListNode {
    CP *data;
    struct CPLinkedListNode *next;
    struct CPLinkedListNode *prev;
} CPLinkedListNode;

extern CPBase *CPBase_new();
extern CP *CP_new(char *course, char *prereq);;
extern int CPHash(char *course);
extern bool isTaken_CP(CPTable table, int index);
extern void insert_CP(CPBase *table, char *course, char *prereq);
extern bool CP_add(CPBase *database, int index, CP *toAdd);
extern CPLinkedList *lookup_CP(CPBase *database, char *course);
extern void printList_CP(CPLinkedList *list);
extern CPLinkedList *CPLinkedList_new();
extern CPLinkedListNode *CPLinkedListNode_new(CP *data);
extern void CPLinkedList_add(CPLinkedList *list, CP *data);
extern void delete_CP(CPBase *database, char *course, char *prereq);

//definitions for CDH
typedef struct CDH *CDHList;
typedef struct CDH {
	char course[6];
	char day[3];
	char hour[5];
	CDHList next;
	CDHList courseIndexNext;
	CDHList dayIndexNext;
} CDH;
typedef CDHList CDHTable[TABLE_LENGTH];
typedef struct {
	CDHTable table;
	CDHTable courseIndex;
	CDHTable dayIndex;
} CDHBase;

typedef struct CDHLinkedList {
    struct CDHLinkedListNode *first;
    struct CDHLinkedListNode *last;
} CDHLinkedList;

typedef struct CDHLinkedListNode {
    CDH *data;
    struct CDHLinkedListNode *next;
    struct CDHLinkedListNode *prev;
} CDHLinkedListNode;

extern CDHBase *CDHBase_new();
extern CDH *CDH_new(char *course, char *day, char *hour);
extern int CDHHash(char *course, char *day);
extern int CDHHash_course(char *course);
extern int CDHHash_day(char *day);
extern bool isTaken_CDH(CDHTable table, int index);
extern void insert_CDH(CDHBase *table, char *course, char *day, char *hour);
extern bool CDH_add(CDHBase *database, int index, CDH *toAdd, int kind);
extern CDHLinkedList *lookup_CDH(CDHBase *database, char *course, char *day, char *hour);
extern void printList_CDH(CDHLinkedList *list);
extern CDHLinkedList *CDHLinkedList_new();
extern CDHLinkedListNode *CDHLinkedListNode_new(CDH *data);
extern void CDHLinkedList_add(CDHLinkedList *list, CDH *data);
extern void delete_CDH(CDHBase *database, char *course, char *day, char *hour);
extern void deleteFromIndices_CDH(CDHBase *database, char *course, char *day, char *hour);

//definitions for CR

typedef struct CR *CRList;
typedef struct CR {
	char course[6];
	char room[15];
	CRList next;
} CR;

typedef CRList CRTable[TABLE_LENGTH];
typedef struct {
	CRTable table;
} CRBase;

typedef struct CRLinkedList {
    struct CRLinkedListNode *first;
    struct CRLinkedListNode *last;
} CRLinkedList;

typedef struct CRLinkedListNode {
    CR *data;
    struct CRLinkedListNode *next;
    struct CRLinkedListNode *prev;
} CRLinkedListNode;

extern CRBase *CRBase_new();
extern CR *CR_new(char *course, char *room);;
extern int CRHash(char *course);
extern bool isTaken_CR(CRTable table, int index);
extern void insert_CR(CRBase *table, char *course, char *room);
extern bool CR_add(CRBase *database, int index, CR *toAdd);
extern CRLinkedList *lookup_CR(CRBase *database, char *course);
extern void printList_CR(CRLinkedList *list);
extern CRLinkedList *CRLinkedList_new();
extern CRLinkedListNode *CRLinkedListNode_new(CR *data);
extern void CRLinkedList_add(CRLinkedList *list, CR *data);
extern void delete_CR(CRBase *database, char *course, char *room);


typedef struct CSGBaseList {
	struct CSGBaseListNode *first;
	struct CSGBaseListNode *last;
} CSGBaseList;

typedef struct CSGBaseListNode {
	CSGBase *data;
	struct CSGBaseListNode *next;
	struct CSGBaseListNode *prev;
} CSGBaseListNode;

extern CSGBaseList *CSGBaseList_new();
extern CSGBaseListNode *CSGBaseListNode_new(CSGBase *data);
extern void CSGBaseList_add(CSGBaseList *list, CSGBase *data);

typedef struct SNAPBaseList {
	struct SNAPBaseListNode *first;
	struct SNAPBaseListNode *last;
} SNAPBaseList;

typedef struct SNAPBaseListNode {
	SNAPBase *data;
	struct SNAPBaseListNode *next;
	struct SNAPBaseListNode *prev;
} SNAPBaseListNode;

extern SNAPBaseList *SNAPBaseList_new();
extern SNAPBaseListNode *SNAPBaseListNode_new(SNAPBase *data);
extern void SNAPBaseList_add(SNAPBaseList *list, SNAPBase *data);

typedef struct CPBaseList {
	struct CPBaseListNode *first;
	struct CPBaseListNode *last;
} CPBaseList;

typedef struct CPBaseListNode {
	CPBase *data;
	struct CPBaseListNode *next;
	struct CPBaseListNode *prev;
} CPBaseListNode;

extern CPBaseList *CPBaseList_new();
extern CPBaseListNode *CPBaseListNode_new(CPBase *data);
extern void CPBaseList_add(CPBaseList *list, CPBase *data);

typedef struct CDHBaseList {
	struct CDHBaseListNode *first;
	struct CDHBaseListNode *last;
} CDHBaseList;

typedef struct CDHBaseListNode {
	CDHBase *data;
	struct CDHBaseListNode *next;
	struct CDHBaseListNode *prev;
} CDHBaseListNode;

extern CDHBaseList *CDHBaseList_new();
extern CDHBaseListNode *CDHBaseListNode_new(CDHBase *data);
extern void CDHBaseList_add(CDHBaseList *list, CDHBase *data);

typedef struct CRBaseList {
	struct CRBaseListNode *first;
	struct CRBaseListNode *last;
} CRBaseList;

typedef struct CRBaseListNode {
	CRBase *data;
	struct CRBaseListNode *next;
	struct CRBaseListNode *prev;
} CRBaseListNode;

extern CRBaseList *CRBaseList_new();
extern CRBaseListNode *CRBaseListNode_new(CRBase *data);
extern void CRBaseList_add(CRBaseList *list, CRBase *data);


typedef struct GenRel {
	char *course;
	char *studentID;
	char *grade;
	char *name;
	char *address;
	char *phone;
	char *prereq;
	char *day;
	char *hour;
	char *room;
} GenRel;

typedef struct GenRelList {
	struct GenRelListNode *first;
	struct GenRelListNode *last;
} GenRelList;

typedef struct GenRelListNode {
	GenRel *data;
	struct GenRelListNode *next;
	struct GenRelListNode *prev;
} GenRelListNode;

extern GenRelList *GenRelList_new();
extern GenRelListNode *GenRelistNode_new(GenRel *data);
extern void GenRelList_add(GenRelList *list, GenRel *data);
extern GenRel *GenRel_new(char *course, char *studentID, char *grade, char *name, char *address, char *phone, char *day, char *hour, char *prereq, char *room);

GenRelList *select(GenRelList *list, char *type, char *param);
GenRelList *project(GenRelList *list, char *param);
bool projectHelper(GenRelList *list, char *param);

bool sameString(char *thing1, char *thing2);

GenRelList *project(GenRelList *list, char *param);

GenRelList *join(GenRelList *list1, GenRelList *list2, char *param);

GenRel *combo(GenRel *rel1, GenRel *rel2);

GenRelList *convert_CSG(CSGBase *database);

GenRelList *convert_SNAP(SNAPBase *database);
GenRelList *convert_CDH(CDHBase *database);
GenRelList *convert_CP(CPBase *database);
GenRelList *convert_CR(CRBase *database);

void GenRelList_print(GenRelList *list);




#endif