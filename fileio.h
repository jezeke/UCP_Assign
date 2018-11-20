#ifndef FILEIO_H
#define FILEIO_H
#include "linked_list.h"

#define LINELENGTH (80) /*max line length for input*/

int loadLines(char *filename, LinkedList *result);

void allocateLineNode(LinkedListNode **node);

int makeLog(char *line);

#endif
