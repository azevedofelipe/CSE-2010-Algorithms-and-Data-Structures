/*
  Author: Felipe Azevedo
  Email: fazevedo2017@my.fit.edu
  Course: CSE 2010
  Section: 2
  Description: Program to add delete and print events on a users timeline using a skip list
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "fakeRandHeight.h"
#include "skipList.h"
#define SKIPLIST_MAX_LEVEL 5
#define MAX_KEY 1300

int main(int argc, char* argv[]){
  FILE *input;
  char *filename;
  char line[100];
  char array_of_lines[100][100];
  int total_lines = 0;

  //Reads in text file input
  if(argc < 2){
		printf("Missing files");
		return(1);
	}else{
		filename = argv[1];
	}
	input = fopen(filename, "r");
	if(input){
		while(fgets(line, sizeof(line), input)){
			line[strlen(line)-2]= '\0';
			strcpy(array_of_lines[total_lines], line);		
			total_lines++;							
		}
	}else{
		printf("File could not be read\n");
	}

	char split_words[100][100];
	int key, key2;
	char *title;

	//Creates skiplist list and initializes it with MAX_INT of 1300 as head
	skiplist list;
	skiplist_init(&list);

	//Goes line by line and seperates line word by word in split_words
	for(int i=0; i<total_lines; i++){
		int l, count = 0;
		//Isolates each line into single words by spaces
		char *word = strtok(array_of_lines[i]," ");
		while(word != NULL){
			strcpy(split_words[count],word);
			word = strtok(NULL," \n");
			count++;
		}
		//Displays a particular event
		if(strcmp(split_words[0],"DisplayEvent") == 0){
			key = atoi(split_words[1]);
			snode *event = skip_find(&list,key);
			printf("DisplayEvent %04d ",key);
			if(event){
				printf("%s \n",event->value);
			}else{
				printf("none\n");
			}
		}
		//Adds an event to the list
		if(strcmp(split_words[0],"AddEvent") == 0){
			key = atoi(split_words[1]);
			word = split_words[2];
			printf("AddEvent %04d %s ",key,word);
			snode *node = skip_find(&list,key);
			if(node){ //If date already exists, replace description of date at each level
				strcpy(node->value,word);
				for(int i=node->n_height-1; i>=0; i--){
					node = skip_find_at_level(&list,key,i);
					strcpy(node->value,word);
				}
				printf("replacingExistingEvent\n");
			}else{ //Else insert new node
				skip_insert(&list, key, word);
				printf("success \n");
			}
		}
		//Deletes a specific event by date
		if(strcmp(split_words[0],"DeleteEvent") == 0){
			key = atoi(split_words[1]);
			printf("DeleteEvent %04d ",key);
			skip_delete(&list,key);
		}
		//Displays events between two dates
		if(strcmp(split_words[0],"DisplayEventsBetweenDates") == 0){
			key = atoi(split_words[1]);		//Start date
			key2 = atoi(split_words[2]);	//End date
			printf("DisplayEventsBetweenDates %04d %04d ", key, key2);
			//Find ceiling of start date and floor of end date and print until end date
			snode *min = skip_find_ceiling(&list,key);
			snode *max = skip_find_floor(&list,key2);
			if(min && max){
				while(min->data <= max->data){
					printf("%04d:%s ",min->data, min->value);
					min = min->forward[0];
				}
				printf("\n");
			}else{
				printf("none\n");
			}
		}
		//Displays events from ceiling of a given date
		if(strcmp(split_words[0],"DisplayEventsFromStartDate") == 0){
			key = atoi(split_words[1]);
			printf("DisplayEventsFromStartDate %04d ",key);
			snode *ceiling = skip_find_ceiling(&list,key);
			if(ceiling){
				//Prints all events from the given ceiling date until MAX_KEY
				while(ceiling->data != MAX_KEY){
					printf("%04d:%s ",ceiling->data, ceiling->value);
					ceiling = ceiling->forward[0];
				}
				printf("\n");
			}else{
				//If there is no ceiling to the given key
				printf("none\n");
			}
		}
		//Prints all events up to the floor of a certain date
		if(strcmp(split_words[0],"DisplayEventsToEndDate") == 0){
			key = atoi(split_words[1]);
			printf("DisplayEventsToEndDate %04d ", key);
			snode *end = skip_find_floor(&list, key);
			snode *curr = list.head->forward[0];
			if(end){
				while(curr != end->forward[0]){
					printf("%04d:%s ",curr->data, curr->value);
					curr = curr->forward[0];
				}
				printf("\n");
			}else{
				printf("none\n");
			}
		}
		//Prints bottom level of skip list with all events
		if(strcmp(split_words[0],"DisplayAllEvents") == 0){
			printf("DisplayAllEvents ");
			snode *curr = list.head->forward[0];
			if(curr->data != MAX_KEY){
				while(curr->data != MAX_KEY){
					printf("%04d:%s ",curr->data, curr->value);
					curr = curr->forward[0];
				}
			}else{
				printf("none");
			}
			printf("\n");
		}
		//Prints every level of the skip list
		if(strcmp(split_words[0],"PrintSkipList") == 0){
			skip_print(&list);
		}	
	}
}