/*
  Author: Felipe Azevedo
  Email: fazevedo2017@my.fit.edu
  Course:CSE2010
  Section:2

  Description of this file: Using recursive algorithms to find all possible palindromes in a string of pallength words
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Singly linked list of words from text file
typedef struct list_words{
	char word[20];
	struct list_words *next;
}word_node;

//Global variable pal_length which is in command line
int pal_length;

//Function to append words to list of words from text file
void append_words(char *word, word_node **L){
	word_node *N, *P;
	
	N = (word_node*)malloc(sizeof(word_node));
	strcpy(N->word, word);
	N->next = NULL;
	
	if(*L == NULL){
		*L = N;
	}else{
		P=*L;
		while(P->next != NULL)
			P=P->next;
		
		P->next = N;
	}
}


//Function to delete specified word in list
void delete_words(word_node **L, char *word){
	word_node *temp = *L, *prev;
	
	if(temp != NULL && strcmp(temp->word,word) == 0){			//Checks if head node has the keyword to delete
		*L = temp->next;
		free(temp);
		return;
	}while(temp != NULL && (strcmp(temp->word,word) != 0)){			//Loops through list until finds node with keyword to delete
		prev = temp;												
		temp = temp->next;
	}
	
	if(temp == NULL)
		return;
	
	prev->next = temp->next;
	free(temp);
}

//Function to check if a string is a palindrome, return 1 if palindrome
int check_palindrome(char string[],int s, int e){
	
	if(s == e)
		return 1;
	if(string[s] != string[e])
		return 0;
	if(s<e+1)
		return check_palindrome(string, s+1, e-1);
	
	return 1;
}

//Function to copy linked list to new linked list
word_node *copy(word_node *list){
	if(list == NULL)
		return NULL;
	
	word_node *newHead = malloc(sizeof(word_node));
	strcpy(newHead->word,list->word);
	
	word_node *p = newHead;
	list = list->next;
	while(list != NULL){
		p->next = malloc(sizeof(word_node));
		p = p->next;
		strcpy(p->word,list->word);
		list = list->next;
	}
	p->next = NULL;
	return newHead;
}

//Recursive function to find all possible permutations of a list of words
void puzzle_solve(int pal_length, char *S, word_node **U){
	struct list_words *Unew;
	Unew = copy(*U);
	
	char old_S[20];
	char temp_word[20];
	
	for(Unew; Unew!=NULL; Unew = Unew->next){
		strcpy(old_S,S);
		strncat(S,Unew->word,strlen(Unew->word));				//Adds word to end of string
		strcpy(temp_word,Unew->word);
		delete_words(&Unew, Unew->word);						//Removes word from list of words
		if(pal_length == 1){
			int e = strlen(S);
			if(check_palindrome(S, 0, e))						//Checks if string is a palindrome and prints it if it is
				printf("%s\n", S);				
			else
				return;
		}else{
			puzzle_solve(pal_length-1,S,&Unew);					//Calls function with pallength-1 and new string and list of words
		}
		strcpy(S,old_S);
		append_words(temp_word, &Unew);
		
	}
}

int main(int argc, char* argv[]){
	
	struct list_words *text = (struct list_words*)malloc(sizeof(struct list_words));
	text = NULL;
	
	
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
	pal_length = atoi(argv[2]);						//Pallength in command line
	input = fopen(filename, "r"); 						//Opens file to read only access
	 
	char line[20];												//string of current line
	if(input){
		while(fgets(line, 20, input) != NULL){				//Loops through the entire text file
			line[strlen(line)-2]= '\0';
			append_words(line,&text);
		}
	}else{
		printf("File could not be read");
	}//end of input file reading
	
	//printf("%s", text->word);
	
	char string_words[50];								
	puzzle_solve(pal_length,string_words,&text);
	printf("Pallength: %d", pal_length);
}

