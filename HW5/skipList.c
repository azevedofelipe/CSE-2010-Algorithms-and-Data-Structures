/*
  Author: Phil Chan/Felipe Azevedo
  Email: fazevedo2017@my.fit.edu
  Description:

  An implementation of Skiplist with these operations and more:

  a.  skip_insert(list, key): insert a key to the list
  b.  skip_delete(list, key): delete a key from the list
  c.  skip_find(list, key): find a key and return the node or NULL
  d.  skip_print(list): print the skiplist
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "skipList.h"
#include "fakeRandHeight.h"
#define SKIPLIST_MAX_LEVEL 5
#define MAX_KEY 1300

// create and return a node with data and previous/next nodes
snode* skip_create_node(int data, char *word, snode* forward, int height)
{
  snode *my_node = (snode *)malloc(sizeof(snode));
  my_node->forward = (snode **)malloc(sizeof(snode*) * (SKIPLIST_MAX_LEVEL+1));
  if (my_node == NULL)
    {
      printf("malloc failed, memory is full\n");
      exit(-1);
    }

  my_node->data = data;
  strcpy(my_node->value,word);
  my_node->forward[height] = forward;
  my_node->n_height = height;

  return my_node;
}

//Function to initialize the skiplist
//Sets head of each level of list with MAX_INT of 1300 (not a possible date)
skiplist *skiplist_init(skiplist *list){
  int i;
  snode *header = (snode *)malloc(sizeof(struct snode));
  list->head = header;
  header->data = MAX_KEY;
  strcpy(header->value," ");
  header->forward = (snode **)malloc(sizeof(snode*) * (SKIPLIST_MAX_LEVEL+1));

  for(i = 0; i<= SKIPLIST_MAX_LEVEL; i++){
    header->forward[i] = list->head;
  }
  //Sets height to 1 to be able to loop first insert for(i<height) would loop once
  list->height = 1;
  return list;
}


// insert a key and maintain the ascending order of DLL
// assuming keys are unique
void skip_insert(skiplist *list, int key, char *word){
  snode *new_node = NULL;   // node to be inserted
  int height = getRandHeight();
  int i;
  //Loops through list from highest level to 0 inserting node
  for(i = height; i>= 0; i--){
    // insert at head, key is smaller than the first node
    if (key < list->head->forward[i]->data){
      snode *first_node = list->head->forward[i];  // first node of the list
      new_node = skip_create_node(key, word, first_node, i);
      list->head->forward[i] = new_node;

      //Sets new list->height if new RandHeight is larger than list->height
      if(height > list->height)
        list->height = height;

    }else{//insert in the middle or at the end
      snode *curr_node = list->head->forward[i]; // current node

      while ((curr_node->forward[i] != NULL) && (key > curr_node->forward[i]->data))
        curr_node = curr_node->forward[i];

      if (curr_node->forward[i] != NULL){ // in the middle, between curr_node and curr_node->next
        new_node = skip_create_node(key, word, curr_node->forward[i], i);
        curr_node->forward[i] = new_node;
      }else{ // at the end, insert after curr_node
        new_node = skip_create_node(key, word, NULL, i);
        curr_node->forward[i] = new_node;
      }
    }
  }
}

//Prints all values in each level of skip list
void skip_print(skiplist *list){
  for(int i = list->height+1; i >= 0; i--){
    printf("(S%d) ",i);
    snode *curr_node = list->head->forward[i];
    if(curr_node->data == MAX_KEY){
      printf("empty\n");
    }else{
      while(curr_node->data != MAX_KEY){
        printf("%04d:%s ",curr_node->data, curr_node->value);
        curr_node = curr_node->forward[i];
      }
      printf("\n");
    }
  }
}

// find key in list, which is in ascending order
// return highest level of key in skiplist
snode* skip_find(skiplist *list, int key){
  for(int i = list->height; i>= 0; i--){
    snode *curr_node = list->head->forward[i]; // current node
    //Loops through level i nodes until curr_node->data > key
    while(curr_node->data != MAX_KEY && key > curr_node->data){
      curr_node = curr_node->forward[i];
    }
    if(curr_node->data == key){   //Return match node
      return curr_node;
    }else{
      if(i == 0){   //If last level of list and no matches return NULL
        return NULL;
      }
    }
  }
}

//Function to find previous node of current key node
snode* skip_find_previous(skiplist *list, int key, int height){
  snode *curr_node = list->head->forward[height]; // current node

  while(curr_node->forward[height]->data != MAX_KEY && key > curr_node->forward[height]->data){
    curr_node = curr_node->forward[height];
  }
  if(curr_node->forward[height]->data == key){
    return curr_node;
  }else{
    return NULL;
  }
}

//Returns the node matching key at the current height of skip list
snode* skip_find_at_level(skiplist *list, int key, int height){
  snode *curr_node = list->head->forward[height]; //current node
  //Loops through level until finds matching key node at height
  while(curr_node->data != MAX_KEY && key > curr_node->data){
    curr_node = curr_node->forward[height];
  }
  if(curr_node->data == key){
    return curr_node;
  }else{
    return NULL;
  }
}

//Used to find largest key smaller than the one specified
//Only goes off of the very bottom level of skip list as all nodes are there
snode* skip_find_floor(skiplist *list, int key){
  snode *curr_node = list->head->forward[0]; //current node
  //Loops through level until finds floor of key
  while(curr_node->data != MAX_KEY && key >= curr_node->data && curr_node->forward[0]->data <= key){
    curr_node = curr_node->forward[0];
  }
  if(curr_node->data != MAX_KEY && curr_node){
    return curr_node;
  }else{
    return NULL;
  }
}

//Finds the smallest key larger than the one specified
//Uses bottom level of skiplist
snode* skip_find_ceiling(skiplist *list, int key){
  snode *curr_node = list->head->forward[0]; // current node
  while(curr_node->data != MAX_KEY && key >= curr_node->data && curr_node->forward[0]->data < key){
    curr_node = curr_node->forward[0];
  }
  curr_node = curr_node->forward[0];
  if(curr_node->data != MAX_KEY && curr_node){
    return curr_node;
  }else{
    return NULL;
  }
}

//Delete a node with key and maintain the ascending order
void skip_delete(skiplist *list, int key){
  //Checks to see if the key matches an existing node       
  snode *found_node = skip_find(list,key);
  snode *prev_node = NULL;

  //If key matches an existing node
  if (found_node != NULL){
    int height = found_node->n_height;
    //Loop from max level of node down to 0
    for(int i = height; i>=0; i--){
      prev_node = skip_find_previous(list,key,i);        //Finds previous node of key node at each level
      snode *curr_node = skip_find_at_level(list,key,i); //Finds matching key node at each level
      if (list->head->forward[i]->data == key){          //First node
	      list->head->forward[i] = curr_node->forward[i];
      }else if (curr_node->forward[i]->data == 1300){    //Last node
        prev_node->forward[i] = curr_node->forward[i];
      }else{ // middle, in between two nodes
        prev_node->forward[i] = curr_node->forward[i];
      }
      free(curr_node);
    }
    printf("success\n");
  }
  else //key was not found or list is empty
    printf("noDateError\n");
}