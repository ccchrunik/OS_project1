#ifndef PLIST_H
#define PLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"


/*
 This process list is double circular linked list.
 */
typedef struct pNode
{
    task_t *task;
    struct pNode *prev;
    struct pNode *next;

} pNode_t;

/*
  This struct is the manager of the process list;
 */
typedef struct plist
{
    pNode_t *dummy;
    int length;
} plist_t;

void list_init(plist_t* vl);
void list_insert_head(plist_t* vl, task_t *vnode);
void list_insert_tail(plist_t* vl, task_t *vnode);
void list_delete_head(plist_t* vl);
void list_delete_tail(plist_t* vl);
// void list_traverse(plist_t* vl);


#endif