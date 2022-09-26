/*
  Author: Felipe Azevedo
  Email: fazevedo2017@my.fit.edu
  Course: CSE2010
  Section: 02

  Description of this file: Monsters and humans game where the player chooses his path towards 
  the end point while the monsters try to intercept the player
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

//Global variables
int col, row, size, player_pos;

struct AdjListNode{
    char dest;    //Char of node(#,*,A,0,' ')
    int pos;      //Position in graph
    int weight;   //Weight(1)
    struct AdjListNode* next;
    struct AdjListNode* prev;
};

struct AdjList{
    struct AdjListNode *head;
};

struct Graph{
    int V;        //num of veritces in graph
    struct AdjList* array;  //array of heads
};

struct Monster{
  char letter;    //Monster letter
  int pos;        //Monster pos in graph
};

//Creates new node in graph
struct AdjListNode* newAdjListNode(char dest, int pos, int weight){
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->pos = pos;
    newNode->weight = weight;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

//Creates the graph with size V(col*row)
struct Graph* createGraph(int V){
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

     // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

//Adds edges to neighbors of a node in the graph
void addEdge(struct Graph* graph, int src, int dest, char src_char, char dest_char, int weight){
    struct AdjListNode* newNode = newAdjListNode(dest_char, dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = newAdjListNode(src_char, src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

//Calculates the position with height and depth of node
int calcPos(int i, int k){
  int pos = i*col + k;
  return pos;
}

struct MinHeapNode{
    int  v;
    int dist;
    struct MinHeapNode *parent;
};

struct MinHeap{
    int size;      //Number of heap nodes present currently
    int capacity;  //Capacity of min heap
    int *pos;
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v, int dist){
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

struct MinHeap* createMinHeap(int capacity){
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx){
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;

    if (smallest != idx){
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap* minHeap){
    return minHeap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap){
    if (isEmpty(minHeap))
        return NULL;

    struct MinHeapNode* root = minHeap->array[0];

    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(struct MinHeap* minHeap, int v, int dist){
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist){
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct MinHeap *minHeap, int v){
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

//Prints shortest path length to player
void printLength(int dist[], int n,int des){
    printf(" %d ",dist[des]);
}

//Prints the path of the monster to player
void print_path(struct AdjListNode *nod){
    //Recurse until src node
    if (nod->prev) {
        print_path(nod->prev);
    }
    int posi = nod->pos;
    int height = posi % row;
    int depth = posi / col;
    printf("(%d, %d) ", height, depth);
}

//Calculates shortest path distance to player
void dijkstra(struct Graph* graph, int src, int des){
    int V = graph->V;//Get the number of vertices in graph
    int dist[V];

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all vertices. dist value of all vertices
    for (int v = 0; v < V; ++v){
        //parent[0]= -1;
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    //src dist 0 so it goes first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    minHeap->size = V;

    struct MinHeapNode *prev = NULL;
    while (!isEmpty(minHeap)){
        // Extract the vertex with minimum distance value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        minHeapNode->parent = prev;
        prev = minHeapNode;

        int u = minHeapNode->v;
        //Go through all neighbors of u
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL){
            int v = pCrawl->pos;
            //If new dist is less than previous and node isnt a #
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v] && pCrawl->dest != '#'){
                dist[v] = dist[u] + pCrawl->weight;
                graph->array[v-1].head->prev = pCrawl;
                // update distance value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
    // print the calculated shortest distances
    printLength(dist, src,des);
    print_path(graph->array[des].head);
}

//Prints the graph in a 2d display
void print_graph(struct Graph* graph, int col, int row){
  printf(" ");
  for(int j = 0; j<col; j++){
    printf("%d", j);
  }
  int i = 0;
  int k = 0;
  while(i<graph->V){
    if(i % col == 0){
      //prints newline if i divisible by col
      printf("\n%d",k);
      k++;
    }
    printf("%c", graph->array[i].head->dest);
    i++;
  }
}

//Move player in the graph
void movePlayer(struct Graph* graph, int pos){
  char move;
  int height = pos % row;
  int depth = pos / col;

  printf("\nPlayer 0, please enter your move [u(p), d(own), l(elf), or r(ight)]: ");
  scanf("%c", &move);
  if(move == '\n'){
    scanf("%c", &move);
  }
  //Checks to see if valid move
  if(move == 'u' && height > 0 && graph->array[pos-col].head->dest == ' '){
    //Move up
    graph->array[pos].head->dest = ' ';
    graph->array[pos-row].head->dest = '0';
    player_pos = pos-row;
    print_graph(graph,col,row);
  }else if(move == 'd' && height < row-1 && graph->array[pos+col].head->dest == ' '){
    //Move down
    graph->array[pos].head->dest = ' ';
    graph->array[pos+row].head->dest = '0';
    player_pos = pos+row;
    print_graph(graph,col,row);
  }else if(move == 'l' && depth > 0 && graph->array[pos-1].head->dest == ' '){
    //Move left
    graph->array[pos].head->dest = ' ';
    graph->array[pos-1].head->dest = '0';
    player_pos = pos-1;
    print_graph(graph,col,row);
  }else if(move == 'r' && depth < col-1 && graph->array[pos+1].head->dest == ' '){
    //Move right
    graph->array[pos].head->dest = ' ';
    graph->array[pos+1].head->dest = '0';
    player_pos = pos+1;
    print_graph(graph,col,row);
  }else{
    //else ask for input again
    movePlayer(graph,pos);
  }
}

//Sort monsters alphabetically
void sortAlpha(struct Monster monsters[],int size){
  char temp;
  for(int i = 0; i<size; i++){
    for(int j = i+1; j< size; j++){
      if(monsters[i].letter > monsters[j].letter){
        temp = monsters[i].letter;
        monsters[i].letter = monsters[j].letter;
        monsters[j].letter = temp;
      }
    }
  }
}

//Find shortest path to player for each monster
void monsterMove(struct Graph* graph, struct Monster monsters[], int player_pos,int size){
  int i = 0;
  sortAlpha(monsters,size);
  for(int i = 0; i<size; i++){
    printf("\nMonster %c: ", monsters[i].letter);
    dijkstra(graph,monsters[i].pos,player_pos);
  }
}

int main(int argc, char* argv[]){
  FILE *input;
  char *filename;
  char line[100];
  char array_of_lines[10][10];
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
      if(total_lines == 0){
        row = line[0] - '0';
        col = line[2] - '0';
        size = row * col;
        printf("row: %d col: %d\n",row,col);
        total_lines++;
      }else{
        strcpy(array_of_lines[total_lines-1], line);    
        total_lines++;
      }              
    }
    total_lines = total_lines--;
  }else{
    printf("File could not be read\n");
  }

  //Creates graph and array of monster structs
  struct Graph* graph = createGraph(size);
  struct Monster monsters[26];

  //Reads in all the nodes into the graph
  int dest,src;
  char src_char;
  int num_mons = 0;

  for(int i=0; i<total_lines-1; i++){
    for(int k = 0; k< col; k++){
      src = calcPos(i,k);
      src_char = array_of_lines[i][k];
      //If current line is a player, set as playerpos
      if(array_of_lines[i][k] == '0'){
        player_pos = src;
      }
      //If current node is a monster add it to array of monsters
      if(isupper(array_of_lines[i][k])){
        monsters[num_mons].letter = array_of_lines[i][k];
        monsters[num_mons].pos = src;
        num_mons++;
      }
      //If node has a valid upper neighbor
      if(i > 0){
        dest = calcPos(i-1,k);
        addEdge(graph,src,dest,array_of_lines[i-1][k],src_char,1);
      }
      //If node has a valid down neighbor
      if(i < total_lines-2){
        dest = calcPos(i+1,k);
        addEdge(graph,src,dest,array_of_lines[i+1][k],src_char,1);
      }
      //If node has a valid left neighbor
      if(k > 0){
        dest = calcPos(i,k-1);
        addEdge(graph,src,dest,array_of_lines[i][k-1],src_char,1);
      }
      //If node has a valid right neighbor
      if(k < col-1){
        dest = calcPos(i,k+1);
        addEdge(graph,src,dest,array_of_lines[i][k+1],src_char,1);
      }
    }
  }

  //First player move and monster move
  print_graph(graph,col,row);
  movePlayer(graph,player_pos);
  monsterMove(graph, monsters,player_pos,num_mons);
}