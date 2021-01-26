#include "linkedList.h"

struct Node* linkedList_createNode(char* data) {
	struct Node* temp = (struct Node *) malloc(sizeof(struct Node)*2);
	for(int i = 0; i < strlen(data); i++) {
		temp->data[i] = data[i];
	}
	temp->next = NULL;
	temp->prev = NULL;
	temp->parent = NULL;
	temp->child = NULL;
	temp->label = 0;
	
	return temp;
}