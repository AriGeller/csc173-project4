/*
 * File: Driver.c
 * Creator: Ari Geller and Adam Kravitz
 *
 * Implementation of a Relational Database
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Database.h"


int main(int argc, char const *argv[])
{
	CSGBase *myCSGDatabase = CSGBase_new();

	insert_CSG(myCSGDatabase, "CS101", "12345", "A");
	insert_CSG(myCSGDatabase, "CS101", "67890", "B");
	insert_CSG(myCSGDatabase, "EE200", "12345", "C");
	insert_CSG(myCSGDatabase, "EE200", "22222", "B+");
	insert_CSG(myCSGDatabase, "CS101", "33333", "A-");
	insert_CSG(myCSGDatabase, "PH100", "67890", "C+");
	insert_CSG(myCSGDatabase, "CS101", "12346", "A");
	printList_CSG(lookup_CSG(myCSGDatabase, "*", "*", "*"));
	delete_CSG(myCSGDatabase, "CS101", "12345", "*");
	printList_CSG(lookup_CSG(myCSGDatabase, "CS101", "12345", "*"));
	printList_CSG(lookup_CSG(myCSGDatabase, "CS101", "*", "F"));
	SNAPBase *mySNAPDatabase = SNAPBase_new();
	insert_SNAP(mySNAPDatabase, "12345", "C. Brown", "12 Apple St.", "555-1234");
	insert_SNAP(mySNAPDatabase, "67890", "L. Van Pelt", "34 Pear St.", "555-6789");
	insert_SNAP(mySNAPDatabase, "22222", "P. Patty", "56 Grape Blvd.", "555-9999");
	printList_SNAP(lookup_SNAP(mySNAPDatabase, "*", "C. Brown"));


	CPBase *myCPDatabase = CPBase_new();
	insert_CP(myCPDatabase, "CS101", "CS100");
	insert_CP(myCPDatabase, "EE200", "EE005");
	insert_CP(myCPDatabase, "EE200", "CS100");
	insert_CP(myCPDatabase, "CS120", "CS101");
	insert_CP(myCPDatabase, "CS121", "CS120");
	insert_CP(myCPDatabase, "CS205", "CS101");
	insert_CP(myCPDatabase, "CS206", "CS121");
	insert_CP(myCPDatabase, "CS206", "CP205");
	printList_CP(lookup_CP(myCPDatabase, "*"));


	CDHBase *myCDHDatabase = CDHBase_new();
	insert_CDH(myCDHDatabase, "CS101", "M", "9AM");
	insert_CDH(myCDHDatabase, "CS101", "W", "9AM");
	insert_CDH(myCDHDatabase, "CS101", "F", "9AM");
	insert_CDH(myCDHDatabase, "EE200", "Tu", "10AM");
	insert_CDH(myCDHDatabase, "EE200", "W", "1PM");
	insert_CDH(myCDHDatabase, "EE200", "Th", "10AM");
	printList_CDH(lookup_CDH(myCDHDatabase, "*", "*", "*"));

	CRBase *myCRDatabase = CRBase_new();
	insert_CR(myCRDatabase, "CS101", "Turing Atd.");
	insert_CR(myCRDatabase, "EE200", "25 Ohm Hall");
	insert_CR(myCRDatabase, "PH100", "Newton Lab.");
	printList_CR(lookup_CR(myCRDatabase, "*"));

}

