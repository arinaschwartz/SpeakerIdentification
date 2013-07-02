/* Andrew Christoforakis & Arin Scwartz
 *
 * Header file for linked hash table structure
 */ 

#ifndef LINKED_LIST
#define LINKED_LIST

#define NUMBUCKETS 31

typedef struct node_s* Node;

struct node_s{
	char *key;
	int value;
	Node next;
};

typedef struct linkedlist_s* List;

struct linkedlist_s{
	Node head;
};

typedef struct hash_table_s* HashTable;

struct hash_table_s{
	int alphabetSize;
	List buckets[NUMBUCKETS];
};

//make an empty node
Node mkNode(char* key, int value);

//free individual node
void freeNode(Node n);

//print contents of node for testing purposes
void printNode(Node n);

//make an empty list
List mkList();

//recursively free list of nodes
void freeList(List l);

//print contents of list for testing purposes
void printList(List l);

//set a key, value pair in list
void set(List l, char* key, int value);

//get the value corresponding to a given key
int get(List l, char* key);

//make an empty has table
HashTable mkHashTable(int s);

//free hash table after use
void freeHashTable(HashTable h);

//print contents of hash table for testing purposes
void printHashTable(HashTable h);

//hash function
int hash(char* s);

//enter new word in hash table
void setKey(HashTable h, char* key, int value);

//retrieve value from hash table
int getCount(HashTable h, char* key);

#endif
