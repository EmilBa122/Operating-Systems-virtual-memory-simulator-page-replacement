#ifndef __STRUCTS__
#define __STRUCTS__

typedef struct page{
    
    char vpn[6];
    char offset[4];
    char RW[2];
    int dirty;
    int scbit;
    
    int time;
    int process;
    struct page* next;
    
}page;

typedef struct list{
    
    int size;
    struct page* start;
    struct page* end;

}list;

void initialize(list* l);
page* createpage(char*, char*, char*, int);
void insert(list*, page*);
void print(list*);
page* getfirst(list*);
void deletefirst(list*);
void destroy(list*);

#endif
