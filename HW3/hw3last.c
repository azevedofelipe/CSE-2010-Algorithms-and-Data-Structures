/*
  Author: Felipe Azevedo
  Email: fazevedo2017@my.fit.edu
  Course: CSE2010	
  Section: 2
  Description of this file:Program to perform searches of hierarchy of a tree
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Struct of tree nodes
struct node{
	char *name;									//Stores the nodes name
	char *parentname;							//Stores the nodes parent name
	struct node *parent;
	struct node *next;
	struct node *child;
};
typedef struct node NODE;
NODE *root,*temp,*temp2;						//Initializes the needed node pointers

//Function to create a new tree node with childname and fathername
NODE * createNode(char *childname, char *fathername){
	NODE *new_node = malloc(sizeof(NODE));
	if(new_node){
		new_node->name = childname;
		new_node->parentname = fathername;
		new_node->next = NULL;
		new_node->parent = NULL;
		new_node->child = NULL;
	}
	return new_node;
}

//Function that recursively searches tree to find the node containing word
NODE * findNode(NODE *root,char *word){										
	if(root == NULL){
		return NULL;
	}else if(strcmp(root->name,word) == 0){									//If current node matches the word return that node
		return root;
	}else{
		NODE *x = findNode(root->child,word);								
		if(x)																//If node has a child go down to that child node
			return x;
		return findNode(root->next,word);									//Loop through all siblings of current node
	}
}
//Function to traverse tree in preorder form and print all nodes of that parent
void traversalPrint(NODE *root){
	if(root == NULL){
		return;
	}
	printf("%s ",root->name);
	traversalPrint(root->child);											//Recursively loops through all child nodes of specified node and prints them
	traversalPrint(root->next);
}
//Recursive function to count total children of a node
int traversalCount(NODE *root){
	if(root == NULL){
		return 0;
	}else{																	//Recursively traverses all children of node and returns total num of child nodes
		return 1 + traversalCount(root->child) + traversalCount(root->next);
	}
}
//Traverses through specified node and returns every node below it in preorder format
NODE * traversal(NODE *root){
	if(root == NULL){
		return NULL;
	}
	return root;
	traversal(root->child);
	traversal(root->next);
}
//Function to add Child node to correct parent node
void addChild(NODE *root, NODE *child){
	root = findNode(root,child->parentname);								//Finds the correct parent node needed for the child node
	child->parent = root;													//Set child nodes parent to parent node
	if(root->child != NULL){												//If parent has children
		root = root->child;													//Set current node to first child and traverse through siblings until last one
		while(root->next != NULL){											//Might need to be root->next
			root = root->next;
		}
		root->next = child;
	}else if(root->child == NULL){
		root->child = child;
	}
}

//Function that returns the first child of a node
NODE * getChildren(NODE *current){
	if(current->child){														//If the node has a child return the first one else print error
		return current->child;
	}else{
		printf("Error, %s has no children\n",current->name);
	}
}

//Function that returns the parent of a node
NODE *getParent(NODE *current){
	if(current->parent != NULL){											//If the node has a parent return it else print error
		return current->parent;
	}else{
		printf("Error, %s has no parent\n",current->name);
	}
}

//Function for finding direct parent of a node
void dirSup(char *word){													
	temp = findNode(root,word);
	if(temp->parent){
		temp = getParent(temp);												//If node has a parent, set node to that parent and print it
		printf("DirectSupervisor %s %s\n",word,temp->name);				
	}else{
		printf(" \n");
	}
}

//Function for finding all subordinates of a node
void dirSubor(char *word){
	temp = findNode(root,word);
	temp = getChildren(temp);
	printf("DirectSubordinates %s ",word);
	while(temp->next != NULL){												//Set node to first child node then print each sibling until NULL
		printf("%s ",temp->name);
		temp = temp->next;
	}
	printf("%s\n",temp->name);
}
//Function for finding all supervisors of a node
void allSuper(char *word){
	temp = findNode(root,word);
	temp = temp->parent;
	printf("AllSupervisors %s ",word);
	while(temp != NULL){													//Assign node to parent then loop through all parents until NULL
		printf("%s ",temp->name);
		temp = temp->parent;
	}
	printf("\n");
}
//Function that prints all subordinates of a node
void allSubs(char *word){
	temp = findNode(root,word);												//Finds node with matching word
	printf("AllSubordinates %s ",word);
	if(temp->child){														//If node has children
		temp = temp->child;													//Go to first child
		traversalPrint(temp);												//Call traversalPrint to print every child
	}else{
		printf(" ");
	}
	printf("\n");
}
//Function that counts number of parents of a node and prints it
void numSups(char *word){
	int count = 0;
	temp = findNode(root,word);
	if(temp->parent){
		temp = temp->parent;
		while(temp != NULL){
			temp = temp->parent;
			count++;
		}
	}else{
		printf("No Parents\n");
	}
	printf("NumberOfAllSuperVisors %s %d\n",word,count);
}
//Function that prints number of all children of a node
void numSubs(char *word){				
	temp = findNode(root,word);
	temp = temp->child;
	int total;
	total = traversalCount(temp);
	printf("NumberOfAllSubordinates %s %d\n",word,total);
}
//Function that checks if superior word is a father of the child word in the tree
void isSuper(char *child, char *superior){
	temp = findNode(root,child);
	temp = temp->parent;
	int check = 0;
	while(temp != NULL){
		if(strcmp(temp->name,superior) == 0){
			check = 1;
			break;
		}else{
			check = 0;
		}temp = temp->parent;
	}
	printf("IsSupervisor %s %s ",child,superior);
	if(check == 1){
		printf("yes\n");
	}else{
		printf("no\n");
	}
}
//Checks if superior node has child with name of child string
void isSubor(char *superior, char *child){
	temp = findNode(root,superior);
	int count = 0;
	printf("IsSubordinate %s %s ",superior,child);
	if(temp->child){
		temp = temp->child;
		temp = traversal(temp);
		if(strcmp(temp->name,child) == 0){
			count++;
		}
	}
	if(count > 0){
		printf("yes\n");
	}else{
		printf("no\n");
	}
}

//Function that returns all siblings of a node
NODE *getSiblings(NODE *root){
	if(root->next != NULL){
		while(root!=NULL){											
			root = root->next;
			return root;
			
		}
	}
}

//Function to compare rank of selected with comparer and print if its higher lower or same
void compRank(char *selected, char *comparer){
	printf("CompareRank %s %s ",selected,comparer);
	temp = getParent(findNode(root,comparer));
	temp2 = getParent(findNode(root, selected));
	char *comparerParent = temp->name;
	char *selectedParent = temp2->name;
	int found, same = 0;
	if(strcmp(temp->name,temp2->name) == 0){					//If they have the same parent, print same
		same++;
	}
	if(temp->next != NULL){										//If they have siblings loop through them and see if both nodes are on same level
		temp = getSiblings(temp);
		if(strcmp(temp->name,selectedParent) == 0){
			same++;
		}
	}if(temp2->next != NULL){
		temp2 = getSiblings(temp2);
		if(strcmp(temp2->name,comparerParent) == 0){
			same++;
		}
	}
	if(temp->child){				
		temp = findNode(root,comparer);
		temp = temp->child;
		temp2 = findNode(temp,selected);									//Loops through children and increments if node is in child
		if(temp2 == temp){
			found++;
		}
	}if(same > 0){
		printf("same\n");
	}else if(found > 0){
		printf("lower\n");
	}else{
		printf("higher\n");
	}
	
}
//Function to find closest common parent of 2 nodes
NODE *closestCom(NODE *root,NODE *node1, NODE *node2){
	if(root == NULL){
		return NULL;
	}
	if(root == node1 || root == node2){
		return root;
	}else{
		NODE *l = closestCom(root->child,node1,node2);
		NODE *r = closestCom(root->next,node1,node2);
		if(l&&r){
			return root;
		}else if(l){
			return l;
		}else{
			return r;
		}
	}
}

//Main
int main(int argc, char* argv[]){
	//Variables for reading in file
	 FILE *input,*input2;
	 char *filename,*filename2;
	 char line[100],line2[100];											//String of current line of file
	 char array_of_lines[100][100],array_of_lines2[100][100];			//2D array of every line in the text file
	 int total_lines,total_lines2 = 0;									//Counts the total number of lines in the text file
	 
	 if(argc < 3){	
		 printf("Missing files");
		 return(1);
	 }else{
		 filename = argv[1];									//Sets first file to 2nd argument in command line
		 filename2 = argv[2];									//Sets 2nd file to 3rd argument in command line
	 }
	 input = fopen(filename, "r"); 								//Opens both files
	 input2 = fopen(filename2, "r"); 							//to read only mode
	 
	 //Reads in first input file
	 if(input){
		 while(fgets(line, 100, input) != NULL){				//Loops through the entire text file
			 line[strlen(line)-2]= '\0';
			 strcpy(array_of_lines[total_lines], line);			//Copies each current line to the array of lines
			 total_lines++;										//Increments
		 }
	 }else{
		 printf("File could not be read");
	 }
	 
	 //Reads in the 2nd input file
	 if(input2){
		 while(fgets(line2, 100, input2) != NULL){				//Loops through the entire text file
			 line2[strlen(line2)-2]= '\0';
			 strcpy(array_of_lines2[total_lines2], line2);		//Copies each current line to the array of lines
			 total_lines2++;									//Increments
		 }
	 }else{
		 printf("File could not be read");
	 }
	 
	 //Variables for first input file
	 char current_line[100];
	 char split_words[100][100],words[100][100];
	 int wordcount = 0;
	 char *rootname;
	 
	 //Loops through each line in the first input file
	 for(int i=0; i<total_lines; i++){
		strcpy(current_line, array_of_lines[i]);							//Assigns the current line to current_line
		if(i == 0){															//If first line of text file, copy that line to rootname
			rootname = array_of_lines[i];									
		}
		int l, count = 0;
		for(int j=0; j<=(strlen(current_line));j++){						//For loop to isolate each word in text file line
			if(current_line[j] == ' '|| current_line[j]=='\0'){				//If line is blank or at end of line, go to next line
				split_words[count][l] = '\0';
				strcpy(words[wordcount],split_words[count]);
				//printf("WORD %d %s\n",wordcount,words[wordcount]);
				count++;				
				wordcount++;
				l=0;		
			}else{
				split_words[count][l] = current_line[j];					//Inserts current char into split_words
				l++;														// Goes to next char
				}
		}//end of for word for loop
	}
	
	//Variables for second input file
	char current_line2[100];
	char split_words2[100][100],words2[100][100];
	int wordcount2 = 0;
	
	//Loops through second input file and separates every word in an array of words
	for(int k=0;k<total_lines2;k++){
		strcpy(current_line2,array_of_lines2[k]);							//Assigns current line in array_of_lines2 to current_line2
		int z, count2 = 0;
		for(int c=0;c<=(strlen(current_line2));c++){						//Loops through each char in the line
			if(current_line2[c] == ' ' || current_line2[c]=='\0'){			//If you reach a space or end of line separate that line
				split_words2[count2][z] = '\0';								//Add null to end of word
				strcpy(words2[wordcount2],split_words2[count2]);			//Copy the current word into the array of words
				//printf("WORD %d %s\n",wordcount2,words2[wordcount2]);
				count2++;													//Increment both arrays by 1
				wordcount2++;												
				z=0;														//Reset split_words2 length back to 0 for new word
			}else{
				split_words2[count2][z] = current_line2[c];					//If not end of word add char to current word and increase length of word by 1
				z++;
			}
		}//end of split words for loop
	}//end of line by line reading
	
	root = createNode(rootname,NULL);										//Create root node with first line of text file
	//Loop through array of words in pairs of 2
	NODE *newnode;
	char *fathername, *childname;
	for(int k = 1; k<wordcount-1;k+=2){
		fathername = words[k];						
		childname = words[k+1];
		newnode = createNode(childname,fathername);							//Creates a new node with child and parentname in the node
		addChild(root,newnode);												//Adds the newly created newnode to the tree in the correct spot
	}
	
	//Strings to compare each line input to for functions
	char *directSupervisor = "DirectSupervisor";
	char *directSubordinates = "DirectSubordinates";
	char *allSupervisors = "AllSupervisors";
	char *allSubordinates = "AllSubordinates";
	char *numSupervisors = "NumberOfAllSupervisors";
	char *numSubordinates = "NumberOfAllSubordinates";
	char *isSup = "IsSupervisor";
	char *isSub = "IsSubordinate";
	char *compareRank = "CompareRank";
	char *closestCommon = "ClosestCommonSupervisor";
	
	//Loops through 2nd input file line by line and compares to every possible function, then executes that matching function
	NODE *test1, *test2;
	int wordnum = 0;
	while(wordnum<wordcount2){
		if(strcmp(words2[wordnum],directSupervisor) == 0){
			dirSup(words2[wordnum+1]);
			wordnum+=2;
		}
		if(strcmp(words2[wordnum],directSubordinates) == 0){
			dirSubor(words2[wordnum+1]);
			wordnum+=2;
		}
		if(strcmp(words2[wordnum],allSupervisors) == 0){
			allSuper(words2[wordnum+1]);
			wordnum+=2;
		}
		if(strcmp(words2[wordnum],allSubordinates) == 0){
			allSubs(words2[wordnum+1]);
			wordnum+=2;
		}
		if(strcmp(words2[wordnum],numSupervisors) == 0){
			numSups(words2[wordnum+1]);
			wordnum+=2;
		}
		if(strcmp(words2[wordnum],numSubordinates) == 0){
			numSubs(words2[wordnum+1]);
			wordnum+=2;
		}
		if(strcmp(words2[wordnum],isSup) == 0){
			isSuper(words2[wordnum+1],words2[wordnum+2]);
			wordnum+=3;
		}
		if(strcmp(words2[wordnum],isSub) == 0){
			isSubor(words2[wordnum+1],words2[wordnum+2]);
			wordnum+=3;
		}
		if(strcmp(words2[wordnum],compareRank) == 0){
			compRank(words2[wordnum+1],words2[wordnum+2]);
			wordnum+=3;
		}
		if(strcmp(words2[wordnum],closestCommon) == 0){
			temp = findNode(root,words2[wordnum+1]);
			temp2 = findNode(root,words2[wordnum+2]);
			temp = closestCom(root,temp,temp2);
			printf("ClosestCommonSupervisor %s %s %s\n",words2[wordnum+1],words2[wordnum+2],temp->parent->name);
			wordnum+=3;
		}
	}
}