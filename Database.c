/*
 * File: Database.c
 * Creator: Ari Geller and Adam Kravitz
 *
 * Implementation of a Relational Database
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Database.h"


CSGBase *CSGBase_new(){
	CSGBase *new = (CSGBase*)malloc(sizeof(CSGBase));
	return new;
}

CSG *CSG_new(char *course, char *studentID, char *grade) {
	CSG *new = (CSG*)malloc(sizeof(CSG));
	for (int i = 0; i < 5; i++) {
		new -> course[i] = course[i];
	}
	new -> course[5] = '\0';
	
	for (int i = 0; i < 5; i++) {
		new -> studentID[i] = studentID[i];
	}
	new -> studentID[5] = '\0';
	for (int i = 0; i < 2; i ++) {
		new -> grade[i] = grade[i];
	}
	new -> grade[2] = '\0';
	
	new -> next = NULL;
	new -> idIndexNext = NULL;
	new -> courseIndexNext = NULL;

	return new;
}

int CSGHash(char* course, char *studentID) {
	int strAsNum = 0;
	int max = strlen(course);
	for (int i = 0; i < max; i++) {
		strAsNum += course[i];
	}
	return (strAsNum + atoi(studentID)) % TABLE_LENGTH;
}

int isTakenCSG(CSGTable table, int index) {
	printf("inside istaken\n");
	if (table[index] == NULL) {
		printf("returning false\n");
		return false;
	} else {
		return true;
	}
}

int CSGHash_id(char *studentID) {
	return atoi(studentID) % TABLE_LENGTH;
}

int CSGHash_course(char *course) {
	int strAsNum = 0;
	int max = strlen(course);
	for (int i = 0; i < max; i++) {
		strAsNum += course[i];
	}
	return strAsNum % TABLE_LENGTH;
}

void insert_CSG(CSGBase *database, char *course, char *studentID, char *grade) {
	printf("%s\n", "inside insert method");
	
	CSG *toAdd = CSG_new(course, studentID, grade);
	printf("%s\n", "created CSG");
	CSG_add(&(database -> table), CSGHash(course, studentID), toAdd, 1);
	CSG_add(&(database -> idIndex), CSGHash_id(studentID), toAdd, 2);
	CSG_add(&(database -> courseIndex), CSGHash_course(course), toAdd, 3);
}

void CSG_add(CSGTable *table, int index, CSG *toAdd, int kind) {
	if (!isTakenCSG(*table, index)){
		*table[index] = toAdd;
		printf("%s %d\n", "hashed successfully with number ", index);
		printf("%s\n", (*table[index]) -> course);
	} else {
		CSG *test = *table[index];
		while(true){
			if (kind == 1) {
				if (test -> next == NULL) {
					test -> next = toAdd;
					printf("%s %d\n", "hashed successfully with number ", index);
					break;
				} else {
					test = test -> next;
				}
			} else if (kind == 2) {
				if (test -> idIndexNext == NULL) {
					test -> idIndexNext = toAdd;
					printf("%s %d\n", "hashed successfully with number ", index);
					break;
				} else {
					test = test -> idIndexNext;
				}
			} else {
				if (test -> courseIndexNext == NULL) {
					test -> courseIndexNext = toAdd;
					printf("%s %d\n", "hashed successfully with number ", index);
					break;
				} else {
					test = test -> courseIndexNext;
				}
			}   
			
		}
	}
}




