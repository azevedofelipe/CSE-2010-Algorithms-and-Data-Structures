#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

// a node in skiplist
typedef struct snode
{
  int             data;
  char			value[100];
  int 			n_height;
  struct snode **forward;
} snode;


// skiplist with only head
// Using a struct allows head to be changed by passing address of skiplist,
// instead of passing address of head (double pointer),
//    which might be more confusing
// This also allows more fields to be added later if desired, e.g. tail and length
typedef struct skiplist
{
  snode   *head;
  int height;
} skiplist;


void skip_insert(skiplist *list,  int key, char *word);
void skip_delete(skiplist *list,  int key);
snode* skip_find(skiplist *list, int key);
void skip_print(skiplist *list);
skiplist *skiplist_init(skiplist *list);
snode* skip_find_floor(skiplist *list, int key);
snode* skip_find_ceiling(skiplist *list, int key);
snode* skip_find_previous(skiplist *list, int key, int height);
snode* skip_find_at_level(skiplist *list, int key, int height);

#endif




