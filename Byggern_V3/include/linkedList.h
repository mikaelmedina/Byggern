#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdint.h>
#include <stdlib.h>

typedef enum {
	PLAY,
	SETTINGS,
	CREDITS,
	JOYCAL,
	DUMMY,
	PLAY_JOYSTICK,
	PLAY_SLIDER
} MENU_ENTRY;

typedef struct Node{
	struct Node* next;
	struct Node* prev;
	struct Node* parent;
	struct Node* child;
	MENU_ENTRY label;
	char data[10];
};


struct Node* linkedList_createNode(char* data);


#endif /* LINKEDLIST_H_ */