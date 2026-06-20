#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
  int data;
  struct Node *next;
} Node;

Node *createNode(int data);
void insertAtBeginning(Node **head, int data);
void insertAtEnd(Node **head, int data);
void insertAtPosition(Node **head, int data, int position);
void deleteFromBeginning(Node **head);
void deleteFromEnd(Node **head);
void deleteByValue(Node **head, int value);
int search(Node *head, int target);
void displayList(Node *head);
void freeList(Node **head);

void reverseList(Node **head);

#endif
