#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "functionality.h"

int main (int argc, char* argv[])
{	
	//programm parameters
	char alg[4];
	int q, max, pt_size;
	max = 2000000;
		
	//reading the command line
	read_command(argc, argv, alg, &q, &pt_size, &max);
	//printf("%s, %d, %d, %d \n", alg, q, pt_size, max);
	
	//stats
	int reads_td = 0;
	int writes_td = 0;
	int page_faults = 0;
	int main_frames = pt_size;
	int entries = 0;	
	int page_hits = 0;
	
	printf("Program parameters:\nq: %d\nmax: %d\npage table size: %d\nalgorithm: %s\n", q, max, pt_size, alg);
	//list that will hold all the pages given by user
	list l;
	initialize(&l);
	
	printf("Reading the files...\n");
	//read the files
	read_from_files(&l, q, max);
	
	//print(&l);
	
	//pagetable
	page* page_table = malloc(pt_size*sizeof(page));
	//initialize -1 proc to declare empty
	for(int i=0; i<pt_size; i++)
	{
	    page_table[i].time = -1;
	}
	
	//start the simulation
	int timer = 1;	
	
    	if(strcmp(alg,"LRU")==0)
    	{
        	printf("Applying LRU as replacement algorithm.\n");
        	printf("Performing the simulation....\n");
    		while(1)
    		{
	        	page* current = getfirst(&l);
	        	if(current == NULL)
	        	{
	            		break;
	        	}
	        	int exists = checkexistence(page_table, current, pt_size);
	        	if(exists == -1)
	        	{     
	        		//page fault, increase the counters of read of write access
	        		page_faults++;
	        		if(strcmp(current->RW, "R") == 0)
	        		{
	        			reads_td++;
	        		}
	        		else
	        		{
	        			writes_td++;
	        		}
	        		//search for space
	        		int spot = checkifempty(page_table, pt_size);
	        		if(spot == -1)
	        		{
	        			//no spot available, search for victim page based on LRU
	        			int spot_v = searchLRUvictim(page_table, pt_size);
	        			//printf("Spoted a victim in position: %d\n", spot_v);
	        			//if victim page is modified, write it to disk
	        			if(page_table[spot_v].dirty == 1)
	        			{
	        				writes_td++;
	        			}
	        			//then remove it
	        			strcpy(page_table[spot_v].vpn, current->vpn);
	        			strcpy(page_table[spot_v].offset, current->offset);
					page_table[spot_v].dirty = 0;
					page_table[spot_v].time = timer;
					page_table[spot_v].process = current->process;
	        		}
	        		else
	        		{
	        			//has an empty spot
	        			strcpy(page_table[spot].vpn, current->vpn);
	        			strcpy(page_table[spot].offset, current->offset);
					page_table[spot].dirty = 0;
					page_table[spot].time = timer;        		
	        			page_table[spot].process = current->process;
	        		}
	        	}
	        	else
	        	{
	        		page_hits++;
	        		//page hit, refresh time and continue
	        		page_table[exists].time = timer;
	        		//if the page is modified, switch on the dirty bit
	        		if(strcmp(current->RW, "W") == 0)
	        		{
	        			page_table[exists].dirty = 1;
	        		}
	        	}
	        	entries++;
	        	deletefirst(&l);
	        	timer++;
	    	}
	    	//remove the dirty bits, write pages back to the disk
	    	for(int i=0; i<pt_size; i++)
	    	{
	    		if(page_table[i].dirty == 1)
	    		{
	    			writes_td++;
	    		}
		}	    
    	}
    	else if(strcmp(alg,"SC")==0)
    	{
        	printf("Applying Second Chance as replacement algorithm\n");
        	int candidate_victim_spot = 0;
        	int iterator;
        	while(1)
        	{
	        	page* current = getfirst(&l);
	        	if(current == NULL)
	        	{
	            		break;
	        	}	        
	        	int exists = checkexistence(page_table, current, pt_size);
	        	if(exists == -1)
	        	{
	        		//page fault, increase the counters of read of write access
	        		page_faults++;
	        		if(strcmp(current->RW, "R") == 0)
	        		{
	        			reads_td++;
	        		}
	        		else
	        		{
	        			writes_td++;
	        		}
	        		//search for space
	        		int spot = checkifempty(page_table, pt_size);
	        		if(spot == -1)
	        		{
	        			//no spot available, search for victim page based on Second Chance
	        		
	        			iterator = candidate_victim_spot;
	        			while(1)
	        			{
	        				if(page_table[iterator].scbit == 0)
	        				{
	        					//we can remove the page
	        					strcpy(page_table[iterator].vpn, current->vpn);
							strcpy(page_table[iterator].offset, current->offset);
							page_table[iterator].dirty = 0;
							page_table[iterator].time = timer;
							page_table[iterator].process = current->process;
							page_table[iterator].scbit = 1;
							
							//the next frame is possible victim frame
							iterator = iterator + 1;
							//if iterator exceeds the size of the page table,
							//then go to element 0
							if(iterator >= pt_size)
							{
								candidate_victim_spot = 0;
							}
							else
							{
								candidate_victim_spot = iterator;
							}
							//end the loop and read the next reference
							break;
	        				}
	        				else
	        				{
	        					//the scbit of the frame is set to 1 and thus we cannot kill it
	        					//set the bit to 0 and 
	        					page_table[iterator].scbit = 0;
	        					//go to the next frame
	        					if(iterator >= pt_size)
	        					{
								iterator = 0;
							}
							else
							{
								iterator = iterator + 1;
							}
	        				}	
	        			}
	        		}
	        		else
	        		{		
	        			//has an empty spot
	        			strcpy(page_table[spot].vpn, current->vpn);
	        			strcpy(page_table[spot].offset, current->offset);
					page_table[spot].dirty = 0;
					page_table[spot].time = timer;        		
	        			page_table[spot].process = current->process;
	        			page_table[spot].scbit = 1;
	        		}
	        	}
	        	else
	        	{
	        		page_hits++; 	
	        		//if the page is modified, switch on the dirty bit
	        		if(strcmp(current->RW, "W") == 0)
	        		{
	        			page_table[exists].dirty = 1;
	        			page_table[exists].scbit = 1;
	        		}
	        	}
	        	deletefirst(&l);
	        	entries++;
	   	}
	    	//remove the dirty bits, write pages back to the disk
	    	for(int i=0; i<pt_size; i++)
	    	{
	    		if(page_table[i].dirty == 1)
	    		{
	    			writes_td++;
	    		}
		}
    	}		
	//print stats
	printf("\n\n-----Statistics-----\n");
	printf("Page faults: %d\n", page_faults);
	printf("Page hits: %d\n", page_hits);
	printf("Number of entriess: %d\n", entries);
	printf("Page fault rate: %lf\n", (float)page_faults/(float)entries);
	printf("Reads from disk: %d\n", reads_td);
	printf("Writes to disk: %d\n", writes_td);
	printf("RAM frames: %d\n", main_frames);
	printf("--------------------\n");
	
	free(page_table);

	destroy(&l);

	return 0;
}



