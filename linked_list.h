/* linked_list.h
#AUTHOR: Jhi Morris (19173632)
#MODIFIED: 14-10-18
#PURPOSE: Establishes the structs for a linked list and linked list node.
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct LinkedListNode
{
  struct LinkedListNode *next;
  void *value;
} LinkedListNode;

typedef struct LinkedList
{
  LinkedListNode *head;
  LinkedListNode *tail;
  int count;
} LinkedList;

#endif
