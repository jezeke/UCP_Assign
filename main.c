/* main.c
#AUTHOR: Jhi Morris (19173632)
#MODIFIED: 17-10-18
#PURPOSE: Main class for the TurtleGraphics program.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "effects.h"
#include "fileio.h"
#include "turtle.h"
#include "linked_list.h"

void freeList(LinkedList *list);

/* main
#RETURN: int (return code)
#ARGUMENTS: int (argc), char* array (arguments)
#PURPOSE: Calls multiple other functions and checks that they completed
  without error, and prints appropriate error messages in the case of fault.*/
int main(int argc, char *argv[])
{ /*require first argument - filename. optional second argument - strict mode (invalid lines will cause the program to end)*/
  LinkedList *input;
  int errCode = 0;
  int strict;

  if(argc == 3)
  { /*figure out strictness*/
    if(!strcmp(argv[2], "-s"))
    {
      strict = 1; /*strict true*/
    }
    else
    {
      strict = -2; /*denotes invalid strict flag*/
    }
  }
  else
  {
    strict = 0; /*strict false*/
  }

  if(strict == -2 || (argc != 2 && argc != 3))
  { /*check if unexpected argument count OR already invalid strict flag*/
    printf("Expecting format: \"UCPAssignment filename [-s]\".\n"
      "The -s flag denotes strict mode enabled. By default strict mode is off.\n"
      "With strict mode on invalid lines will not be ignored or skipped.\n");
    errCode = 1;
  }
  else
  {
    input = (LinkedList*)malloc(sizeof(LinkedList));
    input->count = 0;
    input->head = NULL;
    input->tail = NULL;

    errCode = loadLines(argv[1], input);
    if(errCode == -1)
    {
      printf("Unable to read file \"%s\".\n", argv[1]);
    }
    else
    {
      printf("Loaded %d lines from file \"%s\".\n", input->count, argv[1]);

      errCode = makeLog((char*)"---\n");
      if(errCode == -1) /*start log - check if it failed to open file*/
      {
        printf("Unable to write to log file.\n");
      }
      else
      {
        errCode = parseCommands(input, strict);

        if(errCode >= 0)
        {
          printf("Drawing complete with %d errors.\n", errCode);
        }

        if(errCode == -1)
        {
          printf("Unexpected line in input file. Ending because of -s flag.\n");
        }

        if(errCode == -2)
        {
          printf("Valid commands brought coordinates to invalid position.\n");
        }

        makeLog((char*)""); /*close log file*/
      }

      freeList(input);
    }
    free(input); /*wash our hands of the linked list*/
    input = NULL;
  }

  return errCode;
}

/* freeList
#RETURN: none
#ARGUMENTS: *LinkedList (A pointer to a linked list)
#PURPOSE: Frees the linked list and linked nodes+values. */
void freeList(LinkedList *list)
{
  LinkedListNode *node = list->head;
  LinkedListNode *nextNode;

  if(node->next != NULL)
  {
    nextNode = node->next;
  }

  while(node->next != NULL)
  {
    nextNode = node->next;
    free(node->value);
    free(node);
    node = nextNode;
  }

  free(node->value); /*catch the last node*/
  free(node);
}
