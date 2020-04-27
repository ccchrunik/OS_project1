#include "plist.h"

void list_init(plist_t *p)
{
    p->dummy = malloc(sizeof(pNode_t));
    p->dummy->next = p->dummy;
    p->dummy->prev = p->dummy;
    p->length = 0;
}

void list_insert_head(plist_t* p, task_t *t)
{
    if(p->length == 0)
    {
        pNode_t *node = malloc(sizeof(pNode_t));
        node->task = t;

        node->next = p->dummy;
        node->prev = p->dummy;
        p->dummy->next = node;
        p->dummy->prev = node;
        
        p->length += 1;
    }
    else
    {
        pNode_t *node = malloc(sizeof(pNode_t));
        node->task = t;

        // set node pointer
        node->next = p->dummy->next;
        node->prev = p->dummy;
        // set prev and next pointer
        p->dummy->next->prev = node;
        p->dummy->next = node;
        
        p->length += 1;

    }
}

void list_insert_tail(plist_t* p, task_t *t)
{
    if(p->length == 0)
    {
        pNode_t *node = malloc(sizeof(pNode_t));
        node->task = t;

        node->next = p->dummy;
        node->prev = p->dummy;
        p->dummy->next = node;
        p->dummy->prev = node;
        
        p->length += 1;
    }
    else
    {
        pNode_t *node = malloc(sizeof(pNode_t));
        node->task = t;

        // set node pointer
        node->prev = p->dummy->prev;
        node->next = p->dummy;
        // set prev and next pointer
        p->dummy->prev->next = node;
        p->dummy->prev = node;
        
        p->length += 1;

    }
}

void list_delete_head(plist_t* p)
{
    if(p->length == 1)
    {
        pNode_t *node = p->dummy->next;

        p->dummy->next = p->dummy;
        p->dummy->prev = p->dummy;
        
        free(node);
        p->length = 0;
    }
    else
    {
        pNode_t *node = p->dummy->next;

        // set prev and next pointer
        p->dummy->next = node->next;
        node->next->prev = node->prev;
        
        free(node);
        p->length -= 1;

    }
}

void list_delete_tail(plist_t* p)
{
    if(p->length == 1)
    {
        pNode_t *node = p->dummy->prev;

        p->dummy->next = p->dummy;
        p->dummy->prev = p->dummy;
        
        free(node);
        p->length = 0;
    }
    else
    {
        pNode_t *node = p->dummy->prev;

        // set prev and next pointer
        p->dummy->prev = node->prev;
        node->prev->next = node->next;
        
        free(node);
        p->length -= 1;

    }
}


void list_print(plist_t* p)
{
    pNode_t* node = p->dummy;

    if(p->length == 0)
    {
        printf("empty list!\n");
        return ;
    }
    else
    {
        // printf("print new node!\n");  
        // printf("length = %d\n", p->length); 
        printf("print list!\n"); 
        for(int i = 0; i < p->length; ++i)
        {
            node = node->next;
            printf("node %s = %d\n", node->task->name, node->task->rem_time);
        }
        printf("\n");
        
    }
}


