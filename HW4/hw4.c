/*
  Author: Felipe Azevedo
  Email: fazevedo2017@my.fit.edu
  Course: CSE 2010
  Section: 2

  Description of this file:
  Program to recommend songs to a target customer from a customer with the same tastes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Nodes for customers and their ratings
struct node{
	char name[10];
	int ratings[10];
	float distance;
};
//Global variables
typedef struct node Customer;
Customer *list[100];
Customer *distances[100];
int size = -1;

//Function to create a new tree node with childname and fathername
Customer * createNode(char cusName[], int rating[]){
	Customer *new_node = malloc(sizeof(Customer));
	if(new_node){
		strcpy(new_node->name,cusName);
		new_node->distance = 0.000;
		for(int i = 0; i<10;i++){
			new_node->ratings[i] = rating[i];
		}
	}
	return new_node;
}
//Function to insert customer into array of pointers
void insertCustomer(Customer *list[],char name[],int rating[]){
	Customer *temp;														//CHANGE to make sure its in alphabetical order
	temp = createNode(name,rating);
	int count = 0;
	while(list[count]){
		count++;
	}
	list[count] = temp;
}
//Function to print distance ratings of all customers to target
void printDistance(Customer *list[],int size){
	printf("PrintCustomerDistanceRatings\n");
	for(int i =0; i<size-1;i++){
		if(list[i]->distance == 0.0){		//CHANGE this to grab distance from PQ and do findCustomer to match rating with the current cust
			printf("      ");
		}else{
			printf("%.3f ",list[i]->distance);	
		}
		printf("%-10s ",list[i]->name);
		for(int k=0;k<10;k++){
			printf("%d ",list[i]->ratings[k]);
		}
		printf("\n");
	}
}
//Function to calculate distance of all customers to target customer
void calcDistance(Customer *target,Customer *list[]){
	int count = 0;
	float dist;
	int custRate,targRate;
	
	while(list[count]){
		float size = 0.0;
		float total = 0.0;
		if(strcmp(target->name,list[count]->name) == 0){
			dist = 0.000;
		}else{
			//Loops through the ratings
			for(int i=0;i<10;i++){
				//Checks if both target and cust rated the same song, increments if they did
				custRate = list[count]->ratings[i];
				targRate = target->ratings[i];
				if(custRate != 0 && targRate != 0){
					total = total + abs(custRate-targRate);
					size++;
				}
			}
			dist = (1/size) + ((1/size)*total);
			list[count]->distance = dist;						//CHANGE this to add distance to PQ instead and sort PQ from there
		}
		count++;
	}
}
//Siftdown function for name heapsort
void siftDown_name(Customer *strs[],int root, int bottom){
	int done, maxChild;
	Customer *temp;
	
	done = 0;
	while((root*2 <= bottom) && (!done)){
		if(root*2 == bottom){
			maxChild = root * 2;
		}else if(strcmp(strs[root*2]->name,strs[root*2 + 1]->name) > 0){
			maxChild = root * 2;
		}else{
			maxChild = root * 2 + 1;
		}
		if(strcmp(strs[root]->name,strs[maxChild]->name) < 0){
			temp = strs[root];
			strs[root] = strs[maxChild];
			strs[maxChild] = temp;
			root = maxChild;
		}else{
			done = 1;
		}
	}
}
//siftDown function for distance heapsort
void siftDown_distance(Customer *strs[],int root, int bottom){
	int done, maxChild;
	Customer *temp;
	
	done = 0;
	while((root*2 <= bottom) && (!done)){
		if(root*2 == bottom){
			maxChild = root * 2;
		}else if(strs[root*2]->distance > strs[root*2 + 1]->distance){
			maxChild = root * 2;
		}else{
			maxChild = root * 2 + 1;
		}
		if(strs[root]->distance < strs[maxChild]->distance){
			temp = strs[root];
			strs[root] = strs[maxChild];
			strs[maxChild] = temp;
			root = maxChild;
		}else{
			done = 1;
		}
	}
}
//Heapsort the list of nodes by name in alphabetical order
void heapSort_name(Customer *strs[], int array_size){
	int i;
	Customer *temp;
	
	for(i = (array_size / 2); i>=0;i--){
		siftDown_name(strs,i,array_size - 1);
	}
	for(i = array_size-1;i>=1;i--){
		temp = strs[0];
		strs[0] = strs[i];
		strs[i]= temp;
		siftDown_name(strs,0,i-1);
	}
}
//Heapsort the list of nodes by distance from target
void heapSort_distance(Customer *strs[], int array_size){
	int i;
	Customer *temp;
	
	for(i = (array_size / 2); i>=0;i--){
		siftDown_distance(strs,i,array_size - 1);
	}
	for(i = array_size-1;i>=1;i--){
		temp = strs[0];
		strs[0] = strs[i];
		strs[i] = temp;
		siftDown_distance(strs,0,i-1);
	}
}
//Used to find target customer in list of nodes, uses binarysearch
Customer * findCustomer(Customer *list[], char name[], int size){
	int bottom = 0;
	int mid;
	int top = size-1;
	
	while(bottom <= top){
		mid = (bottom + top)/2;
		if(strcmp(list[mid]->name, name) == 0){
			return list[mid];
		}else if(strcmp(list[mid]->name,name) > 0){
			top = mid-1;
		}else if(strcmp(list[mid]->name, name) < 0){
			bottom = mid+1;
		}
	}
}
//Swaps two array nodes with eachother
void swap(int pos1, int pos2){
	Customer *temp = distances[pos1];
	distances[pos1] = distances[pos2];
	distances[pos2] = temp;
}
//Returns parent of current node list[i]
int parent(int i){
	return (i-1)/2;
}
//Gets left child of current index
int leftChild(int i){
	return ((2*i)+1);
}
//Gets right child of current index
int rightChild(int i){
	return ((2*i)+2);
}
//if parent is larger swaps positions
void shiftUp(int i){
	while(i > 0 && distances[parent(i)]->distance > distances[i]->distance){
		swap(parent(i),i);
		i = parent(i);
	}
}
//Shifts down priority queue
void shiftDown(int i){
	int maxIndex = i;
	int l = leftChild(i);
	if(l <= size && distances[l]->distance < distances[maxIndex]->distance){
		maxIndex = l;
	}
	int r = rightChild(i);
	if(r <= size && distances[r]->distance < distances[maxIndex]->distance){
		maxIndex = r;
	}
	if(i != maxIndex){
		swap(i,maxIndex);
		shiftDown(maxIndex);
	}
}
//If queue is full(100) print full
void isFull(){
	printf("Queue is full, cant add\n");
}
//If queue is empty print empty
void isEmpty(){
	printf("Queue is empty\n");
}
//Inserts node into queue unless queue is full
void insert(Customer *p){
	if(size >= 99){
		isFull();
		return;
	}
	size = size+1;
	distances[size] = p;
	
	shiftUp(size);
}
//Removes root node and replaces it with next smallest
Customer *removeMin(){
	if(size <= 0){
		isEmpty();
		return NULL;
	}
	Customer *result = distances[0];
	
	distances[0] = distances[size];
	size = size-1;
	
	shiftDown(0);
	return result;
}
//Returns the smallest node distance
Customer *getMin(){
	return distances[0];
}
//Function to recommend songs to targetcust from closestcust
void recommend_songs(Customer *target){
	Customer *closest = getMin();
	int count = 0;
	
	printf("RecommendSongs %s ", closest->name);
	//Loops through ratings of targetcust and closest cust and recommends songs closestcust rated >= 4 and targetcust hasnt rated
	for(int i =0; i<10; i++){
		if(closest->ratings[i] >= 4 && target->ratings[i] == 0){
			printf("song%d %d ",i,closest->ratings[i]);
			count++;
		}
	}
	//If closestcust hasnt rated any songs higher than 3, remove him from queue and recommend from next smallest
	if(count == 0){
		removeMin();
		recommend_songs(target);
	}
	printf("\n");
}
//Main
int main(int argc, char* argv[]){
	//Variables to read files
	FILE *input;
	FILE *input2;
	char *filename;
	char *filename2;
	char line[100];								
	char array_of_lines[100][100];
	int total_lines = 0;
	
	if(argc < 2){
		printf("Missing files");
		return(1);
	}else{
		filename = argv[1];
		filename2 = argv[2];
	}
	input = fopen(filename, "r");
	input2 = fopen(filename2, "r");
	//Reads in input file
	if(input){
		//Loops through text file, copies line by line, increments total_lines for each line
		while(fgets(line, 100, input) != NULL){	
			line[strlen(line)-2]= '\0';
			strcpy(array_of_lines[total_lines], line);		
			total_lines++;							
		}
	}else{
		printf("File could not be read\n");
	}
	//Variables for first input file
	char split_words[100][100];
	char targetCust[10],custName[10];
	int temp;
	int cR[10];
	//Goes line by line and seperates line word by word in split_words
	for(int i=0; i<total_lines; i++){
		//If first line of text file set name as targetCust
		if(i == 0){															
			strcpy(targetCust,array_of_lines[0]);
		}else{
			int l, count = 0;
			//Isolates each word by going char by char until hits space or newline
			for(int j=0; j<=(strlen(array_of_lines[i]));j++){						
				if((array_of_lines[i][j] == ' '|| array_of_lines[i][j]=='\0') && array_of_lines[i][j+1] != ' '){
					split_words[count][l] = '\0';
					count++;				
					l=0;		
				}else{
					if(array_of_lines[i][j] == ' '){
						split_words[count][l] = '\0';
						l++;
					}else{
						split_words[count][l] = array_of_lines[i][j];
						l++;	
					}
				}
			}
			//Copies current custName and ratings for each line in text file starting from 2nd line
			strcpy(custName,split_words[0]);
			for(int k = 1;k<11;k++){
				temp = atoi(split_words[k]);
				cR[k-1] = temp;
			}
			//Creates node of current customer and inserts it into array of pointers of nodes
			insertCustomer(list,custName,cR);
		}
	}
	//List needs to be sorted in order to use binary search
	heapSort_name(list,total_lines-1);
	Customer *targetNode;
	targetNode = findCustomer(list,targetCust,total_lines-1);
	calcDistance(targetNode,list);
	
	//Inserts all customers into priority queue and sorts queue by distance
	for(int i = 1; i<total_lines-1;i++){
		insert(list[i]);
	}
	heapSort_distance(distances,total_lines-2);
	
	//Test
	int z = 0;
	while(z <= size){
		//printf("%.3f %s\n", distances[z]->distance, distances[z]->name);
		z++;
	}
	
	//Variables for 2nd input file
	int action_lines = 0;
	char array_of_lines2[100][100]; 
	char action_current_line[100];
	
	//Reads in the 2nd input file
	if(input2){
		//Loops through 2nd text file, copies line by line, increments total_lines for each line
		while(fgets(action_current_line, 100, input2) != NULL){				
			action_current_line[strlen(action_current_line)-2]= '\0';
			strcpy(array_of_lines2[action_lines], action_current_line);		
			action_lines++;					
		}
	}else{
		printf("File could not be read");
	}
	//Variables for second input file
	char current_line2[100];
	char split_words2[100][100];
	char *add_cus = "AddCustomer";
	char *recommend = "RecommendSongs";
	char *print = "PrintCustomerDistanceRatings";
	//Loops through second input file and separates every word in an array of words
	for(int k=0;k<action_lines;k++){
		strcpy(current_line2,array_of_lines2[k]);							//Assigns current line in array_of_lines2 to current_line2
		int z, count2 = 0;
		for(int c=0;c<=(strlen(current_line2));c++){						//Loops through each char in the line
			if(current_line2[c] == ' ' || current_line2[c]=='\0'){			//If you reach a space or end of line separate that line
				split_words2[count2][z] = '\0';								//Add null to end of word
				count2++;													//Increment both arrays by 1											
				z=0;														//Reset split_words2 length back to 0 for new word
			}else{
				split_words2[count2][z] = current_line2[c];					//If not end of word add char to current word and increase length of word by 1
				z++;
			}
		}
		//Compares each line in action file to every possible command
		if(strcmp(split_words2[0],add_cus) == 0){
			//Adds customer to list
			strcpy(custName,split_words2[1]);
			printf("AddCustomer %s ", custName);
			for(int k = 2; k<12;k++){
				temp = atoi(split_words2[k]);
				cR[k-2] = temp;
				printf("%d ", temp);
			}
			printf("\n");
			insertCustomer(list,custName,cR);
			//Increments total_lines, calcs distance of new customer and inserts into priorityqueue
			total_lines++;
			calcDistance(targetNode,list);
			insert(list[total_lines-2]);
			heapSort_distance(distances,total_lines-2);
		}
		if(strcmp(split_words2[0],recommend) == 0){
			//Recommends songs of closestcust
			recommend_songs(targetNode);
		}
		if(strcmp(split_words2[0],print) == 0){
			//Sorts list by name in alphabetical order, calcs distance and prints list
			heapSort_name(list,total_lines-1);
			calcDistance(targetNode,list);
			printDistance(list,total_lines);
		}
	}
}

