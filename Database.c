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

CSGLinkedList *CSGLinkedList_new()  {
    CSGLinkedList *list = (CSGLinkedList*)malloc(sizeof(CSGLinkedList));
    list->first = list->last = NULL;
    return list;
}

CSGLinkedListNode *CSGLinkedListNode_new(CSG *data) {
    CSGLinkedListNode *node = (CSGLinkedListNode*)malloc(sizeof(CSGLinkedListNode));
    if (node == NULL) {
	abort();
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

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

int CSGHash(char* course, char *studentID) {
	int strAsNum = 0;
	int max = strlen(course);
	for (int i = 0; i < max; i++) {
		strAsNum += course[i];
	}
	return (strAsNum + atoi(studentID)) % TABLE_LENGTH;
}

int isTakenCSG(CSGTable table, int index) {
	if (table[index] == NULL) {
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
	
	CSG *toAdd = CSG_new(course, studentID, grade);
	if(CSG_add(database, CSGHash(course, studentID), toAdd, 1)) {
		CSG_add(database, CSGHash_id(studentID), toAdd, 2);
		CSG_add(database, CSGHash_course(course), toAdd, 3);
	}
	
}

bool CSG_add(CSGBase *database, int index, CSG *toAdd, int kind) {
	if (kind == 1) {
		if(!isTakenCSG(database -> table, index)) {
			database -> table[index] = toAdd;
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
			printf("%s\n", "Sorry, there was no info to find.");
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
			printf("%s\n", "Sorry, there was no info to find.");
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
			printf("%s\n", "Sorry, there was no info to find.");
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
			printf("%s\n", "Sorry, there was no info to find.");
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
			printf("%s\n", "Sorry, there was no info to find.");
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
	for (CSGLinkedListNode *node = list -> first; node != NULL; node = node -> next) {
		printf("Student %s got a grade of %s in course %s.\n", node -> data -> studentID, node -> data -> grade, node -> data -> course);
	}
}

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
		printf("%s\n", "right start in delete");
		int index = CSGHash(course, studentID);
		if (database -> table[index] == NULL) {
			printf("%s\n", "Sorry, there was no info to delete.");
		} else {
			printf("%s\n", "found info to delete");
			CSG *test = database -> table[index];
			printf("%s\n", "created test");
			if (strcmp(test -> course, course) == 0 && strcmp(test -> studentID, studentID) == 0) {
				printf("%s\n", "inside if");
				if (test -> next != NULL) {
					printf("%s\n", "next is not null");
					database -> table[index] = test -> next;
					printf("%s\n", "sucessfully deleted");
				} else {
					printf("%s\n", "next is null");
					database -> table[index] = NULL;
					printf("%s\n", "deleted");
				}
				deleteFromIndices_CSG(database, test -> course, test -> studentID, test -> grade);
				printf("%s\n", "deleted from indices");
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

		
