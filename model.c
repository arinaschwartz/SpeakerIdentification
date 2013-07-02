#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<stdbool.h>
#include "model.h"

Node mkNode(char* key, int value){
	Node new_node = malloc(sizeof(struct node_s));
	new_node->key = strdup(key);
	new_node->value = value;
	new_node->next = NULL;
	return new_node;
}

void freeNode(Node n){
	free(n->key);
	free(n);
}

void printNode(Node n){
        printf("%s\t%d\n", n->key, n->value);
}

List mkList(){
	List new_list = malloc(sizeof(struct linkedlist_s));
	new_list->head = NULL;
	return new_list;
}

void freeList(List l){
	Node n = l->head;
	while (n != NULL){
		Node next = n->next;
		freeNode(n);
		n = next;
	}
	free(l);
}

void printList(List l){
        Node n = l->head;
        while (n != NULL){
                printNode(n);
                n = n->next;
        }
}

/* Note: as it stands now, new elements are added
 * to the top of the list, not the bottom
 */
void set(List l, char* key, int value){
	Node n = l->head;
	bool notFound = true;
	while (n != NULL){
		if (strcmp(n->key,key) == 0){
			n->value = value;
			notFound = false;
		}
		n = n->next;
	}
	if (notFound){
		Node new_node = mkNode(key,value);
		if (l->head == NULL) //list was originally empty
			l->head = new_node;
		else{
			new_node->next = l->head;
			l->head = new_node;
		}
	}
}

int get(List l, char* key){
	Node n = l->head;
	while (n != NULL){
		char* thisKey = n->key;
		if (strcmp(thisKey,key) == 0)
			return n->value;
		n = n->next;
	}
	//if key not in list, return zero
	return 0;
}

HashTable mkHashTable(int s){
	HashTable new_table = malloc(sizeof(struct hash_table_s));
	new_table->alphabetSize = s;
	int i;
	for (i=0; i < NUMBUCKETS; i++)
		new_table->buckets[i] = mkList();
	return new_table;
}

void freeHashTable(HashTable h){
	int i;
	for(i=0; i < NUMBUCKETS; i++)
		freeList(h->buckets[i]);
	free(h);
}

void printHashTable(HashTable h){
	int i;
	for (i=0; i < NUMBUCKETS; i++){
		printf("Bucket %d:\n", i+1);
		printList(h->buckets[i]);
		printf("\n");
	}
}

int hash(char* s){
	int h=0;
	assert( s != NULL);
	while (*s != '\0'){
		h = (h*31 + *s) % NUMBUCKETS; //31 was suggested prime number
		s++;
	}
	return h;
}

void setKey(HashTable h, char* key, int value){
	int index = hash(key);
	set(h->buckets[index],key,value);
}

int getCount(HashTable h, char* key){
	int index = hash(key);
	return get(h->buckets[index],key);
}
