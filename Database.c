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


*CSGList CSGList_new(){
	CSGList *new = (CSGList)calloc(TABLE_LENGTH, sizeof(CSG));
	return new;
}

*CSG CSG_new(char *course, int studentID, char *grade) {
	CSG *new = (CSG*)malloc(sizeof(CSG));
	new -> course = course;
	new -> studentID = studentID;
	new -> grade = grade;
	new -> next = NULL;
}

int CSGHash(char* course, int studentID) {
	int strAsNum = 0;
	int max = strlen(course)
	for (int i = 0; i < max; i++) {
		strAsNum += course[i];
	}

	return (strAsNum + studentID) % TABLE_LENGTH;
}

int isTakenCSG(CSGTable *table, int index) {
	if (table[index] == NULL) {
		return false;
	} else {
		return true;
	}
}

void insert_CSG(CSGTable *table, char *course, int studentID, char *grade) {
	CSG *toAdd = CSG_new(course, studentID, grade);
	int index = CSGHash(course, studentID);
	if(!isTakenCSG(table, index)){
		table[index] = toAdd;
	}
}


