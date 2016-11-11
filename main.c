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
	CSGBase *myDatabase = CSGBase_new();

	insert_CSG(myDatabase, "CS101", "12345", "A");
	insert_CSG(myDatabase, "CS101", "67890", "B");
	insert_CSG(myDatabase, "EE200", "12345", "C");
	insert_CSG(myDatabase, "EE200", "22222", "B+");
	insert_CSG(myDatabase, "CS101", "33333", "A-");
	insert_CSG(myDatabase, "PH100", "67890", "C+");
	insert_CSG(myDatabase, "CS101", "12346", "A");
	printList_CSG(lookup_CSG(myDatabase, "*", "*", "*"));
	delete_CSG(myDatabase, "CS101", "12345", "*");
	printList_CSG(lookup_CSG(myDatabase, "CS101", "12345", "*"));
}