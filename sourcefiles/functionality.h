#ifndef __FUNCT__
#define __FUNCT__

#include "structs.h"

void read_command (int , char** , char* , int* , int* , int*);
void read_from_files(list*, int, int);
int checkexistence(page*, page*, int);
int checkifempty(page*, int);
int searchLRUvictim(page *, int);
void substring(char s[], char sub[], int , int );

#endif
