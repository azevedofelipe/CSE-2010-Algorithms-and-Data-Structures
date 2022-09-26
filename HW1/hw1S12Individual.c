/*
  Author: Felipe Azevedo
  Email: fazevedo2017@my.fit.edu
  Course:CSE 2010
  Section: 02

  Description of this file:
	Program to assign representatives with customers and put customers on hold for a chatroom
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct reps{	
	char rep_name[20];					//Name of rep in list
	int time;						//Time rep returned from a call
	struct reps *next;
}*rep;

struct on_hold{
	char hold_name[20];					//Name of customer on hold
	int time;					//Time when put on hold
	struct on_hold *next;			//Next node
}*on_hold;

struct sessions{
	char current_customer[20];		//Name of customer in call		
	char current_rep[20];			//Name of rep on call
	int session_start;				//Time call started
	int request_time;				//Time customer requested chat
	struct sessions *next;			//Next node
}*sessions;

//Adds customer to on hold list
void append_hold(struct on_hold *head_ref, char *name,int time){						//Use this for onhold
	struct on_hold *N,*P;
		N = malloc(sizeof(on_hold));
		strcpy(N->hold_name, name);
		N->time = time;
		N->next = NULL;
		
		if(head_ref == NULL){
			head_ref = N;
		}else{
			P = head_ref;
			while(P->next != NULL)
				P = P->next;
			P->next = N;
		}
}

//Adds rep into available rep list
void append_rep(struct reps *head_ref, char *name,int time){					
	struct reps *N,*P;
	N = malloc(sizeof N);
	strcpy(N->rep_name, name);
	N->time = time;
	N->next = NULL;	
	if(head_ref == NULL){
		head_ref = N;
	}else{
		P = head_ref;
		while(P->next != NULL)
			P = P->next;
		P->next = N;
	}
}

//Removes rep from available rep list
void delete_rep(struct reps *rep){
	struct reps *todelete;
	if(rep == NULL)
		printf("List is empty");
	else{
		todelete = rep;
		rep = rep->next;
		free(todelete);
	}
}

//Deletes customer on hold from hold list
void delete_hold(struct on_hold *hold){
	struct on_hold *todelete;
	if(hold == NULL)
		printf("List empty");
	else{
		todelete = hold;
		*hold = *hold->next;
		free(todelete);
	}
}

void rep_assignment(char customer[], int time, char waitorlater[], rep *free_reps){				//Function to assign rep to chatter
	char wait[] = "wait";
	int newtime;
	if(free_reps == NULL){											//If no free reps
		if(strcmp(waitorlater,wait) == 0){							//If customer chose wait, then put on hold
			on_hold *new_hold = malloc(sizeof(on_hold));
			strcpy((*new_hold)->hold_name, customer);
			(*new_hold)->time = time;
			printf("PutOnHold %s %d", customer, time);				//Prints PutOnHold customer time
		}else{
			printf("TryLater %s %d", customer, time);				//Prints trylater customer time
		}
	}else{
		sessions *new_session = malloc(sizeof(sessions));							//Create new list node for new session 			APPEND THIS
		strcpy((*new_session)->current_customer, customer);							//Copies customer name to new session
		strcpy((*new_session)->current_rep, (*free_reps)->rep_name);						//Copies rep name to new session
		if((*free_reps)->time > time){												//If reps time is more recent than customers time
			newtime = (*free_reps)->time;
		}else{
			newtime = time;
		}
	//delete_rep;
	printf("RepAssignment %s %s %d\n", customer, (*new_session)->current_rep, newtime);
	}
}


//Quit on hold function, removes customer from hold list
void quit_on_hold(int quit_time, char customer[], on_hold *hold){
	if((*hold) == NULL)
		printf("ERROR: HOLD LIST EMPTY\n");
	else{
		while(strcmp((*hold)->next->hold_name, customer)){
			(*hold) = (*hold)->next;
		}
		printf("QuitOnHold %d %s \n", quit_time, customer);
	}
}

//Chat ended function, deletes chat session node and places rep into available list
void chat_ended(char customer[], char rep_name[], int time, sessions *session){
	while(strcmp((*session)->next->current_rep, rep_name))
		(*session) = (*session)->next;
	(*session) = (*session)->next;
	if(strcmp((*session)->current_rep, rep_name)){
		printf("ChatEnded %s %s %d \n", customer, rep_name, time);
		struct reps *new_rep = malloc(sizeof(struct reps));
		strcpy(new_rep->rep_name, rep_name);
		new_rep->time = time;
		
	}else{
		printf("ERROR NO MATCHING SESSION FOUND\n");
	}
}

//Checks to see if anyone is on hold, if they are assign rep to them
void check_holds(on_hold *hold, rep *rep){
	char wait[] = "wait";
	if((*hold)==NULL){
		return;
	}else{
		rep_assignment((*hold)->hold_name, (*hold)->time, wait, rep);
		on_hold *todelete = hold;
		(*hold) = (*hold)->next;
		free(todelete);
	}
}

//Prints list of available reps
void print_rep_list(int time, struct reps *rep){
	struct reps *N;
	N = rep;
	while(N != NULL){
		printf("%s ", N->rep_name);
		N=N->next;
	}
}

//Calculates max wait time of people on hold
void maxwaittime(int reptime, int custime){				
	int waittime;
	if(reptime >= custime)
		waittime = (reptime - custime) - 40;
	else
		waittime = (custime - custime) - 40;
}

int main(int argc, char* argv[])
{
	printf("THISWORKS");
	//Initializing the linked lists
	struct reps *rep_list = NULL;
	//on_hold *hold_list = malloc(sizeof(on_hold));
	//sessions *session_list = malloc(sizeof(sessions));
	
	//Create rep list
	append_rep(rep_list,"Alice",0);
	append_rep(rep_list,"Bob",0);
	append_rep(rep_list,"Carol",0);
	append_rep(rep_list,"David",0);
	append_rep(rep_list,"Emily",0);
	printf("\nAPPENded");
	printf("%s", rep_list->rep_name);
	print_rep_list(1000, rep_list);
	
	//Reads the input file from command line and stores it in 2d array of chars for each line
	 FILE *input;
	 char *filename;
	 char ch;
	 
	 if(argc < 2){
		 printf("Missing file name");
		 return(1);
	 }else{
		 filename = argv[1];
		 printf("Filename: %s\n", filename);
	 }
	 input = fopen(filename, "r"); //Opens file to read only access
	 
	 char line[100];		//string of current line
	 char array_of_lines[20][100]; 		// 2d array of every line in the text file
	 int total_lines = 0;			//counts the total number of lines in the text file
	 
	 if(input){
		 while(fgets(line, 100, input) != NULL){		//Loops through the entire text file
			 strcpy(array_of_lines[total_lines], line);		//Copies each current line to the array of lines
			 total_lines++;							//Increments
		 }
	 }else{
		 printf("File could not be read");
	 }//end of input file reading
	
	//string for current line and array for words in line
	char current_line[100];
	char split_words[5][40];		//stores 5 words of 40 chars for each line(5 max parameter for customerorder)
  
	//Strings to compare input text with to see what command to execute
	char chat_request_order[] = "ChatRequest";
	char quit_on_hold_order[] = "QuitOnHold";
	char chat_ended_order[] = "ChatEnded";
	char print_rep_list_order[] = "PrintAvailableRepList";
	char print_wait_time_order[] = "PrintMaxWaitTime";
	
	for(int i=0; i<total_lines; i++){		 //Loops through each line of text in the file, this is where main if statements go
		strcpy(current_line, array_of_lines[i]);			//Assigns the current line to current_line
		int l, count = 0;
		for(int j=0; j<=(strlen(current_line));j++){						//For loop to isolate each word in text file line
			if(current_line[j] == ' '|| current_line[j]=='\0'){				//If line is blank or at end of line, go to next line
				split_words[count][l] = '\0';
				split_words[count][l] = '\0';
				count++;					//Goes to next line
				l=0;						//Resets back to start of line
			}else{
				split_words[count][l] = current_line[j];					//Inserts current char into split_words
				l++;														// Goes to next char
				}
		}//end of for word for loop
	}
		
		
		if(strcmp(split_words[0],chat_request_order) == 0){
			int time = atoi(split_words[1]);						//Should maybe make function to split time into hh and mm
			printf("ChatRequest %d %s %s", time, split_words[2], split_words[3]);
			rep_assignment(split_words[2], time, split_words[3], rep_list);
		}
		if(strcmp(split_words[0], quit_on_hold_order) == 0){
			int time = atoi(split_words[1]);
			quit_on_hold(time, split_words[2], hold_list);
		}
		if(strcmp(split_words[0], chat_ended_order) == 0){
			int time = atoi(split_words[3]);
			chat_ended(split_words[1], split_words[2], time, session_list);			//CREATE FUNCTION
			check_holds(hold_list, rep_list);																//CREATE FUNCTION
		}
		if(strcmp(split_words[0], print_rep_list_order) == 0){
			int time = atoi(split_words[1]);
			print_rep_list(time, rep_list);				//CREATE FUNCTION
		}
		if(strcmp(split_words[0], print_wait_time_order) == 0){
			int time = atoi(split_words[1]);
			//print_wait_time(time);				//CREATE WAIT TIME FUNCTION
		}
		return 0;
}

