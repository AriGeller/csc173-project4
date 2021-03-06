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
#include "LinkedList.h"


//methods for Basic CSG

//method that creates and mallocs a new CSGBase
CSGBase *CSGBase_new(){
	CSGBase *new = (CSGBase*)malloc(sizeof(CSGBase));
	return new;
}

//method that creates and mallocs a new CSG
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
	for (int i = 0; i < 2; i++) {
		new -> grade[i] = grade[i];
	}
	new -> grade[2] = '\0';
	
	new -> next = NULL;
	new -> idIndexNext = NULL;
	new -> courseIndexNext = NULL;

	return new;
}

//method that creates and mallocs a new CSGLinkedList
CSGLinkedList *CSGLinkedList_new()  {
    CSGLinkedList *list = (CSGLinkedList*)malloc(sizeof(CSGLinkedList));
    list->first = list->last = NULL;
    return list;
}

//method that creates and mallocs a new CSGLinkedListNode
CSGLinkedListNode *CSGLinkedListNode_new(CSG *data) {
    CSGLinkedListNode *node = (CSGLinkedListNode*)malloc(sizeof(CSGLinkedListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

//adds a CSG to a CSGLinked List
void CSGLinkedList_add(CSGLinkedList *list, CSG *data) {
	CSGLinkedListNode *node = CSGLinkedListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}

//hash function for primary index of CSG table
int CSGHash(char* course, char *studentID) {
	int strAsNum = 0;
	int max = strlen(course);
	for (int i = 0; i < max; i++) {
		strAsNum += course[i];
	}
	return (strAsNum + atoi(studentID)) % TABLE_LENGTH;
}

//function to check if a bucket has at least one item in it
int isTakenCSG(CSGTable table, int index) {
	if (table[index] == NULL) {
		return false;
	} else {
		return true;
	}
}

//hash function for secondary CSG index on student id
int CSGHash_id(char *studentID) {
	return atoi(studentID) % TABLE_LENGTH;
}

//hash function for secondary CSG index on course
int CSGHash_course(char *course) {
	int strAsNum = 0;
	int max = strlen(course);
	for (int i = 0; i < max; i++) {
		strAsNum += course[i];
	}
	return strAsNum % TABLE_LENGTH;
}

//function to add CSGs to the database (primary and secondary indices))
void insert_CSG(CSGBase *database, char *course, char *studentID, char *grade) {
	
	CSG *toAdd = CSG_new(course, studentID, grade);
	if(CSG_add(database, CSGHash(course, studentID), toAdd, 1)) {
		//only attempts to add to the seconary indices if it was successfully added to primary
		CSG_add(database, CSGHash_id(studentID), toAdd, 2);
		CSG_add(database, CSGHash_course(course), toAdd, 3);
	}
	
}

//adds CSGs to a table. returns true if successful
bool CSG_add(CSGBase *database, int index, CSG *toAdd, int kind) {
	if (kind == 1) {
		if(!isTakenCSG(database -> table, index)) {
			database -> table[index] = toAdd;
			printf("Added student %s in course %s with grade %s.\n", toAdd -> studentID, toAdd -> course, toAdd -> grade);
			return true;
		} else {
			CSG *test = database -> table[index];
			while(true) {
				if (test -> next == NULL) {
					if (strcmp(test -> course, toAdd -> course) == 0 && strcmp(test -> studentID, toAdd -> studentID) == 0){
						printf("%s\n", "that record is already in the database.");
						return false;
					} else {
						test -> next = toAdd;
						printf("Added student %s in course %s with grade %s.\n", toAdd -> studentID, toAdd -> course, toAdd -> grade);
						return true;
					}
					
				} else {
					test = test -> next;

				}
			}
		}
	} else if (kind == 2) {
		if(!isTakenCSG(database -> idIndex, index)) {
			database -> idIndex[index] = toAdd;
		} else {
			CSG *test = database -> idIndex[index];
			while(true) {
				
				if (test -> idIndexNext == NULL) {
					test -> idIndexNext = toAdd;
					break;
				} else {
					test = test -> idIndexNext;
				}
			}
		}
	} else {
		if(!isTakenCSG(database -> courseIndex, index)) {
			database -> courseIndex[index] = toAdd;
		} else {
			CSG *test = database -> courseIndex[index];
			while (true) {
				if (test -> courseIndexNext == NULL) {
					test -> courseIndexNext = toAdd;
					break;
				} else {
					test = test -> courseIndexNext;
				}
			}
		}
	}

	return true;
}

//looks up tuples from CSG table, returns all found as a linked list
CSGLinkedList *lookup_CSG(CSGBase *database, char *course, char *studentID, char *grade) {
	CSGLinkedList *list = CSGLinkedList_new();
	bool courseIsStar = false;
	bool idIsStar = false;
	bool gradeIsStar = false;
	if (strcmp(course, "*") == 0) {
		courseIsStar = true;
	}
	if (strcmp(studentID, "*") == 0) {
		idIsStar = true;
	}
	if (strcmp(grade, "*") == 0) {
		gradeIsStar = true;
	}
	if (!courseIsStar && !idIsStar) {
		int index = CSGHash(course, studentID);
		if (database -> table[index] == NULL) {
		} else {
			CSG *test = database -> table[index];
			while (test != NULL) {
				if (strcmp(test -> course, course) == 0 && strcmp(test -> studentID, studentID) == 0) {
					CSGLinkedList_add(list, test);	
				}
				test = test -> next;
			}
		}
	} else if (!courseIsStar && idIsStar && gradeIsStar) {
		int index = CSGHash_course(course);
		if (database -> courseIndex[index] == NULL) {
		} else {
			CSG *test = database -> courseIndex[index];
			while(test != NULL) {
				if (strcmp(test -> course, course) == 0) {
					CSGLinkedList_add(list, test);
					
				} 
				test = test -> courseIndexNext;
			}
		}
	} else if (!courseIsStar && idIsStar && !gradeIsStar) {
		int index = CSGHash_course(course);
		if (database -> courseIndex[index] == NULL) {
		} else {
			CSG *test = database -> courseIndex[index];
			while(test != NULL) {
				if (strcmp(test -> course, course) == 0 && strcmp(test -> grade, grade) == 0) {
					CSGLinkedList_add(list, test);
				} 
				test = test -> courseIndexNext;
			}
		}
	} else if (courseIsStar && !idIsStar && gradeIsStar) {
		int index = CSGHash_id(studentID);
		if (database -> idIndex[index] == NULL) {
		} else {
			CSG *test = database -> idIndex[index];
			while (test != NULL) {
				if (strcmp(test -> studentID, studentID) == 0) {
					CSGLinkedList_add(list, test);
				}
				test = test -> idIndexNext;
			}
		}
	} else if (courseIsStar && !idIsStar && !gradeIsStar) {
		int index = CSGHash_id(studentID);
		if (database -> idIndex[index] == NULL) {
		} else {
			CSG *test = database -> idIndex[index];
			while (test != NULL) {
				if (strcmp(test -> studentID, studentID) == 0 && strcmp(test -> grade, grade) == 0) {
					CSGLinkedList_add(list, test);
				}
			}
		}
	} else if (courseIsStar && idIsStar && gradeIsStar) {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (database -> table[i] != NULL) {
				CSG *toAdd = database -> table[i];
				while(toAdd != NULL) {
					CSGLinkedList_add(list, toAdd);
					toAdd = toAdd -> next;
				}
				
			}
		}


	} else {
		printf("%s\n", "Wrong input");
	}
	return list;
}

void printList_CSG(CSGLinkedList *list) {
	if (list -> first == NULL) {
		printf("Sorry, no data matched your search.\n");

	} else {
		for (CSGLinkedListNode *node = list -> first; node != NULL; node = node -> next) {
			printf("Student %s got a grade of %s in course %s.\n", node -> data -> studentID, node -> data -> grade, node -> data -> course);
		}
	}
	
}

//deletes tuple from a CSG table and indices
void delete_CSG(CSGBase *database, char *course, char *studentID, char *grade) {
	bool courseIsStar = false;
	bool idIsStar = false;
	bool gradeIsStar = false;
	if (strcmp(course, "*") == 0) {
		courseIsStar = true;
	}
	if (strcmp(studentID, "*") == 0) {
		idIsStar = true;
	}
	if (strcmp(grade, "*") == 0) {
		gradeIsStar = true;
	}
	if (!courseIsStar && !idIsStar) {
		int index = CSGHash(course, studentID);
		if (database -> table[index] == NULL) {
			printf("%s\n", "Sorry, there was no info to delete.");
		} else {
			CSG *test = database -> table[index];
			if (strcmp(test -> course, course) == 0 && strcmp(test -> studentID, studentID) == 0) {
				if (test -> next != NULL) {
					database -> table[index] = test -> next;
				} else {
					database -> table[index] = NULL;
				}
				deleteFromIndices_CSG(database, test -> course, test -> studentID, test -> grade);
				printf("Deleted record of student %s in course %s with grade %s.\n", test -> studentID, test -> course, test -> grade);
			} 
			while (test -> next != NULL) {
				test = test -> next;
				if (strcmp(test -> next -> course, course) == 0 && strcmp(test -> next -> studentID, studentID) == 0) {
					deleteFromIndices_CSG(database, test -> next -> course, test -> next -> studentID, test -> next -> grade);
					printf("Deleted record of student %s in course %s with grade %s.\n", test -> next -> studentID, test -> next -> course, test -> next -> grade);
					if (test -> next -> next == NULL) {
						test -> next = NULL;
					} else {
						test -> next = test -> next -> next;
					}
				}
			}
		}
	} else {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (database -> table[i] != NULL) {
				CSG *test = database -> table[i];
				if (!courseIsStar && idIsStar && gradeIsStar) {
					if (strcmp(test -> course, course) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						deleteFromIndices_CSG(database, test -> course, test -> studentID, test -> grade);
						printf("Deleted record of student %s in course %s with grade %s.\n", test -> studentID, test -> course, test -> grade);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> course, course) == 0) {
							deleteFromIndices_CSG(database, test -> next -> course, test -> next -> studentID, test -> next -> grade);
							printf("Deleted record of student %s in course %s with grade %s.\n", test -> next -> studentID, test -> next -> course, test -> next -> grade);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				} else if (courseIsStar && !idIsStar && gradeIsStar) {
					if (strcmp(test -> studentID, studentID) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						deleteFromIndices_CSG(database, test -> course, test -> studentID, test -> grade);
						printf("Deleted record of student %s in course %s with grade %s.\n", test -> studentID, test -> course, test -> grade);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> studentID, studentID) == 0) {
							deleteFromIndices_CSG(database, test -> next -> course, test -> next -> studentID, test -> next -> grade);
							printf("Deleted record of student %s in course %s with grade %s.\n", test -> next -> studentID, test -> next -> course, test -> next -> grade);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				} else if (!courseIsStar && idIsStar && !gradeIsStar) {
					if (strcmp(test -> course, course) == 0 && strcmp(test -> grade, grade) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						deleteFromIndices_CSG(database, test -> course, test -> studentID, test -> grade);
						printf("Deleted record of student %s in course %s with grade %s.\n", test -> studentID, test -> course, test -> grade);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> course, course) == 0 && strcmp(test -> next -> grade, grade) == 0) {
							deleteFromIndices_CSG(database, test -> next -> course, test -> next -> studentID, test -> next -> grade);
							printf("Deleted record of student %s in course %s with grade %s.\n", test -> next -> studentID, test -> next -> course, test -> next -> grade);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				} else if (courseIsStar && !idIsStar && !gradeIsStar) {
					if (strcmp(test -> studentID, studentID) == 0 && strcmp(test -> grade, grade) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						deleteFromIndices_CSG(database, test -> course, test -> studentID, test -> grade);
						printf("Deleted record of student %s in course %s with grade %s.\n", test -> studentID, test -> course, test -> grade);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> studentID, studentID) == 0 && strcmp(test -> next -> grade, grade) == 0) {
							deleteFromIndices_CSG(database, test -> next -> course, test -> next -> studentID, test -> next -> grade);
							printf("Deleted record of student %s in course %s with grade %s.\n", test -> next -> studentID, test -> next -> course, test -> next -> grade);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				} else if (courseIsStar && idIsStar && gradeIsStar) {
					while (test != NULL) {
						deleteFromIndices_CSG(database, test -> course, test -> studentID, test -> grade);
						printf("Deleted record of student %s in course %s with grade %s.\n", test -> studentID, test -> course, test -> grade);
						test = test -> next;

					}
					database -> table[i] = NULL;
				} else {
					printf("%s\n", "Sorry, wrong, input.");
				}
			}
		}
	}
}

//deletes tuple specifically from indices. called from general delete
void deleteFromIndices_CSG(CSGBase *database, char *course, char *studentID, char *grade) {
	for (int i = 0; i < TABLE_LENGTH; i++) {
		if (database -> idIndex[i] != NULL) {
			CSG *idTest = database -> idIndex[i];
			if (strcmp(idTest -> course, course) == 0 && strcmp(idTest -> studentID, studentID) == 0 && strcmp(idTest -> grade, grade) == 0) {
				if (idTest -> idIndexNext != NULL) {
					database -> idIndex[i] = idTest -> idIndexNext;
				} else {
					database -> idIndex[i] = NULL;
				}
					
			}
			while (idTest -> idIndexNext != NULL) {
				if (strcmp(idTest -> idIndexNext -> course, course) == 0 && strcmp(idTest -> idIndexNext -> studentID, studentID) == 0 && strcmp(idTest -> idIndexNext -> grade, grade) == 0) {
					if (idTest -> idIndexNext -> idIndexNext == NULL) {
						idTest -> idIndexNext = NULL;
					} else {
						idTest -> idIndexNext = idTest -> idIndexNext -> idIndexNext;
					}
				}
				idTest = idTest -> idIndexNext;	
			}
		}
		if (database -> courseIndex[i] != NULL) {
			CSG *courseTest = database -> courseIndex[i];
			if (strcmp(courseTest -> course, course) == 0 && strcmp(courseTest -> studentID, studentID) == 0 && strcmp(courseTest -> grade, grade) == 0) {
				if (courseTest -> courseIndexNext != NULL) {
					database -> courseIndex[i] = courseTest -> courseIndexNext;
				} else {
					database -> courseIndex[i] = NULL;
				}
					
			}
			while (courseTest -> courseIndexNext != NULL) {
				if (strcmp(courseTest -> courseIndexNext -> course, course) == 0 && strcmp(courseTest -> courseIndexNext -> studentID, studentID) == 0 && strcmp(courseTest -> courseIndexNext -> grade, grade) == 0) {
					if (courseTest -> courseIndexNext -> courseIndexNext == NULL) {
						courseTest -> courseIndexNext = NULL;
					} else {
						courseTest -> courseIndexNext = courseTest -> courseIndexNext -> courseIndexNext;
					}
				}
				courseTest = courseTest -> courseIndexNext;
			}
		}
	}
}

//methods for a SNAP table. All functionality same as CSG.

SNAPBase *SNAPBase_new(){
	SNAPBase *new = (SNAPBase*)malloc(sizeof(SNAPBase));
	return new;
}

SNAP *SNAP_new(char *studentID, char *name, char *address, char *phone) {
	SNAP *new = (SNAP*)malloc(sizeof(SNAP));
	for (int i = 0; i < 5; i++) {
		new -> studentID[i] = studentID[i];
	}
	new -> studentID[5] = '\0';
	if (strlen(name) > 20) {
		printf("Name too long. Aborting.\n");
		return NULL;
	}
	int temp = 0;
	for (int i = 0; i < strlen(name); i++) {
		new -> name[i] = name[i];
		temp += 1;
	}
	new -> name[temp + 1] = '\0';
	if (strlen(address) > 20) {
		printf("Address too long. Aborting.\n");
		return NULL;
	}
	temp = 0;
	for (int i = 0; i < strlen(address); i ++) {
		new -> address[i] = address[i];
		temp += 1;
	}
	new -> address[temp + 1] = '\0';
	for (int i = 0; i < 10; i++) {
		new -> phone[i] = phone[i];
	}
	new -> phone[10] = '\0';
	new -> next = NULL;
	new -> nameIndexNext = NULL;
	
	return new;
}

SNAPLinkedList *SNAPLinkedList_new()  {
    SNAPLinkedList *list = (SNAPLinkedList*)malloc(sizeof(SNAPLinkedList));
    list->first = list->last = NULL;
    return list;
}

SNAPLinkedListNode *SNAPLinkedListNode_new(SNAP *data) {
    SNAPLinkedListNode *node = (SNAPLinkedListNode*)malloc(sizeof(SNAPLinkedListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

void SNAPLinkedList_add(SNAPLinkedList *list, SNAP *data) {
	SNAPLinkedListNode *node = SNAPLinkedListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}


int isTakenSNAP(SNAPTable table, int index) {
	if (table[index] == NULL) {
		return false;
	} else {
		return true;
	}
}

int SNAPHash(char *studentID) {
	return atoi(studentID) % TABLE_LENGTH;
}

int SNAPHash_name(char *name) {
	int strAsNum = 0;
	int max = strlen(name);
	for (int i = 0; i < max; i++) {
		strAsNum += name[i];
	}
	return strAsNum % TABLE_LENGTH;
}

void insert_SNAP(SNAPBase *database, char *studentID, char *name, char *address, char *phone) {
	
	SNAP *toAdd = SNAP_new(studentID, name, address, phone);
	if(SNAP_add(database, SNAPHash(studentID), toAdd, 1)) {
		SNAP_add(database, SNAPHash_name(name), toAdd, 2);
		
	}
	
}

bool SNAP_add(SNAPBase *database, int index, SNAP *toAdd, int kind) {
	if (kind == 1) {
		if(!isTakenSNAP(database -> table, index)) {
			database -> table[index] = toAdd;
			printf("Added student %s with number %s, address %s and phone %s.\n", toAdd -> name, toAdd -> studentID, toAdd -> address, toAdd -> phone);
			return true;
		} else {
			SNAP *test = database -> table[index];
			while(true) {
				if (test -> next == NULL) {
					if (strcmp(test -> studentID, toAdd -> studentID) == 0){
						printf("%s\n", "that record is already in the database.");
						return false;
					} else {
						test -> next = toAdd;
						printf("Added student %s with number %s, address %s and phone %s.\n", toAdd -> name, toAdd -> studentID, toAdd -> address, toAdd -> phone);
						return true;
					}
					
				} else {
					test = test -> next;

				}
			}
		}
	} else if (kind == 2) {
		if(!isTakenSNAP(database -> nameIndex, index)) {
			database -> nameIndex[index] = toAdd;
		} else {
			SNAP *test = database -> nameIndex[index];
			while(true) {
				
				if (test -> nameIndexNext == NULL) {
					test -> nameIndexNext = toAdd;
					break;
				} else {
					test = test -> nameIndexNext;
				}
			}
		}
	} 
	return true;
}

SNAPLinkedList *lookup_SNAP(SNAPBase *database, char *studentID, char *name) {
	SNAPLinkedList *list = SNAPLinkedList_new();
	bool nameIsStar = false;
	bool idIsStar = false;
	if (strcmp(name, "*") == 0) {
		nameIsStar = true;
	}
	if (strcmp(studentID, "*") == 0) {
		idIsStar = true;
	}
	if (!idIsStar) {
		int index = SNAPHash(studentID);
		if (database -> table[index] == NULL) {
		} else {
			SNAP *test = database -> table[index];
			while (test != NULL) {
				if (strcmp(test -> studentID, studentID) == 0) {
					SNAPLinkedList_add(list, test);	
				}
				test = test -> next;
			}
		}
	} else if (!nameIsStar) {
		int index = SNAPHash_name(name);
		if (database -> nameIndex[index] == NULL) {
		} else {
			SNAP *test = database -> nameIndex[index];
			while(test != NULL) {
				if (strcmp(test -> name, name) == 0) {
					SNAPLinkedList_add(list, test);
				} 
				test = test -> nameIndexNext;
			}
		}
	} else if (idIsStar && nameIsStar) {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (database -> table[i] != NULL) {
				SNAP *toAdd = database -> table[i];
				while(toAdd != NULL) {
					SNAPLinkedList_add(list, toAdd);
					toAdd = toAdd -> next;
				}
				
			}
		}
	} else {
		printf("%s\n", "Wrong input");
	}
	return list;
}

void printList_SNAP(SNAPLinkedList *list) {
	if (list -> first == NULL) {
		printf("Sorry, but nothing matched your search.\n");
	} else {
		for (SNAPLinkedListNode *node = list -> first; node != NULL; node = node -> next) {
			printf("Student %s has ID number %s.\nThey live at %s and their number is %s.\n", node -> data -> name, node -> data -> studentID, node -> data -> address, node -> data -> phone);
		}
	}
	
}

void delete_SNAP(SNAPBase *database, char *studentID, char *name) {
	bool nameIsStar = false;
	bool idIsStar = false;
	
	if (strcmp(name, "*") == 0) {
		nameIsStar = true;
	}
	if (strcmp(studentID, "*") == 0) {
		idIsStar = true;
	}
	if (!idIsStar) {
		int index = SNAPHash(studentID);
		if (database -> table[index] == NULL) {
			printf("%s\n", "Sorry, there was no info to delete.");
		} else {
			SNAP *test = database -> table[index];
			if (strcmp(test -> studentID, studentID) == 0) {
				if (test -> next != NULL) {
					database -> table[index] = test -> next;
				} else {
					database -> table[index] = NULL;
				}
				deleteFromIndices_SNAP(database, test -> studentID, test -> name);
				printf("Deleted record of student %s, ID number %s.\n", test -> name, test -> studentID);
			} 
			while (test -> next != NULL) {
				test = test -> next;
				if (strcmp(test -> next -> studentID, studentID) == 0) {
					deleteFromIndices_SNAP(database, test -> studentID, test -> name);
					printf("Deleted record of student %s, ID number %s.\n", test -> name, test -> studentID);
					if (test -> next -> next == NULL) {
						test -> next = NULL;
					} else {
						test -> next = test -> next -> next;
					}
				}
			}
		}
	} else {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (database -> table[i] != NULL) {
				SNAP *test = database -> table[i];
				if (!nameIsStar) {
					if (strcmp(test -> name, name) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						deleteFromIndices_SNAP(database, test -> studentID, test -> name);
						printf("Deleted record of student %s, ID number %s.\n", test -> name, test -> studentID);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> name, name) == 0) {
							deleteFromIndices_SNAP(database, test -> studentID, test -> name);
							printf("Deleted record of student %s, ID number %s.\n", test -> name, test -> studentID);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				}  else if (nameIsStar && idIsStar) {
					while (test != NULL) {
						deleteFromIndices_SNAP(database, test -> studentID, test -> name);
						printf("Deleted record of student %s, ID number %s.\n", test -> name, test -> studentID);
						test = test -> next;

					}
					database -> table[i] = NULL;
				} else {
					printf("%s\n", "Sorry, wrong, input.");
				}
			}
		}
	}
}
void deleteFromIndices_SNAP(SNAPBase *database, char *studentID, char *name) {
	for (int i = 0; i < TABLE_LENGTH; i++) {
		if (database -> nameIndex[i] != NULL) {
			SNAP *nameTest = database -> nameIndex[i];
			if (strcmp(nameTest -> name, name) == 0 && strcmp(nameTest -> studentID, studentID) == 0) {
				if (nameTest -> nameIndexNext != NULL) {
					database -> nameIndex[i] = nameTest -> nameIndexNext;
				} else {
					database -> nameIndex[i] = NULL;
				}
					
			}
			while (nameTest -> nameIndexNext != NULL) {
				if (strcmp(nameTest -> nameIndexNext -> name, name) == 0 && strcmp(nameTest -> nameIndexNext -> studentID, studentID) == 0) {
					if (nameTest -> nameIndexNext -> nameIndexNext == NULL) {
						nameTest -> nameIndexNext = NULL;
					} else {
						nameTest -> nameIndexNext = nameTest -> nameIndexNext -> nameIndexNext;
					}
				}
				nameTest = nameTest -> nameIndexNext;	
			}
		}
	}
}

//methods for CP tables. ALl functionality same as CSG.

CPBase *CPBase_new(){
	CPBase *new = (CPBase*)malloc(sizeof(CPBase));
	return new;
}

CP *CP_new(char *course, char *prereq) {
	CP *new = (CP*)malloc(sizeof(CP));
	for (int i = 0; i < 5; i++) {
		new -> course[i] = course[i];
	}
	new -> course[5] = '\0';

	for (int i = 0; i < 5; i++) {
		new -> prereq[i] = prereq[i];
	}
	new -> prereq[5] = '\0';
	new -> next = NULL;
	
	return new;
}

CPLinkedList *CPLinkedList_new()  {
    CPLinkedList *list = (CPLinkedList*)malloc(sizeof(CPLinkedList));
    list->first = list->last = NULL;
    return list;
}

CPLinkedListNode *CPLinkedListNode_new(CP *data) {
    CPLinkedListNode *node = (CPLinkedListNode*)malloc(sizeof(CPLinkedListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

void CPLinkedList_add(CPLinkedList *list, CP *data) {
	CPLinkedListNode *node = CPLinkedListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}


int isTakenCP(CPTable table, int index) {
	if (table[index] == NULL) {
		return false;
	} else {
		return true;
	}
}

int CPHash(char *course) {
	int strAsNum = 0;
	for (int i = 0; i < strlen(course); i++) {
		strAsNum += course[i];
	}
	return strAsNum % TABLE_LENGTH;
}

void insert_CP(CPBase *database, char *course, char *prereq) {
	
	CP *toAdd = CP_new(course, prereq);
	CP_add(database, CPHash(course), toAdd);
		
	
}

bool CP_add(CPBase *database, int index, CP *toAdd) {
	if(!isTakenCP(database -> table, index)) {
		database -> table[index] = toAdd;
		printf("Added prereq %s for course %s.\n", toAdd -> prereq, toAdd -> course);
		return true;
	} else {
		CP *test = database -> table[index];
		while(true) {
			if (test -> next == NULL) {
				if (strcmp(test -> course, toAdd -> course) == 0 && strcmp(test -> prereq, toAdd -> prereq) == 0){
					printf("%s\n", "That record is already in the database.");
					return false;
				} else {
					test -> next = toAdd;
					printf("Added prereq %s for course %s.\n", toAdd -> prereq, toAdd -> course);
					return true;
				}
			
			} else {
				test = test -> next;
			}
		}
	}
	return true;
}

CPLinkedList *lookup_CP(CPBase *database, char *course) {
	CPLinkedList *list = CPLinkedList_new();
	bool courseIsStar = false;
	
	if (strcmp(course, "*") == 0) {
		courseIsStar = true;
	}
	if (!courseIsStar) {
		int index = CPHash(course);
		if (database -> table[index] == NULL) {
		} else {
			CP *test = database -> table[index];
			while (test != NULL) {
				if (strcmp(test -> course, course) == 0) {
					CPLinkedList_add(list, test);	
				}
				test = test -> next;
			}
		}
	} else if (courseIsStar) {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (database -> table[i] != NULL) {
				CP *toAdd = database -> table[i];
				while(toAdd != NULL) {
					CPLinkedList_add(list, toAdd);
					toAdd = toAdd -> next;
				}
				
			}
		}
	} else {
		printf("%s\n", "Wrong input");
	}
	return list;
}

void printList_CP(CPLinkedList *list) {
	if (list -> first == NULL) {
		printf("Sorry, no results matched your search.\n");
	} else {
		for (CPLinkedListNode *node = list -> first; node != NULL; node = node -> next) {
			printf("%s is a prerequisite for %s.\n", node -> data -> prereq, node -> data -> course);
		}
	}
	
}

void delete_CP(CPBase *database, char *course, char *prereq) {
	bool courseIsStar = false;
	bool prereqIsStar = false;
	
	if (strcmp(course, "*") == 0) {
		courseIsStar = true;
	}
	if (strcmp(prereq, "*") == 0) {
		prereqIsStar = true;
	}
	if (!courseIsStar && !prereqIsStar) {
		int index = CPHash(course);
		if (database -> table[index] == NULL) {
			printf("%s\n", "Sorry, there was no info to delete.");
		} else {
			CP *test = database -> table[index];
			if (strcmp(test -> course, course) == 0 && strcmp(test -> prereq, prereq) == 0) {
				if (test -> next != NULL) {
					database -> table[index] = test -> next;
				} else {
					database -> table[index] = NULL;
				}
				printf("Deleted record of prereq %s for course %s.\n", test -> prereq, test -> course);
			} 
			while (test -> next != NULL) {
				test = test -> next;
				if (strcmp(test -> next -> course, course) == 0 && strcmp(test -> prereq, prereq) == 0) {
					printf("Deleted record of prereq %s for course %s.\n", test -> prereq, test -> course);;
					if (test -> next -> next == NULL) {
						test -> next = NULL;
					} else {
						test -> next = test -> next -> next;
					}
				}
			}
		}
	} else if (prereqIsStar) {
		int index = CPHash(course);
		if (database -> table[index] == NULL) {
			printf("%s\n", "Sorry, there was no info to delete.");
		} else {
			CP *test = database -> table[index];
			if (strcmp(test -> course, course) == 0) {
				if (test -> next != NULL) {
					database -> table[index] = test -> next;
				} else {
					database -> table[index] = NULL;
				}
				printf("Deleted record of prereq %s for course %s.\n", test -> prereq, test -> course);
			} 
			while (test -> next != NULL) {
				test = test -> next;
				if (strcmp(test -> next -> course, course) == 0) {
					printf("Deleted record of prereq %s for course %s.\n", test -> prereq, test -> course);
					if (test -> next -> next == NULL) {
						test -> next = NULL;
					} else {
						test -> next = test -> next -> next;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (database -> table[i] != NULL) {
				CP *test = database -> table[i];
				if (!prereqIsStar) {
					if (strcmp(test -> prereq, prereq) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						printf("Deleted record of prereq %s for course %s.\n", test -> prereq, test -> course);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> prereq, prereq) == 0) {
							printf("Deleted record of prereq %s for course %s.\n", test -> prereq, test -> course);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				}  else {
					while (test != NULL) {
						printf("Deleted record of prereq %s for course %s.\n", test -> prereq, test -> course);
						test = test -> next;

					}
					database -> table[i] = NULL;
				} 

			}
		}
	}
}


//methods for CDH tables. All functionality same as CSG.

CDHBase *CDHBase_new(){
	CDHBase *new = (CDHBase*)malloc(sizeof(CDHBase));
	return new;
}

CDH *CDH_new(char *course, char *day, char *hour) {
	CDH *new = (CDH*)malloc(sizeof(CDH));
	for (int i = 0; i < 5; i++) {
		new -> course[i] = course[i];
	}
	new -> course[5] = '\0';
	int temp = 0;
	for (int i = 0; i < strlen(day); i++) {
		new -> day[i] = day[i];
		temp += 1;
	}
	new -> day[temp + 1] = '\0';
	temp = 0;
	for (int i = 0; i < strlen(hour); i ++) {
		new -> hour[i] = hour[i];
		temp += 1;
	}
	new -> hour[temp + 1] = '\0';
	
	new -> next = NULL;
	new -> courseIndexNext = NULL;
	new -> dayIndexNext = NULL;

	return new;
}

CDHLinkedList *CDHLinkedList_new()  {
    CDHLinkedList *list = (CDHLinkedList*)malloc(sizeof(CDHLinkedList));
    list->first = list->last = NULL;
    return list;
}

CDHLinkedListNode *CDHLinkedListNode_new(CDH *data) {
    CDHLinkedListNode *node = (CDHLinkedListNode*)malloc(sizeof(CDHLinkedListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

void CDHLinkedList_add(CDHLinkedList *list, CDH *data) {
	CDHLinkedListNode *node = CDHLinkedListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}

int CDHHash(char* course, char *day) {
	int strAsNum = 0;
	
	for (int i = 0; i < strlen(course); i++) {
		strAsNum += course[i];
	}
	for (int i = 0; i < strlen(day); i++) {
		strAsNum += day[i];
	}
	return strAsNum  % TABLE_LENGTH;
}

int isTakenCDH(CDHTable table, int index) {
	if (table[index] == NULL) {
		return false;
	} else {
		return true;
	}
}


int CDHHash_course(char *course) {
	int strAsNum = 0;
	int max = strlen(course);
	for (int i = 0; i < max; i++) {
		strAsNum += course[i];
	}
	return strAsNum % TABLE_LENGTH;
}

int CDHHash_day(char *day) {
	int strAsNum = 0;

	for (int i = 0; i < strlen(day); i++) {
		strAsNum += day[i];
	}
	return strAsNum % TABLE_LENGTH;
}

void insert_CDH(CDHBase *database, char *course, char *day, char *hour) {
	
	CDH *toAdd = CDH_new(course, day, hour);
	if(CDH_add(database, CDHHash(course, day), toAdd, 1)) {
		CDH_add(database, CDHHash_course(course), toAdd, 2);
		CDH_add(database, CDHHash_day(day), toAdd, 3);
	}
	
}

bool CDH_add(CDHBase *database, int index, CDH *toAdd, int kind) {
	if (kind == 1) {
		if(!isTakenCDH(database -> table, index)) {
			database -> table[index] = toAdd;
			printf("Added course %s on day %s at time %s.\n", toAdd -> course, toAdd -> day, toAdd -> hour);
			return true;
		} else {
			CDH *test = database -> table[index];
			while(true) {
				if (test -> next == NULL) {
					if (strcmp(test -> course, toAdd -> course) == 0 && strcmp(test -> day, toAdd -> day) == 0){
						printf("%s\n", "that record is already in the database.");
						return false;
					} else {
						test -> next = toAdd;
						printf("Added course %s on day %s at time %s.\n", toAdd -> course, toAdd -> day, toAdd -> hour);
						return true;
					}
					
				} else {
					test = test -> next;

				}
			}
		}
	} else if (kind == 2) {
		if(!isTakenCDH(database -> courseIndex, index)) {
			database -> courseIndex[index] = toAdd;
		} else {
			CDH *test = database -> courseIndex[index];
			while(true) {
				
				if (test -> courseIndexNext == NULL) {
					test -> courseIndexNext = toAdd;
					break;
				} else {
					test = test -> courseIndexNext;
				}
			}
		}
	} else {
		if(!isTakenCDH(database -> dayIndex, index)) {
			database -> dayIndex[index] = toAdd;
		} else {
			CDH *test = database -> dayIndex[index];
			while (true) {
				if (test -> dayIndexNext == NULL) {
					test -> dayIndexNext = toAdd;
					break;
				} else {
					test = test -> dayIndexNext;
				}
			}
		}
	}

	return true;
}

CDHLinkedList *lookup_CDH(CDHBase *database, char *course, char *day, char *hour) {
	CDHLinkedList *list = CDHLinkedList_new();
	bool courseIsStar = false;
	bool dayIsStar = false;\
	bool hourIsStar = false;
	if (strcmp(course, "*") == 0) {
		courseIsStar = true;
	}
	if (strcmp(day, "*") == 0) {
		dayIsStar = true;
	}
	if (strcmp(hour, "*") == 0) {
		hourIsStar = true;
	}
	if (!courseIsStar && !dayIsStar) {
		int index = CDHHash(course, day);
		if (database -> table[index] == NULL) {
		} else {
			CDH *test = database -> table[index];
			while (test != NULL) {
				printf("loop1");
				if (strcmp(test -> course, course) == 0 && strcmp(test -> day, day) == 0) {
					CDHLinkedList_add(list, test);	
				}
				test = test -> next;
			}
		}
	} else if (!courseIsStar && dayIsStar && hourIsStar) {
		int index = CDHHash_course(course);
		if (database -> courseIndex[index] == NULL) {
		} else {
			CDH *test = database -> courseIndex[index];
			while(test != NULL) {
				printf("loop2");
				if (strcmp(test -> course, course) == 0) {
					CDHLinkedList_add(list, test);
					
				} 
				test = test -> courseIndexNext;
			}
		}
	} else if (!courseIsStar && dayIsStar && !hourIsStar) {
		int index = CDHHash_course(course);
		if (database -> courseIndex[index] == NULL) {
		} else {
			CDH *test = database -> courseIndex[index];
			while(test != NULL) {
				printf("loop3");
				if (strcmp(test -> course, course) == 0 && strcmp(test -> hour, hour) == 0) {
					CDHLinkedList_add(list, test);
				} 
				test = test -> courseIndexNext;
			}
		}
	} else if (courseIsStar && !dayIsStar && hourIsStar) {
		int index = CDHHash_day(day);
		if (database -> dayIndex[index] == NULL) {
		} else {
			CDH *test = database -> dayIndex[index];
			while (test != NULL) {
				printf("loop4");
				if (strcmp(test -> day, day) == 0) {
					CDHLinkedList_add(list, test);
				}
				test = test -> dayIndexNext;
			}
		}
	} else if (courseIsStar && !dayIsStar && !hourIsStar) {
		int index = CDHHash_day(day);
		if (database -> dayIndex[index] == NULL) {
		} else {
			CDH *test = database -> dayIndex[index];
			while (test != NULL) {
				if (strcmp(test -> day, day) == 0 && strcmp(test -> hour, hour) == 0) {
					CDHLinkedList_add(list, test);
				}
				test = test -> dayIndexNext;
			}
		}
	} else if (courseIsStar && dayIsStar && hourIsStar) {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (database -> table[i] != NULL) {
				CDH *toAdd = database -> table[i];
				while(toAdd != NULL) {
					printf("loop6");
					CDHLinkedList_add(list, toAdd);
					toAdd = toAdd -> next;
				}
				
			}
		}


	} else {
		printf("%s\n", "Wrong input");
	}
	return list;
}

void printList_CDH(CDHLinkedList *list) {
	if (list -> first == NULL) {
		printf("Sorry, no results matched your search.\n");
	} else {
		for (CDHLinkedListNode *node = list -> first; node != NULL; node = node -> next) {
			printf("%s meets at %s on day %s.\n", node -> data -> course, node -> data -> hour, node -> data -> day);
		}
	}
	
	
}

void delete_CDH(CDHBase *database, char *course, char *day, char *hour) {
	bool courseIsStar = false;
	bool dayIsStar = false;
	bool hourIsStar = false;
	if (strcmp(course, "*") == 0) {
		courseIsStar = true;
	}
	if (strcmp(day, "*") == 0) {
		dayIsStar = true;
	}
	if (strcmp(hour, "*") == 0) {
		hourIsStar = true;
	}
	if (!courseIsStar && !dayIsStar) {
		int index = CDHHash(course, day);
		if (database -> table[index] == NULL) {
			printf("%s\n", "Sorry, there was no info to delete.");
		} else {
			CDH *test = database -> table[index];
			if (strcmp(test -> course, course) == 0 && strcmp(test -> day, day) == 0) {
				if (test -> next != NULL) {
					database -> table[index] = test -> next;
				} else {
					database -> table[index] = NULL;
				}
				deleteFromIndices_CDH(database, test -> course, test -> day, test -> hour);
				printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
			} 
			while (test -> next != NULL) {
				test = test -> next;
				if (strcmp(test -> next -> course, course) == 0 && strcmp(test -> next -> day, day) == 0) {
					deleteFromIndices_CDH(database, test -> next -> course, test -> next -> day, test -> next -> hour);
					printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
					if (test -> next -> next == NULL) {
						test -> next = NULL;
					} else {
						test -> next = test -> next -> next;
					}
				}
			}
		}
	} else {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (database -> table[i] != NULL) {
				CDH *test = database -> table[i];
				if (!courseIsStar && day && hourIsStar) {
					if (strcmp(test -> course, course) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						deleteFromIndices_CDH(database, test -> course, test -> day, test -> hour);
						printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> course, course) == 0) {
							deleteFromIndices_CDH(database, test -> next -> course, test -> next -> day, test -> next -> hour);
							printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				} else if (courseIsStar && !dayIsStar && hourIsStar) {
					if (strcmp(test -> day, day) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						deleteFromIndices_CDH(database, test -> course, test -> day, test -> hour);
						printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> day, day) == 0) {
							deleteFromIndices_CDH(database, test -> next -> course, test -> next -> day, test -> next -> hour);
							printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				} else if (!courseIsStar && dayIsStar && !hourIsStar) {
					if (strcmp(test -> course, course) == 0 && strcmp(test -> hour, hour) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						deleteFromIndices_CDH(database, test -> course, test -> day, test -> hour);
						printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> course, course) == 0 && strcmp(test -> next -> hour, hour) == 0) {
							deleteFromIndices_CDH(database, test -> next -> course, test -> next -> day, test -> next -> hour);
							printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				} else if (courseIsStar && !day && !hour) {
					if (strcmp(test -> day, hour) == 0 && strcmp(test -> hour, hour) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						deleteFromIndices_CDH(database, test -> course, test -> day, test -> hour);
						printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> day, day) == 0 && strcmp(test -> next -> hour, hour) == 0) {
							deleteFromIndices_CDH(database, test -> next -> course, test -> next -> day, test -> next -> hour);
							printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				} else if (courseIsStar && day && hour) {
					while (test != NULL) {
						deleteFromIndices_CDH(database, test -> course, test -> day, test -> hour);
						printf("Deleted record of class %s on day %s at time %s.\n", test -> course, test -> day, test -> hour);
						test = test -> next;

					}
					database -> table[i] = NULL;
				} else {
					printf("%s\n", "Sorry, wrong, input.");
				}
			}
		}
	}
}
void deleteFromIndices_CDH(CDHBase *database, char *course, char *day, char *hour) {
	for (int i = 0; i < TABLE_LENGTH; i++) {
		if (database -> courseIndex[i] != NULL) {
			CDH *courseTest = database -> courseIndex[i];
			if (strcmp(courseTest -> course, course) == 0 && strcmp(courseTest -> day, day) == 0 && strcmp(courseTest -> hour, hour) == 0) {
				if (courseTest -> courseIndexNext != NULL) {
					database -> courseIndex[i] = courseTest -> courseIndexNext;
				} else {
					database -> courseIndex[i] = NULL;
				}
					
			}
			while (courseTest -> courseIndexNext != NULL) {
				if (strcmp(courseTest -> courseIndexNext -> course, course) == 0 && strcmp(courseTest -> courseIndexNext -> day, day) == 0 && strcmp(courseTest -> courseIndexNext -> hour, hour) == 0) {
					if (courseTest -> courseIndexNext -> courseIndexNext == NULL) {
						courseTest -> courseIndexNext = NULL;
					} else {
						courseTest -> courseIndexNext = courseTest -> courseIndexNext -> courseIndexNext;
					}
				}
				courseTest = courseTest -> courseIndexNext;	
			}
		}
		if (database -> dayIndex[i] != NULL) {
			CDH *dayTest = database -> dayIndex[i];
			if (strcmp(dayTest -> course, course) == 0 && strcmp(dayTest -> day, day) == 0 && strcmp(dayTest -> hour, hour) == 0) {
				if (dayTest -> dayIndexNext != NULL) {
					database -> dayIndex[i] = dayTest -> dayIndexNext;
				} else {
					database -> dayIndex[i] = NULL;
				}
					
			}
			while (dayTest -> dayIndexNext != NULL) {
				if (strcmp(dayTest -> dayIndexNext -> course, course) == 0 && strcmp(dayTest -> dayIndexNext -> day, day) == 0 && strcmp(dayTest -> dayIndexNext -> hour, hour) == 0) {
					if (dayTest -> dayIndexNext -> dayIndexNext == NULL) {
						dayTest -> dayIndexNext = NULL;
					} else {
						dayTest -> dayIndexNext = dayTest -> dayIndexNext -> dayIndexNext;
					}
				}
				dayTest = dayTest -> dayIndexNext;
			}
		}
	}
}

//methods for CR tables. All functionality same as CSG.

CRBase *CRBase_new(){
	CRBase *new = (CRBase*)malloc(sizeof(CRBase));
	return new;
}

CR *CR_new(char *course, char *room) {
	CR *new = (CR*)malloc(sizeof(CR));
	for (int i = 0; i < 5; i++) {
		new -> course[i] = course[i];
	}
	new -> course[5] = '\0';
	int temp = 0;
	for (int i = 0; i < strlen(room); i++) {
		new -> room[i] = room[i];
		temp += 1;
	}
	new -> room[temp + 1] = '\0';
	new -> next = NULL;
	
	return new;
}

CRLinkedList *CRLinkedList_new()  {
    CRLinkedList *list = (CRLinkedList*)malloc(sizeof(CRLinkedList));
    list->first = list->last = NULL;
    return list;
}

CRLinkedListNode *CRLinkedListNode_new(CR *data) {
    CRLinkedListNode *node = (CRLinkedListNode*)malloc(sizeof(CRLinkedListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

void CRLinkedList_add(CRLinkedList *list, CR *data) {
	CRLinkedListNode *node = CRLinkedListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}


int isTakenCR(CRTable table, int index) {
	if (table[index] == NULL) {
		return false;
	} else {
		return true;
	}
}

int CRHash(char *course) {
	int strAsNum = 0;
	for (int i = 0; i < strlen(course); i++) {
		strAsNum += course[i];
	}
	return strAsNum % TABLE_LENGTH;
}

void insert_CR(CRBase *database, char *course, char *room) {
	
	CR *toAdd = CR_new(course, room);
	CR_add(database, CRHash(course), toAdd);
		
	
}

bool CR_add(CRBase *database, int index, CR *toAdd) {
	if(!isTakenCR(database -> table, index)) {
		database -> table[index] = toAdd;
		printf("Added room %s for course %s.\n", toAdd -> room, toAdd -> course);
		return true;
	} else {
		CR *test = database -> table[index];
		while(true) {
			if (test -> next == NULL) {
				if (strcmp(test -> course, toAdd -> course) == 0 && strcmp(test -> room, toAdd -> room) == 0){
					printf("%s\n", "That record is already in the database.");
					return false;
				} else {
					test -> next = toAdd;
					printf("Added room %s for course %s.\n", toAdd -> room, toAdd -> course);
					return true;
				}
			
			} else {
				test = test -> next;
			}
		}
	}
	return true;
}

CRLinkedList *lookup_CR(CRBase *database, char *course) {
	CRLinkedList *list = CRLinkedList_new();
	bool courseIsStar = false;
	
	if (strcmp(course, "*") == 0) {
		courseIsStar = true;
	}
	if (!courseIsStar) {
		int index = CRHash(course);
		if (database -> table[index] == NULL) {
		} else {
			CR *test = database -> table[index];
			while (test != NULL) {
				if (strcmp(test -> course, course) == 0) {
					CRLinkedList_add(list, test);	
				}
				test = test -> next;
			}
		}
	} else if (courseIsStar) {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (database -> table[i] != NULL) {
				CR *toAdd = database -> table[i];
				while(toAdd != NULL) {
					CRLinkedList_add(list, toAdd);
					toAdd = toAdd -> next;
				}
				
			}
		}
	} else {
		printf("%s\n", "Wrong input");
	}
	return list;
}

void printList_CR(CRLinkedList *list) {
	if (list -> first == NULL) {
		printf("Sorry, no results were found.\n");
	} else{
		for (CRLinkedListNode *node = list -> first; node != NULL; node = node -> next) {
			printf("%s meets in %s.\n", node -> data -> course, node -> data -> room);
		}
	}
	
}

void delete_CR(CRBase *database, char *course, char *room) {
	bool courseIsStar = false;
	bool roomIsStar = false;
	
	if (strcmp(course, "*") == 0) {
		courseIsStar = true;
	}
	if (strcmp(room, "*") == 0) {
		roomIsStar = true;
	}
	if (!courseIsStar && !roomIsStar) {
		int index = CRHash(course);
		if (database -> table[index] == NULL) {
			printf("%s\n", "Sorry, there was no info to delete.");
		} else {
			CR *test = database -> table[index];
			if (strcmp(test -> course, course) == 0 && strcmp(test -> room, room) == 0) {
				if (test -> next != NULL) {
					database -> table[index] = test -> next;
				} else {
					database -> table[index] = NULL;
				}
				printf("Deleted record of course %s in room  %s.\n", test -> course, test -> room);
			} 
			while (test -> next != NULL) {
				test = test -> next;
				if (strcmp(test -> next -> course, course) == 0 && strcmp(test -> room, room) == 0) {
					printf("Deleted record of course %s in room  %s.\n", test -> course, test -> room);
					if (test -> next -> next == NULL) {
						test -> next = NULL;
					} else {
						test -> next = test -> next -> next;
					}
				}
			}
		}
	} else if (roomIsStar) {
		int index = CRHash(course);
		if (database -> table[index] == NULL) {
			printf("%s\n", "Sorry, there was no info to delete.");
		} else {
			CR *test = database -> table[index];
			if (strcmp(test -> course, course) == 0) {
				if (test -> next != NULL) {
					database -> table[index] = test -> next;
				} else {
					database -> table[index] = NULL;
				}
				printf("Deleted record of course %s in room  %s.\n", test -> course, test -> room);
			} 
			while (test -> next != NULL) {
				test = test -> next;
				if (strcmp(test -> next -> course, course) == 0) {
					printf("Deleted record of course %s in room  %s.\n", test -> course, test -> room);
					if (test -> next -> next == NULL) {
						test -> next = NULL;
					} else {
						test -> next = test -> next -> next;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < TABLE_LENGTH; i++) {
			if (database -> table[i] != NULL) {
				CR *test = database -> table[i];
				if (!roomIsStar) {
					if (strcmp(test -> room, room) == 0) {
						if (test -> next != NULL) {
							database -> table[i] = test -> next;
						} else {
							database -> table[i] = NULL;
						}
						printf("Deleted record of course %s in room  %s.\n", test -> course, test -> room);
					}
				
					while (test -> next != NULL) {
						test = test -> next;
						if (strcmp(test -> next -> room, room) == 0) {
							printf("Deleted record of course %s in room  %s.\n", test -> course, test -> room);
							if (test -> next -> next == NULL) {
								test -> next = NULL;
							} else {
								test -> next = test -> next -> next;
							}
						}
					}
				}  else {
					while (test != NULL) {
						printf("Deleted record of course %s in room  %s.\n", test -> course, test -> room);
						test = test -> next;

					}
					database -> table[i] = NULL;
				} 

			}
		}
	}
}
		
CSGBaseList *CSGBaseList_new() {
	CSGBaseList *list = (CSGBaseList*)malloc(sizeof(CSGBaseList));
    list->first = list->last = NULL;
    return list;
}

CSGBaseListNode *CSGBaseListNode_new(CSGBase *data) {
	CSGBaseListNode *node = (CSGBaseListNode*)malloc(sizeof(CSGBaseListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

void CSGBaseList_add(CSGBaseList *list, CSGBase *data) {
	CSGBaseListNode *node = CSGBaseListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}

SNAPBaseList *SNAPBaseList_new() {
	SNAPBaseList *list = (SNAPBaseList*)malloc(sizeof(SNAPBaseList));
    list->first = list->last = NULL;
    return list;
}

SNAPBaseListNode *SNAPBaseListNode_new(SNAPBase *data) {
	SNAPBaseListNode *node = (SNAPBaseListNode*)malloc(sizeof(SNAPBaseListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

void SNAPBaseList_add(SNAPBaseList *list, SNAPBase *data) {
	SNAPBaseListNode *node = SNAPBaseListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}

CPBaseList *CPBaseList_new() {
	CPBaseList *list = (CPBaseList*)malloc(sizeof(CPBaseList));
    list->first = list->last = NULL;
    return list;
}

CPBaseListNode *CPBaseListNode_new(CPBase *data) {
	CPBaseListNode *node = (CPBaseListNode*)malloc(sizeof(CPBaseListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

void CPBaseList_add(CPBaseList *list, CPBase *data) {
	CPBaseListNode *node = CPBaseListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}


CDHBaseList *CDHBaseList_new() {
	CDHBaseList *list = (CDHBaseList*)malloc(sizeof(CDHBaseList));
    list->first = list->last = NULL;
    return list;
}

CDHBaseListNode *CDHBaseListNode_new(CDHBase *data) {
	CDHBaseListNode *node = (CDHBaseListNode*)malloc(sizeof(CDHBaseListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

void CDHBaseList_add(CDHBaseList *list, CDHBase *data) {
	CDHBaseListNode *node = CDHBaseListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}

CRBaseList *CRBaseList_new() {
	CRBaseList *list = (CRBaseList*)malloc(sizeof(CRBaseList));
    list->first = list->last = NULL;
    return list;
}

CRBaseListNode *CRBaseListNode_new(CRBase *data) {
	CRBaseListNode *node = (CRBaseListNode*)malloc(sizeof(CRBaseListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

void CRBaseList_add(CRBaseList *list, CRBase *data) {
	CRBaseListNode *node = CRBaseListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}

//methods for creating a GeneralRelation

//creates and mallocs new list of general relations
GenRelList *GenRelList_new() {
	GenRelList *list = (GenRelList*)malloc(sizeof(GenRelList));
    list->first = list->last = NULL;
    return list;
}

//creates and mallocs a new GenRelListNode
GenRelListNode *GenRelListNode_new(GenRel *data) {
	GenRelListNode *node = (GenRelListNode*)malloc(sizeof(GenRelListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

//adds a node to a GenRelList.
void GenRelList_add(GenRelList *list, GenRel *data) {
	GenRelListNode *node = GenRelListNode_new(data);
    node->next = list->first;
    if (list->first != NULL) {
	list->first->prev = node;
    }
    list->first = node;
    if (list->last == NULL) {
	list->last = node;
    }
}

//creates a new GenRel
GenRel *GenRel_new(char *course, char *studentID, char *grade, char *name, char *address, char *phone, char *day, char *hour, char *prereq, char *room) {
	GenRel *new = (GenRel*)malloc(sizeof(GenRel));
	
	new -> course = course;
	new -> studentID = studentID;
	new -> grade = grade;
	new -> name = name;
	new -> address = address;
	new -> phone = phone;
	new -> day = day;
	new -> hour = hour;
	new -> prereq = prereq;
	new -> room = room;

	return new;
}

//method to check if two stings are the same, first making sure that neither string is NULL.
bool sameString(char *thing1, char *thing2) {
	if (thing1 == NULL || thing2 == NULL) {
		return false;
	} else if (strcmp(thing1, thing2) == 0) {
		return true;
	} else {
		return false;
	}
}

//Relational Algebra operation select. 
GenRelList *select(GenRelList *list, char *type, char *param) {
	GenRelList *result = GenRelList_new();
	GenRelListNode *node = list -> first;
	while (node != NULL) {
		//specific checks for course and prereq because they contain the same string but mean different things
		if (sameString(type, "prereq") && sameString(node -> data -> prereq, param)) {
			GenRelList_add(result, node -> data);
		} else if (sameString(type, "course") && sameString(node -> data -> course, param)) {
			GenRelList_add(result, node -> data);
		} else if (sameString(param, node -> data -> studentID) || sameString(param, node -> data -> grade) || sameString(param, node -> data -> name) || sameString(param, node -> data -> address) || sameString(param, node -> data -> phone) || sameString(param, node -> data -> day) || sameString(param, node -> data -> hour) || sameString(param, node -> data -> room)) {
			GenRelList_add(result, node -> data);
		}
		node = node -> next;

 	}
 	return result;
}

//Relational Algebra operation project
GenRelList *project(GenRelList *list, char *param) {
	GenRelList *result = GenRelList_new();
	GenRelListNode *node = list -> first;
	while (node != NULL) {
		if (sameString(param, "course") && node -> data -> course != NULL && projectHelper(result, node -> data -> course)) {
			GenRelList_add(result, GenRel_new(node -> data -> course, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
		} else if (sameString(param, "studentID") && node -> data -> studentID != NULL && projectHelper(result, node -> data -> studentID)) {
			GenRelList_add(result, GenRel_new(NULL, node -> data -> studentID, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
		} else if (sameString(param, "grade") && node -> data -> grade != NULL && projectHelper(result, node -> data -> grade)) {
			GenRelList_add(result, GenRel_new(NULL, NULL, node -> data -> grade, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
		} else if (sameString(param, "name") && node -> data -> name != NULL && projectHelper(result, node -> data -> name)) {
			GenRelList_add(result, GenRel_new(NULL, NULL, NULL, node -> data -> name, NULL, NULL, NULL, NULL, NULL, NULL));
		} else if (sameString(param, "address") && node -> data -> address != NULL && projectHelper(result, node -> data -> address)) {
			GenRelList_add(result, GenRel_new(NULL, NULL, NULL, NULL, node -> data -> address, NULL, NULL, NULL, NULL, NULL));
		} else if (sameString(param, "phone") && node -> data -> phone != NULL && projectHelper(result, node -> data -> phone)) {
			GenRelList_add(result, GenRel_new(NULL, NULL, NULL, NULL, NULL, node -> data -> phone, NULL, NULL, NULL, NULL));
		} else if (sameString(param, "day") && node -> data -> day != NULL && projectHelper(result, node -> data -> day)) {
			GenRelList_add(result, GenRel_new(NULL, NULL, NULL, NULL, NULL, NULL, node -> data -> day, NULL, NULL, NULL));
		} else if (sameString(param, "hour") && node -> data -> hour!= NULL && projectHelper(result, node -> data -> hour)) {
			GenRelList_add(result, GenRel_new(NULL, NULL, NULL, NULL, NULL, NULL, NULL, node -> data -> hour, NULL, NULL));
		} else if (sameString(param, "prereq") && node -> data -> prereq != NULL && projectHelper(result, node -> data -> prereq)) {
			GenRelList_add(result, GenRel_new(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, node -> data -> prereq, NULL));
		} else if (sameString(param, "room") && node -> data -> room != NULL && projectHelper(result, node -> data -> room)) {
			GenRelList_add(result, GenRel_new(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, node -> data -> room));
		}
		node = node -> next;
	}

	return result;
}

//method that ensures items are only added to the projection results if they aren't already in 
bool projectHelper(GenRelList *list, char *param) {
	GenRelListNode *node = list -> first;
	if (node == NULL) {
		return true;
	}
	while (node != NULL) {
		if (sameString(node -> data -> course, param)  || sameString(node -> data -> studentID, param) || sameString(node -> data -> grade, param) || sameString(node -> data -> name, param) || sameString(node -> data -> address, param) || sameString(node -> data -> phone, param) || sameString(node -> data -> day, param) || sameString(node -> data -> hour, param) || sameString(node -> data -> prereq, param) || sameString(node -> data -> room, param)) {
			return false;
		}
		node = node -> next;
	}
	return true;
}

//Relational Algebra operation join

//in our database, you can only join on studentID or course
GenRelList *join(GenRelList *list1, GenRelList *list2, char *param){
	GenRelList *result = GenRelList_new();
	if (sameString(param, "course")) {
		GenRelListNode *list1Node = list1 -> first;
		while(list1Node != NULL) {
			GenRelListNode *list2Node = list2 -> first;
			while (list2Node != NULL) {
				if (sameString(list1Node -> data -> course, list2Node -> data -> course)) {
					GenRelList_add(result, combo(list1Node -> data, list2Node -> data));
				}
				list2Node = list2Node -> next;
			}
			list1Node = list1Node -> next;
		}
	} else if (sameString(param, "studentID")) {
		GenRelListNode *list1Node = list1 -> first;
		while(list1Node != NULL) {
			GenRelListNode *list2Node = list2 -> first;
			while (list2Node != NULL) {
				if (sameString(list1Node -> data -> studentID, list2Node -> data -> studentID)) {
					GenRelList_add(result, combo(list1Node -> data, list2Node -> data));
				}
				list2Node = list2Node -> next;
			}
			list1Node = list1Node -> next;
		}
	}
	return result;
}

//method for join that creates a GenRel with all attributes of two tuples with different schemas. 
GenRel *combo(GenRel *rel1, GenRel *rel2) {
	char *course; 
	char *studentID;
	char *grade;
	char *name;
	char *address;
	char *phone;
	char *day;
	char *hour;
	char *prereq;
	char *room;
	if (rel1 -> course != NULL) {
		course = rel1 -> course;
	} else if (rel2 -> course != NULL) {
		course = rel2 -> course;
	} else {
		course = NULL;
	}
	if (rel1 -> studentID != NULL) {
		studentID = rel1 -> studentID;
	} else if (rel2 -> course != NULL) {
		studentID = rel2 -> studentID;
	} else {
		grade = NULL;
	}
	if (rel1 -> grade != NULL) {
		grade = rel1 -> grade;
	} else if (rel2 -> grade!= NULL) {
		grade = rel2 -> grade;
	} else {
		grade = NULL;
	}
	if (rel1 -> name != NULL) {
		name = rel1 -> name;
	} else if (rel2 -> name != NULL) {
		name = rel2 -> name;
	} else {
		name = NULL;
	}
	if (rel1 -> address != NULL) {
		address = rel1 -> address;
	} else if (rel2 -> address != NULL) {
		address = rel2 -> address;
	} else {
		address = NULL;
	}
	if (rel1 -> phone != NULL) {
		phone = rel1 -> phone;
	} else if (rel2 -> phone != NULL) {
		phone = rel2 -> phone;
	} else {
		phone = NULL;
	}
	if (rel1 -> day != NULL) {
		day = rel1 -> day;
	} else if (rel2 -> day != NULL) {
		day = rel2 -> day;
	} else {
		day = NULL;
	}
	if (rel1 -> hour != NULL) {
		hour = rel1 -> hour;
	} else if (rel2 -> hour != NULL) {
		hour = rel2 -> hour;
	} else {
		hour = NULL;
	}
	if (rel1 -> prereq != NULL) {
		prereq = rel1 -> prereq;
	} else if (rel2 -> prereq != NULL) {
		prereq = rel2 -> prereq;
	} else {
		prereq = NULL;
	}
	if (rel1 -> room != NULL) {
		room = rel1 -> room;
	} else if (rel2 -> room != NULL) {
		room = rel2 -> room;
	} else {
		room = NULL;
	}

	return GenRel_new(course, studentID, grade, name, address, phone, day, hour, prereq, room);
}

//methods for converting a specific kind of table into a General Relation table

GenRelList *convert_CSG(CSGBase *database) {
	GenRelList *result = GenRelList_new();
	for (int i = 0; i < TABLE_LENGTH; i++) {
		if (database -> table[i] != NULL) {
			CSG *item = database -> table[i];
			while(item != NULL) {
				char *course = item -> course;
				char *studentID = item -> studentID;
				char *grade = item -> grade;
				GenRelList_add(result, GenRel_new(course, studentID, grade, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
				item = item -> next;
			}
		}
	}
	return result;

}

GenRelList *convert_SNAP(SNAPBase *database) {
    GenRelList *result = GenRelList_new();
    for (int i = 0; i < TABLE_LENGTH; i++) {
        if (database -> table[i] != NULL) {
            SNAP *item = database -> table[i];
            while(item != NULL) {
                char *name = item -> name;
                char *studentID = item -> studentID;
                char *address = item -> address;
                char *phone = item -> phone;
                GenRelList_add(result, GenRel_new(NULL, studentID, NULL, name, address, phone, NULL, NULL, NULL, NULL));
                item = item -> next;
            }
        }
    }
    return result;
}

GenRelList *convert_CDH(CDHBase *database) {
    GenRelList *result = GenRelList_new();
    for (int i = 0; i < TABLE_LENGTH; i++) {
        if (database -> table[i] != NULL) {
            CDH *item = database -> table[i];
            while(item != NULL) {
                char *course = item -> course;
                char *day = item -> day;
                char *hour = item -> hour;
                GenRelList_add(result, GenRel_new(course, NULL, NULL, NULL, NULL, NULL, day, hour, NULL, NULL));
                item = item -> next;
            }
        }
    }
    return result;
}

GenRelList *convert_CP(CPBase *database) {
    GenRelList *result = GenRelList_new();
    for (int i = 0; i < TABLE_LENGTH; i++) {
        if (database -> table[i] != NULL) {
            CP *item = database -> table[i];
            while(item != NULL) {
                char *course = item -> course;
                char *prereq = item -> prereq;
                GenRelList_add(result, GenRel_new(course, NULL, NULL, NULL, NULL, NULL, NULL, NULL, prereq, NULL));
                item = item -> next;
            }
        }
    }
    return result;
}

GenRelList *convert_CR(CRBase *database) {
    GenRelList *result = GenRelList_new();
    for (int i = 0; i < TABLE_LENGTH; i++) {
        if (database -> table[i] != NULL) {
            CR *item = database -> table[i];
            while(item != NULL) {
                char *course = item -> course;
                char *room = item -> room;
                GenRelList_add(result, GenRel_new(course, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, room));
                item = item -> next;
            }
        }
    }
    return result;
}

//method to print out a GenRel.
void GenRelList_print(GenRelList *list) {
	GenRelListNode *node = list -> first;
	while (node != NULL) {
		printf("NEW\n");
		if (node -> data -> course != NULL) {
			printf("Course: %s\n", node -> data -> course);
		}
		if (node -> data -> studentID != NULL) {
			printf("ID: %s\n", node -> data -> studentID);
		}
		if (node -> data -> grade != NULL) {
			printf("Grade: %s\n", node -> data -> grade);
		}
		if (node -> data -> name != NULL) {
			printf("Name: %s\n", node -> data -> name);
		}
		if (node -> data -> address != NULL) {
			printf("Address: %s\n", node -> data -> address);
		}
		if (node -> data -> phone != NULL) {
			printf("Phone: %s\n", node -> data -> phone);
		}
		if (node -> data -> day != NULL) {
			printf("Day: %s\n", node -> data -> day);
		}
		if (node -> data -> hour != NULL) {
			printf("Hour: %s\n", node -> data -> hour);
		}
		if (node -> data -> prereq != NULL) {
			printf("Prerequisite: %s\n", node -> data -> prereq);
		}
		if (node -> data -> room != NULL) {
			printf("Room: %s\n", node -> data -> room);
		}
		printf("DONE\n");
		node = node -> next;
	}

}
