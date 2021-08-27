#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h> 
#include<math.h>

struct t_struct
{
	int id;
	int burst;
	int arrival;
	struct t_struct *next;
};

struct t_struct *head = NULL;
struct t_struct *curr = NULL;
int count = 0;

struct t_struct* create(int id, int burst, int arrival)
{
	struct t_struct *test = (struct t_struct*)malloc(sizeof(struct t_struct));
	//Testing if the structure is unable to create
	if(test == NULL)
	{
		printf("\n failed to create node \n");
		return NULL;
	}
	//Assigning next object it's variables, no next pointer as of yet
	test->id = id;
	test->burst = burst;
	test->arrival = arrival;
	test->next = NULL;
	
	head = curr = test;
	return test;
}

void printList(void){
	struct t_struct *test = head;
	printf("\n ======= printing ========\n");
	while(test != NULL){
		printf("\n [id: %d, b: %d, a: %d] \n", test->id, test->burst, test->arrival);
		test = test->next;
	}
	printf("\n ======= end of print =======\n");
}

struct t_struct* add(int id, int burst, int arrival, bool end)
{
	if(head == NULL){
		return (create(id, burst, arrival));
	}
	
	struct t_struct *test = (struct t_struct*)malloc(sizeof(struct t_struct));
	if(test == NULL){
		printf("\n failed to create node\n");
		return NULL;
	}
	test->id = id;
	test->burst = burst;
	test->arrival = arrival;
	test->next = NULL;
	
	if(end)
	{
		curr->next = test;
		curr = test;
	}
	else
	{
		test->next = head;
		head = test;
	}
	return test;
}

int iterate(int id, int burst){
	int elapsed;
	elapsed = burst;
	while(elapsed > 0){
		count++;
		elapsed--;
	}
	return 1;
}

struct t_struct* search_in_list(int id, int burst, struct t_struct **prev)
{
    struct t_struct *test = head;
    struct t_struct *tmp = NULL;
    bool found = false;

    while(test != NULL)
    {
        if(test->id == id && test->burst == burst)
        {
            found = true;
            break;
        }
        else
        {
            tmp = test;
            test = test->next;
        }
    }

    if(true == found)
    {
        if(prev)
            *prev = tmp;
        return test;
    }
    else
    {
        return NULL;
    }
}

int delete(int id, int burst, FILE* fp, bool last)
{
	struct t_struct *prev = NULL;
	struct t_struct *del = NULL;
	int turnaround = 0;
	int waiting = 0;
	
	del = search_in_list(id, burst, &prev);
	if(del == NULL){
		return -1;
	}
	else{
		if(prev != NULL){
			prev->next = del->next;
		}
		if(del == curr){
			curr = prev;
		}
		else if(del == head){
			head = del->next;
		}
	}
	iterate(del->id, del->burst);
	turnaround = count - del->arrival;
	waiting = turnaround - del->burst;
	printf("ID: %d, Waiting time: %d, Turnaround time: %d\n--------\n", del->id, waiting, turnaround);
	if(last)
		fprintf(fp, "%d %d %d", del->id, waiting, turnaround);
	else
		fprintf(fp, "%d %d %d\n", del->id, waiting, turnaround);
	
	free(del);
	del = NULL;
	return 0;
}

	
int main(int argc, char *argv[])
{
	FILE * fp;
	char * line = NULL;
	char *filename;
	size_t len = 0;
	ssize_t read;
	int shortest = 100;
	int shortID;
	struct t_struct *ptr = NULL;
	bool repeat;
	
	if(argc < 2){
		printf("./FCFS file\n");
		return (1);
	}
	else{
		filename = argv[1];
	}
	
	fp = fopen(filename, "r");
	if(fp == NULL)
		exit(EXIT_FAILURE);
	
	
	while((read = getline(&line, &len, fp)) != -1){
		// tokenizing to string with spacing " " 
		char *tok = strtok(line, "\t");
		// tokenizing to int from string
		int f = atoi(tok);
		int s; 
		int t;
		while(tok!=NULL){
			tok=strtok(NULL,"\t");
			if(tok==NULL)
				break;
			s = atoi(tok);
			tok=strtok(NULL, "\t\n");
			if(tok==NULL)
				break;
			t = atoi(tok);
		}
		add(f, s, t, true);
	}
	fclose(fp);
	//Creating my new file to output my data! 
	filename = "outputSJF.txt";
	fp = fopen(filename, "w");
	
	/* This algorithm will be slightly harder, as I will have to iterate over the list everytime and
	find the shortest job that is within our elapsed time (current) and then possibly delete the node
	when it's elapsed variable is = 0, as this will quicken the process significantly */
	ptr = head;
	shortest = ptr->burst;
	shortID = ptr->id;
	printf("Beginning the Shortest Job First algorithm...\n--------\n");
	repeat = true;
	while(repeat){
		if(ptr->arrival > count){
			delete(shortID, shortest, fp, false);
			ptr = head;
			shortest = ptr->burst;
			shortID = ptr->id;
			continue;
		}
		if(ptr->burst < shortest){
			shortest = ptr->burst;
			shortID = ptr->id;
			
		}
		if(ptr->next != NULL){
			ptr = ptr->next;
		}
		
		//In this case, it is either the end of the line or head is the last remaining component in the list
		if(ptr->next == NULL){
			if(ptr->burst < shortest){
				shortest = ptr->burst;
				shortID = ptr->id;
			}
			delete(shortID, shortest, fp, false);
		
			// if the head is != (end of list) this checks whether the head is the last object or not
			if(head->next != NULL){
				ptr = head;
				shortest = ptr->burst;
				shortID = ptr->id;
				continue;
			}
			else{
				repeat = false;
				shortest = head->burst;
				shortID = head->id;
				delete(shortID, shortest, fp, true);
			}
		}
	}
	fclose(fp);
	printf("Rhys Reid Mallia, s3656436, COSC1114 :)\n");
	return 0;
}
	