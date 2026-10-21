# Operating Systems virtual memory simulator page replacement

## Assignment Description
Virtual Memory Simulator with the Page Replacement Algorithms LRU and
Second Chance based on Real Memory Traces (bzip and gcc inside of the folder
tracefiles) of Two Processes in C

**INSTRUCTIONS FOR RUNNING :**

1. **Open a Linux terminal**
2. **Type make**

**(Now after the command ./mementries the program expects to receive as
arguments the replacement algorithm after the character "-a" , the number of
references q after the character "-q" , the number of frames of main memory
after the character "-s" and it can receive the maximum number of references
after the character "-m" if it does not receive it we assume that we will read
all the references from the 2 files)**

**So e.g. the user can type :**

3. **./mementries -a LRU -q 20 -s 150 -m 500**

**DESIGN CHOICES**

**INITIALLY THE PROGRAM WILL ALTERNATELY READ q NUMBER OF REFERENCES FROM THE
TWO FILES AND WILL PLACE THEM IN A LINKED LIST , WHEN THE NUMBER OF REFERENCES
REACHES THE NUMBER max THE PROGRAM STOPS READING REFERENCES OTHERWISE IF THE
USER HAS NOT SPECIFIED max THE PROGRAM WILL READ ALL THE REFERENCES FROM THE TWO
FILES.**<br><br>
**TO REPRESENT THE PAGE IN THE PROGRAM WE USE THE struct page , EACH PAGE OF THE
PROGRAM APART FROM ITS vpn (p#) , ITS offset AND THE R OR W HAS A VARIABLE NAMED
dirty FOR WHETHER IT HAS BEEN MODIFIED , IT HAS THE VARIABLE scbit WHICH IS USED
FOR THE second chance ALGORITHM AND THE VARIABLE timer WHICH IS USED TO TELL US
WHEN THE PAGE "ARRIVED" IN OUR SIMULATION.**<br><br>
**THE page table OF THE ASSIGNMENT IS AN ARRAY OF SIZE pt_size AND EACH CELL OF
IT CAN CONTAIN ONE PAGE.**

IMPLEMENTATION OF THE ASSIGNMENT

In the case where the replacement algorithm is LRU the program will each time
take a reference from the list and will check whether a page exists in the
page_table according to the reference (through the function chechexistence). If
it does not exist it will check whether there is space in the page_table for it
to be assigned (through the function checkifempty). If there is not we must find
a victim page according to the function searchLRUvictim, we check whether the
victim page has been modified if yes we write it to the disk then we replace
these two pages. If there was space in the page_table we write the page.
Otherwise if it already existed we have a page_hit.<br><br>
On the contrary if the replacement algorithm was second chance we define two
variables candidate_victim_spot and iterator. For each reference from the list
the algorithm will check whether a page exists in the page_table according to
the reference.<br><br>
If a page does not exist in the page_table and there is no space then we check
for each page of the page_table its second chance bit. If the second chance bit
of a page is 0 then we can replace it and we increase the candidate_victim_spot
by one or we set it back to 0 if we have exceeded the limits of the size of the
page_table. If the second chance bit is 1 for the specific page then after we
set it to 0 we go to the next frame or if we have exceeded the size of the
page_table we go to the initial frame. Otherwise if there is space we write it
to the page_table. Finally if it existed in the page_table then we have a
page_hit.

**<ins>HAS NOT BEEN IMPLEMENTED</ins> the hashed page table instead a simple
Page Table has been implemented.**
