#ifndef DLINKED_LIST_H
#define DLINKED_LIST_H

typedef struct Node {
  char *data;
  struct Node *next;
  struct Node *prev;
} Node;

Node *createNode(const char *data);

void insertAtBeginning(Node **head, const char *data);
void insertAtEnd(Node **head, const char *data);
void insertAtPosition(Node **head, const char *data, int position);

void deleteFromBeginning(Node **head);
void deleteFromEnd(Node **head);
void deleteByValue(Node **head, const char *value);

int search(Node *head, const char *value);

void displayList(Node *head);
void displayReverseList(Node *head);

void freeList(Node **head);
void reverseList(Node **head);

#endif
