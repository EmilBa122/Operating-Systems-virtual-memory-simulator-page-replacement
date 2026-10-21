#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "functionality.h"


// C substring function definition
void substring(char s[], char sub[], int p, int l) 
{
   int c = 0;
   while (c < l) 
   {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}

int searchLRUvictim(page *page_table, int size)
{	
	int min_time = 100000000;
	int min_spot = -1;
	for(int i=0; i<size; i++)
	{
		//find the first empty spot
	    	if(page_table[i].time < min_time)
	    	{
	    		min_time = page_table[i].time;
	    		min_spot = i;
	    	}
	}
	return min_spot;
}

int checkifempty(page* page_table,int size)
{
	for(int i=0; i<size; i++)
	{
		//find the first empty spot
	    	if(page_table[i].time == -1)
	    	{
	    		return i;
	    	}
	}
	//no empty spot
	return -1;
}

int checkexistence(page* page_table, page* current, int size)
{	
	for(int i=0; i<size; i++)
	{
		//if the page exists (check only the vpn)
	    	if(strcmp(page_table[i].vpn, current->vpn) == 0)
	    	{
			//return its spot
			return i;
	    	}
	}
	//else return -1
	return -1;
}


void read_command (int argc, char** argv, char* alg, int* q, int* pt_size, int* max)
{
	int i=1;	
	if (argc==9 || argc==7)
	{
		for (i=1; i<argc; i=i+2)
		{
		    if (strcmp(argv[i], "-a") == 0)
		    {
			    strcpy(alg,argv[i+1]);
		    }
		    else if (strcmp(argv[i], "-q") == 0)
		    {
			    *q=atoi(argv[i+1]);
		    }
		    else if (strcmp(argv[i], "-s") == 0)
		    {
			    *pt_size=atoi(argv[i+1]);
		    }
		    else if (strcmp(argv[i], "-m") == 0)
		    {
			    *max=atoi(argv[i+1]);
		    }
		    else 
		    {
			    printf("Wrong arguments! Flags: -a, -q, -s, -m\n");
			    exit(0);
		    }
	    	}
	}
	else
	{
	    	printf("Wrong arguments! Flags: -a, -q, -s, -m\n");
		exit(0);
	}
}

void read_from_files(list* l, int q, int max)
{
	//files
	FILE *bzip;
	bzip = fopen("./tracefiles/bzip.trace", "r");
	FILE *gcc;
	gcc = fopen("./tracefiles/gcc.trace", "r");
	
	char * line = NULL;
    	size_t len = 0;
    	ssize_t read;

    	if (bzip == NULL)
        	exit(EXIT_FAILURE);
    	if (gcc == NULL)
        	exit(EXIT_FAILURE);
    
    	//no restrictions, read all the entries
    	if(max == 2000000)
    	{
        	//printf("No max value given!!\n");
        
        	int flag1 = 1;
        	int flag2 = 1;
        	while(1)
        	{
            		for(int i=0; i<q; i++)
            		{            
                    		read = getline(&line, &len, bzip);
                    		if(read == -1)
                    		{
                       		 flag1 = 0;
                    		}
                    		else
                    		{
                       		//printf("Readng line from bzip!\n");
                        
                       		strtok(line, "\n");
                        
                       		//printf("%s\n", line);
                        
                       		char vpn[6];
                       		char offset[4];
                       		char RW[2];
                         
                       		substring(line, vpn, 1, 5);
                        		substring(line, offset, 6, 3);
                        		substring(line, RW, 10, 1);
                      	             
                        		page* p = createpage(vpn, offset, RW, 1);
                        
                        		insert(l , p);
                		} 
            		}
           		for(int i=0; i<q; i++)
           		{
                    		read = getline(&line, &len, gcc);
                    		if(read == -1)
                    		{
                        		flag2 = 0;
                    		}
                    		else
                    		{
                        		//printf("Readng line from gcc!\n");
                        
                        		strtok(line, "\n");
                        
                        		char vpn[6];
                        		char offset[4];
                        		char RW[2];
                        
                        		substring(line, vpn, 1, 5);
                        		substring(line, offset, 6, 3);
                        		substring(line, RW, 10, 1);
                      	             
                        		page* p = createpage(vpn, offset, RW, 2);
                        
                        		insert(l , p);
                		} 
            		}            
            		if(flag1==0 && flag2==0)
            		{
                		break;
           		}
        	}
    	}
    	else
    	{
        	//printf("Max is: %d\n", max);
        
        	int counter = 0;
        	while(1)
        	{
            		for(int i=0; i<q; i++)
            		{
                		if(counter < max)
                		{
                    			//printf("Readng line from bzip!\n");
                    			read = getline(&line, &len, bzip);
                    			strtok(line, "\n");
                    
                  			char vpn[6];
		            		char offset[4];
		            		char RW[2];
		         
		           	 	substring(line, vpn, 1, 5);
		            		substring(line, offset, 6, 3);
		            		substring(line, RW, 10, 1);
		            
                    			page* p = createpage(vpn, offset, RW, 1);
                    
                    			insert(l , p);
                    
                    			counter++;
                		}
            		}
            		for(int i=0; i<q; i++)
            		{
                		if(counter < max)
                		{
                    			//printf("Readng line from gcc!\n");
                    			read = getline(&line, &len, gcc);
                    			strtok(line, "\n");
                    
                  			char vpn[6];
		            		char offset[4];
		            		char RW[2];
		      
		            		substring(line, vpn, 1, 5);
		            		substring(line, offset, 6, 3);
		            		substring(line, RW, 10, 1);
                    
                    			page* p = createpage(vpn, offset, RW, 2);
                    
                    			insert(l , p);
                    
                    			counter++;
                		}
            		}
            		if(counter == max)
            		{
                		break;
            		}             
        	}
    	}
    	fclose(gcc);
    	fclose(bzip);
    	if (line)
        	free(line);

}

