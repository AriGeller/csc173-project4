/*
 * File: Driver.c
 * Creator: Ari Geller and Adam Kravitz
 *
 * Implementation of a Relational Database
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Database.h"


void writeDatabase(CSGBaseList *CSGs, SNAPBaseList *SNAPs, CPBaseList *CPs, CDHBaseList *CDHs, CRBaseList *CRs);
void readDatabase(CSGBaseList *CSGs, SNAPBaseList *SNAPs, CPBaseList *CPs, CDHBaseList *CDHs, CRBaseList *CRs);
void findGrade(CSGBase *CSGdatabase, SNAPBase *SNAPDatabase, char *name, char *course);
void findRoom(CSGBase *CSGDatabase, SNAPBase *SNAPDatabase, CDHBase *CDHDatabase, CRBase *CRDatabase, char *name, char *hour, char *day);

int main(int argc, char const *argv[])
{
	CSGBaseList *CSGs = CSGBaseList_new();
	SNAPBaseList *SNAPs = SNAPBaseList_new();
	CPBaseList *CPs = CPBaseList_new();
	CDHBaseList *CDHs = CDHBaseList_new();
	CRBaseList *CRs = CRBaseList_new();

	readDatabase(CSGs, SNAPs, CPs, CDHs, CRs);
	//printList_CSG(lookup_CSG(CSGs -> first -> data, "CS101", "*", "*"));

	
	// CSGBase *myCSGDatabase = CSGBase_new();
	// CSGBaseList_add(CSGs, myCSGDatabase);

	// insert_CSG(myCSGDatabase, "CS101", "12345", "A");
	// insert_CSG(myCSGDatabase, "CS101", "67890", "B");
	// insert_CSG(myCSGDatabase, "EE200", "12345", "C");
	// insert_CSG(myCSGDatabase, "EE200", "22222", "B+");
	// insert_CSG(myCSGDatabase, "CS101", "33333", "A-");
	// insert_CSG(myCSGDatabase, "PH100", "67890", "C+");
	// insert_CSG(myCSGDatabase, "CS101", "12346", "A");
	// printList_CSG(lookup_CSG(myCSGDatabase, "*", "*", "*"));
	// //delete_CSG(myCSGDatabase, "CS101", "12345", "*");
	// printList_CSG(lookup_CSG(myCSGDatabase, "CS101", "12345", "*"));
	// printList_CSG(lookup_CSG(myCSGDatabase, "CS101", "*", "F"));
	
	// SNAPBase *mySNAPDatabase = SNAPBase_new();
	// SNAPBaseList_add(SNAPs, mySNAPDatabase);
	// insert_SNAP(mySNAPDatabase, "12345", "C. Brown", "12 Apple St.", "555-1234");
	// insert_SNAP(mySNAPDatabase, "67890", "L. Van Pelt", "34 Pear St.", "555-6789");
	// insert_SNAP(mySNAPDatabase, "22222", "P. Patty", "56 Grape Blvd.", "555-9999");
	// printList_SNAP(lookup_SNAP(mySNAPDatabase, "*", "C. Brown"));


	// CPBase *myCPDatabase = CPBase_new();
	// CPBaseList_add(CPs, myCPDatabase);
	// insert_CP(myCPDatabase, "CS101", "CS100");
	// insert_CP(myCPDatabase, "EE200", "EE005");
	// insert_CP(myCPDatabase, "EE200", "CS100");
	// insert_CP(myCPDatabase, "CS120", "CS101");
	// insert_CP(myCPDatabase, "CS121", "CS120");
	// insert_CP(myCPDatabase, "CS205", "CS101");
	// insert_CP(myCPDatabase, "CS206", "CS121");
	// insert_CP(myCPDatabase, "CS206", "CP205");
	// printList_CP(lookup_CP(myCPDatabase, "*"));


	// CDHBase *myCDHDatabase = CDHBase_new();
	// CDHBaseList_add(CDHs, myCDHDatabase);
	// insert_CDH(myCDHDatabase, "CS101", "M", "9AM");
	// insert_CDH(myCDHDatabase, "CS101", "W", "9AM");
	// insert_CDH(myCDHDatabase, "CS101", "F", "9AM");
	// insert_CDH(myCDHDatabase, "EE200", "Tu", "10AM");
	// insert_CDH(myCDHDatabase, "EE200", "W", "1PM");
	// insert_CDH(myCDHDatabase, "EE200", "Th", "10AM");
	// printList_CDH(lookup_CDH(myCDHDatabase, "*", "*", "*"));

	// CRBase *myCRDatabase = CRBase_new();
	// CRBaseList_add(CRs, myCRDatabase);
	// insert_CR(myCRDatabase, "CS101", "Turing Atd.");
	// insert_CR(myCRDatabase, "EE200", "25 Ohm Hall");
	// insert_CR(myCRDatabase, "PH100", "Newton Lab.");
	// printList_CR(lookup_CR(myCRDatabase, "*"));



	// findGrade(CSGs -> first -> data, SNAPs -> first -> data, "C. Brown", "CS101");
	// findGrade(CSGs -> first -> data, SNAPs -> first -> data, "C. Brown", "CH200");
	// findGrade(CSGs -> first -> data, SNAPs -> first -> data, "A. Geller", "CS101");
	//findRoom(CSGs -> first -> data, SNAPs -> first -> data, CDHs -> first -> data, CRs -> first -> data, "C. Brown", "9AM","M");
	//findRoom(CSGs -> first -> data, SNAPs -> first -> data, CDHs -> first -> data, CRs -> first -> data, "P. Patty", "10AM","Th");
	
	GenRelList *CSGTest = convert_CSG(CSGs -> first -> data);
	GenRelList *SNAPTest = convert_SNAP(SNAPs -> first -> data);
	printf("Projection of course:\n");
	GenRelList_print(project(CSGTest, "course"));
	printf("Selection of all CS101 students:\n");
	GenRelList_print(select(CSGTest, "course", "CS101"));
	printf("Join of CSG and SNAP:\n");
	GenRelList_print(join(CSGTest, SNAPTest, "studentID"));


	writeDatabase(CSGs, SNAPs, CPs, CDHs, CRs);
	

}


void writeDatabase(CSGBaseList *CSGs, SNAPBaseList *SNAPs, CPBaseList *CPs, CDHBaseList *CDHs, CRBaseList *CRs) {
	FILE *fp;
	fp = fopen("database.txt", "w");
	fputs("CSG\n", fp);
	CSGBaseListNode *CSGNode = CSGs -> first;
	while (CSGNode != NULL) {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (CSGNode -> data -> table[i] != NULL) {
				CSG *toWrite = CSGNode -> data -> table[i];
				while (toWrite != NULL) {
					fprintf(fp, "%s ", toWrite -> course);
					fprintf(fp, "%s ", toWrite -> studentID);
					fprintf(fp, "%s\n", toWrite -> grade);
					toWrite = toWrite -> next;
				}
			}
		
		}
		fputs("NEXT NEXT NEXT\n", fp);
		CSGNode = CSGNode -> next;
	}
	fputs("?\n", fp);
	fputs("SNAP\n", fp);
	SNAPBaseListNode *SNAPNode = SNAPs -> first;
	while (SNAPNode != NULL) {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (SNAPNode -> data -> table[i] != NULL) {
				SNAP *toWrite = SNAPNode -> data -> table[i];
				while (toWrite != NULL) {
					for (int i = 0; i < strlen(toWrite -> name); i++) {
						if (toWrite -> name[i] == ' ') {
							toWrite -> name[i] = '@';
						}
					}
					for (int i = 0; i < strlen(toWrite -> address); i++) {
						if (toWrite -> address[i] == ' ') {
							toWrite -> address[i] = '@';
						}
					}
					fprintf(fp, "%s ", toWrite -> studentID);
					fprintf(fp, "%s ", toWrite -> name);
					fprintf(fp, "%s ", toWrite -> address);
					fprintf(fp, "%s\n", toWrite -> phone);
					toWrite = toWrite -> next;
				}
			}
		}
		fputs("NEXT NEXT NEXT NEXT\n", fp);
		SNAPNode = SNAPNode -> next;
	}
	fputs("?\n", fp);
	fputs("CP\n", fp);
	CPBaseListNode *CPNode = CPs -> first;
	while (CPNode != NULL) {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (CPNode -> data -> table[i] != NULL) {
				CP *toWrite = CPNode -> data -> table[i];
				while (toWrite != NULL) {
					fprintf(fp, "%s ", toWrite -> course);
					fprintf(fp, "%s\n", toWrite -> prereq);
					toWrite = toWrite -> next;
				}
			}
		}
		fputs("NEXT NEXT\n", fp);
		CPNode = CPNode -> next;
	}
	fputs("?\n", fp);
	fputs("CDH\n", fp);
	CDHBaseListNode *CDHNode = CDHs -> first;
	while (CDHNode != NULL) {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (CDHNode -> data -> table[i] != NULL) {
				CDH *toWrite = CDHNode -> data -> table[i];
				while (toWrite != NULL) {
					fprintf(fp, "%s ", toWrite -> course);
					fprintf(fp, "%s ", toWrite -> day);
					fprintf(fp, "%s\n", toWrite -> hour);
					toWrite = toWrite -> next;
				}
			}
		}
		fputs("NEXT NEXT NEXT\n", fp);
		CDHNode = CDHNode -> next;
	}
	fputs("?\n", fp);
	fputs("CR\n", fp);
	CRBaseListNode *CRNode = CRs -> first;
	while (CRNode != NULL) {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (CRNode -> data -> table[i] != NULL) {
				CR *toWrite = CRNode -> data -> table[i];
				while (toWrite != NULL) {
					for (int i = 0; i < strlen(toWrite -> room); i++){
						if (toWrite -> room[i] == ' ') {
							toWrite -> room[i] = '@';
						}
					}
					fprintf(fp, "%s ", toWrite -> course);
					fprintf(fp, "%s\n", toWrite -> room);
					toWrite = toWrite -> next;
				}
			}
		}
		fputs("NEXT NEXT\n", fp);
		CRNode = CRNode -> next;
	}
	fputs("?\n", fp);

}


void readDatabase(CSGBaseList *CSGs, SNAPBaseList *SNAPs, CPBaseList *CPs, CDHBaseList *CDHs, CRBaseList *CRs) {
	FILE *fp;
	fp = fopen("database.txt", "r");
	char *input = (char*)calloc(20, sizeof(char));
	fscanf(fp, "%s\n", input); //should be "CSG"
	while (strcmp(input, "?") != 0) {
		CSGBase *base = CSGBase_new();
		while(strcmp(input, "NEXT") != 0) {
			char *course = (char*)calloc(5, sizeof(char));
			char *studentID = (char*)calloc(5, sizeof(char));
			char *grade = (char*)calloc(2, sizeof(char));
			fscanf(fp, "%s %s %s\n", course, studentID, grade);		
			if (strcmp(course, "NEXT") == 0){
				fscanf(fp, "\n");
				input = course;
				continue;
			}
			insert_CSG(base, course, studentID, grade);
		}
		CSGBaseList_add(CSGs, base);
		char *c = (char*)malloc(sizeof(char));
		fscanf(fp, "%c", c);
		if (*c == '?') {
			input = c;
		}  else {
			ungetc(*c, fp);
			input = "a";
		}
		
	}
	fscanf(fp, "%s\n", input);
	while (strcmp(input, "?") != 0) {
		SNAPBase *base = SNAPBase_new();
		while(strcmp(input, "NEXT") != 0) {
			char *studentID = (char*)calloc(5, sizeof(char));
			char *name = (char*)calloc(20, sizeof(char));
			char *address = (char*)calloc(20, sizeof(char));
			char *phone = (char*)calloc(10, sizeof(char));
			fscanf(fp, "%s %s %s %s\n", studentID, name, address, phone);		
			if (strcmp(studentID, "NEXT") == 0){
				fscanf(fp, "\n");
				input = studentID;
				continue;
			}
			for (int i = 0; i < strlen(name); i++) {
				if (name[i] == '@') {
					name[i] = ' ';
				}
			}
			for (int i = 0; i < strlen(address); i++) {
				if (address[i] == '@') {
					address[i] = ' ';
				}
			}
			insert_SNAP(base, studentID, name, address, phone);
		}
		SNAPBaseList_add(SNAPs, base);
		char *c = (char*)malloc(sizeof(char));
		fscanf(fp, "%c", c);
		if (*c == '?') {
			input = c;
		}  else {
			ungetc(*c, fp);
			input = "a";
		}
		
	}
	fscanf(fp, "%s\n", input);
	while (strcmp(input, "?") != 0) {
		CPBase *base = CPBase_new();
		while(strcmp(input, "NEXT") != 0) {
			char *course = (char*)calloc(5, sizeof(char));
			char *prereq = (char*)calloc(5, sizeof(char));
			fscanf(fp, "%s %s\n", course, prereq);		
			if (strcmp(course, "NEXT") == 0){
				fscanf(fp, "\n");
				input = course;
				continue;
			}
			insert_CP(base, course, prereq);
		}
		CPBaseList_add(CPs, base);
		char *c = (char*)malloc(sizeof(char));
		fscanf(fp, "%c", c);
		if (*c == '?') {
			input = c;
		}  else {
			ungetc(*c, fp);
			input = "a";
		}
		
	}
	fscanf(fp, "%s\n", input);
	while (strcmp(input, "?") != 0) {
		CDHBase *base = CDHBase_new();
		while(strcmp(input, "NEXT") != 0) {
			char *course = (char*)calloc(5, sizeof(char));
			char *day = (char*)calloc(2, sizeof(char));
			char *hour = (char*)calloc(4, sizeof(char));
			fscanf(fp, "%s %s %s\n", course, day, hour);		
			if (strcmp(course, "NEXT") == 0){
				fscanf(fp, "\n");
				input = course;
				continue;
			}
			insert_CDH(base, course, day, hour);
		}
		CDHBaseList_add(CDHs, base);
		char *c = (char*)malloc(sizeof(char));
		fscanf(fp, "%c", c);
		if (*c == '?') {
			input = c;
		}  else {
			ungetc(*c, fp);
			input = "a";
		}
		
	}
	fscanf(fp, "%s\n", input);
	while (strcmp(input, "?") != 0) {
		CRBase *base = CRBase_new();
		while(strcmp(input, "NEXT") != 0) {
			char *course = (char*)calloc(5, sizeof(char));
			char *room = (char*)calloc(20, sizeof(char));
			fscanf(fp, "%s %s\n", course, room);		
			if (strcmp(course, "NEXT") == 0){
				fscanf(fp, "\n");
				input = course;
				continue;
			}
			for (int i = 0; i < strlen(room); i++) {
				if (room[i] == '@') {
					room[i] = ' ';
				}
			}
			insert_CR(base, course, room);
		}
		CRBaseList_add(CRs, base);
		char *c = (char*)malloc(sizeof(char));
		fscanf(fp, "%c", c);
		if (*c == '?') {
			input = c;
		}  else {
			ungetc(*c, fp);
			input = "a";
		}
		
	}
	fscanf(fp, "%s\n", input);
}

void findGrade(CSGBase *CSGDatabase, SNAPBase *SNAPDatabase, char *name, char *course) {
	SNAPLinkedList *tempSnap = lookup_SNAP(SNAPDatabase, "*", name);
	if (tempSnap -> first != NULL) {
		SNAPLinkedListNode *node = tempSnap -> first;
		while(strcmp(node -> data -> name, name) != 0) {
			node = node -> next;
		}
		char *id = node -> data -> studentID;
		CSGLinkedList *tempCSG = lookup_CSG(CSGDatabase, course, id, "*");
		if (tempCSG -> first != NULL) {
			char *grade = tempCSG -> first -> data -> grade;
			printf("%s got a grade of %s in %s.\n", name, grade, course);
		} else {
			printf("Sorry, %s never took %s.\n", name, course);
		}
	} else {
		printf("Sorry, we have no record of a student named %s.\n", name);
	}
	

}


void findRoom(CSGBase *CSGDatabase, SNAPBase *SNAPDatabase, CDHBase *CDHDatabase, CRBase *CRDatabase, char *name, char *hour,char *day) {
	SNAPLinkedList *tempSnap = lookup_SNAP(SNAPDatabase, "*", name);
	if (tempSnap -> first != NULL) {
		char *id = tempSnap -> first -> data -> studentID;
		CDHLinkedList *tempCDH = lookup_CDH(CDHDatabase, "*", day, hour);
		if (tempCDH -> first != NULL) {
			char *course = tempCDH -> first -> data ->course;
			CSGLinkedList *tempCSG = lookup_CSG(CSGDatabase, course , id, "*");
			if (tempCSG -> first != NULL) {
				char *course = tempCSG -> first -> data -> course;
				CRLinkedList *tempCR = lookup_CR(CRDatabase, course);
				if (tempCR -> first != NULL) {
					char *room = tempCR -> first -> data -> room;
					printf("%s is in %s room at %s on %s.\n", name, room, hour, day);
				} else {
					printf("Sorry, %s never took %s.\n", name, hour);
				}
			} else {
				printf("Sorry, %s never took %s.\n", name, hour);
			}
		} else {
			printf("Sorry, %s never took %s.\n", name, hour);
		}
	} else {
		printf("Sorry, we have no record of a student named %s.\n", name);
	}
	

}
