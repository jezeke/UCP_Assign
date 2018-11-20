/* fileio.c
#AUTHOR: Jhi Morris (19173632)
#MODIFIED: 16-10-18
#PURPOSE: Handles loading of files into memory
*/

#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOGLOC "graphics.log" /*file to write log to*/

/* loadLines
#RETURN: int (non-negative if successful)
#ARGUMENTS: char* (string, filename), LinkedList (linkedlist for export)
#PURPOSE: Loads a file, exports a linkedlist of nodes containing values of
  each line of the file (char*)*/
int loadLines(char *filename, LinkedList *result)
{
  char line[LINELENGTH];
  LinkedListNode *newNode;
  int returnCode = 0; /*default, all OK*/
  FILE *file = fopen(filename, "r");

  if(file == NULL)
  {
    returnCode = -1; /*failed to open*/
  }
  else
  {
    result->head = NULL;

    while(fgets(line, LINELENGTH, file) != NULL)
    {
      newNode = NULL;
      allocateLineNode(&newNode);

      strncpy((char*)newNode->value, line, LINELENGTH);
      if(result->tail != NULL)
      { /*insert to end*/
        result->tail->next = newNode;
        result->tail = newNode;
        result->count++;
      }
      else
      { /*make initial node*/
        result->tail = newNode;
        result->head = newNode;
        result->count++;
      }
    }

    if(result->count == 0)
    {
      returnCode = -2; /*failed to find any lines*/
    }

    fclose(file);
  }

  return returnCode;
}

/* allocateLineNode
#RETURN: none
#ARGUMENTS: LinkedListNode** (pointer to a pointer to a linked list node)
#PURPOSE: Allocates memory on the heap for the node to store line strings
  and initializes 'next' for comparison.*/
void allocateLineNode(LinkedListNode **node)
{
   *node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
   (*node)->value = malloc(sizeof(char)*LINELENGTH);
   (*node)->next = NULL;
}

/* makeLog
#RETURN: int (error code - returns -1 if failed to open file)
#ARGUMENTS: char* (string)
#PURPOSE: Opens an appending filestream the first time run. Writes
  the string to the file. Closes the file when run with an empty string.*/
int makeLog(char *line)
{ /*errCode is only useful when first logging - when opening the file*/
  int errCode = 0;
  static FILE *logfile = NULL;

  if(logfile == NULL)
  { /*check if file open from previous invocation*/
    logfile = fopen(LOGLOC, "a");

    if(logfile == NULL)
    {
      errCode = -1;
    }
  }

  if(errCode != -1)
  {
    if(*line == '\0')
    { /*empty string designates logging complete*/
      fclose(logfile);
      logfile = NULL;
    }
    else
    {
      fprintf(logfile, line);
    }
  }

  return errCode;
}
