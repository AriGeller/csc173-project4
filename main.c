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
	printf("%s\n", myDatabase -> table[533]-> course);
	
}