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
	
int main(int argc, char *argv[])
{
	FILE * fp;
	char * line = NULL;
	char *filename;
	size_t len = 0;
	ssize_t read;
	int current = 0;
	int turnaround = 0;
	int waiting = 0;
	struct t_struct *ptr = NULL;
	
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

	//Loop to begin tokenizing everything into a useable linked list
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
	//This code is in order to output my data into a file usable in my python script, so that I may produce graphs and the such
	filename = "outputFCFS.txt";
	fp = fopen(filename, "w");
	
	//beginning to iterate over the queue, while waiting the appointed amount of burst.
	//this is kind of easy because the processes can come in while the burst in happening
	//and it isn't nessicary to check every few seconds to see if the next has arrived
	
	ptr = head;
	printf("Beginning the First Come First Serve algorithm...\n--------\n");
	while(ptr->next != NULL){
		current = 0;
		while(current < ptr->burst){
			count++;
			current++;
		}
		turnaround = count - ptr->arrival;
		waiting = turnaround - ptr->burst;
		printf("ID: %d, Waiting time: %d, Turnaround time: %d\n--------\n", ptr->id, waiting, turnaround);
		fprintf(fp, "%d %d %d\n", ptr->id, waiting, turnaround);
		ptr = ptr->next;
	}
	/* As the last values are not used in the loop, manually added, I can luckily do this in a hard
	coded fashion as last one comes last :) */
	
	current = 0;
	while(current < ptr->burst){
		count++;
		current++;
	}
	turnaround = count - ptr->arrival;
	waiting = turnaround - ptr->burst;
	printf("ID: %d, Waiting time: %d, Turnaround time: %d\n--------\n", ptr->id, waiting, turnaround);
	fprintf(fp, "%d %d %d", ptr->id, waiting, turnaround);
	printf("Rhys Reid Mallia, s3656436, COSC1114 :)\n");
	fclose(fp);
	return 0;
}
	