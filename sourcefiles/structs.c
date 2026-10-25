#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "functionality.h"

void initialize(list* l)
{
    l->size = 0;
    l->start = NULL;   
}

page* createpage(char* vpn, char* offset, char* RW, int proc)
{    
    page* p = malloc(sizeof(page));
    
    strcpy(p->vpn, vpn);
    strcpy(p->offset, offset);
    strcpy(p->RW, RW);
    
    //printf("%s%s %s\n", p->vpn, p->offset, p->RW);
    
    p->dirty = 0;
    p->time = 0;
    p->process = proc;
    p->next = NULL;
    
    return p;    
}

void insert(list* l, page* p)
{    
    if(l->start == NULL)
    {
        l->start = p;
        l->end = p;
        l->size++;
        return ;
    }
    l->end->next = p;
    l->end = p;
    l->size++;
    return ;
}

void print(list* l)
{    
    page* temp = l->start;
    while(temp != NULL)
    {
        printf("%s%s %s\n", temp->vpn, temp->offset, temp->RW);
        temp = temp->next;
    }
    printf("There are %d elements in the list!\n", l->size);
}

page* getfirst(list* l)
{
    if(l->start == NULL)
    {
       return NULL; 
    }
    else
    {
        return l->start;
    }
}

void deletefirst(list* l)
{    
    if(l->start == NULL)
    {
        return ;
    }
    page* temp = l->start;
    l->start = l->start->next;
    free(temp);
    l->size--;
}

void destroy(list* l)
{    
    if(l->start == NULL)
    {
        return ;
    }
    page* temp = l->start;
    while(temp != NULL)
    {
        l->start = l->start->next;
        free(temp);
        temp = l->start;
        l->size--;
    }        
}
