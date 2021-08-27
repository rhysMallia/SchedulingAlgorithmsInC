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
	int elapsed;
	struct t_struct *next;
};

struct t_struct *head = NULL;
struct t_struct *curr = NULL;
int count = 0;
int quantum = 19;
int context = 1;
int qc = 20;

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
	test->elapsed = burst;
	test->arrival = arrival;
	test->next = NULL;
	
	head = curr = test;
	return test;
}

void printList(void){
	struct t_struct *test = head;
	printf("\n ======= printing ========\n");
	while(test != NULL){
		printf("\n [id: %d, b: %d, a: %d, e: %d] \n", test->id, test->burst, test->arrival, test->elapsed);
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
	test->elapsed = burst;
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
	int turnaround;
	int waiting;
	int current = 0;
	bool delete = false;
	int temp = 0;
	float fWait;
	float fTurn;
	del = search_in_list(id, burst, &prev);
	/* This loop works to iterate down the elapsed time towards 1, when 1 is reached, the context switch takes one final value 
	in order to switch to the next, which means that the elapse is only down 19 values or 1.9 quantum in the real world,
	the loop then finds if at any point the value is reached, and if it is the object is set to be deleted from the chain as it is 
	'complete' in terms of processing */
	temp = del->elapsed;
	if(temp != 0){
		while(current < quantum){
			if(temp == 1){
				count++;
				temp--;
				delete = true;
				break;
			}
			count++;
			current++;
			temp--;
		}
	}
	
	del->elapsed = temp;
	
	//I was tired when I wrote this but I put this here incase the loop didn't work for some odd reason
	if(del->elapsed == 0){
		delete = true;
	}
	
	//if the structures elapsed time = 0, then it should be removed as it is no longer able to be worked on! 
	if(delete){
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
		turnaround = count - del->arrival;
		waiting = turnaround - del->burst;
		//I change the values to floats and divide the final answer by 10 as I have multiplied it in order to keep my code simple
		fWait = (float)waiting;
		fTurn = (float)turnaround;
		printf("ID: %d, Waiting time: %0.2f, Turnaround time: %0.2f\n--------\n", del->id, fWait / 10, fTurn / 10);
		if(last){
			fprintf(fp, "%d %0.2f %0.2f", del->id, fWait / 10, fTurn /10);
		}
		else{
			fprintf(fp, "%d %0.2f %0.2f\n", del->id, fWait / 10, fTurn /10);
		}
		free(del);
		del = NULL;
	}
	return 0;
}

	
int main(int argc, char *argv[])
{
	FILE * fp;
	char * line = NULL;
	char *filename;
	size_t len = 0;
	ssize_t read;
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
		// I have 10x each variable in order to not use floats
		add(f, s * 10, t * 10, true);
	}
	fclose(fp);
	// creating new output file 
	filename = "outputRR.txt";
	fp = fopen(filename, "w");
	
	/* This algorithm will be very interesting as it must iterate through the list and apply the quantum (1.9)
	as well as the context switch (0.1), to do this I will have a check for a zero elapsed variable (Extra)
	and in order to achieve efficency it will be deleted from the list */
	
	ptr = head;
	repeat = true;
	printf("Beginning the Round Robin algorithm...\n--------\n");		
	while(repeat){
		if(ptr->arrival > count){
			ptr = head;
			continue;
		}
		delete(ptr->id, ptr->burst, fp, false);
		
		if(ptr->next != NULL){
			ptr = ptr->next;
		}
		else{
			if(head->next != NULL){
				ptr = head;
				continue;
			}
			else{
				repeat = false;
				continue;
			}
		}
		
	}
	//This loop is not perfect and leaves a single entry in the list, that will now be manually parsed through to delete
	delete(head->id, head->burst, fp, true);
	printf("Rhys Reid Mallia, s3656436, COSC1114 :)\n");
	fclose(fp);
	return 0;
}