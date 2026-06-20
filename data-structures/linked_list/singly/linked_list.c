#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node *createNode(int data) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL)
    return NULL;

  node->data = data;
  node->next = NULL;
  return node;
}

void insertAtBeginning(Node **head, int data) {
  Node *node = createNode(data);
  node->next = *head;
  *head = node;
}

void insertAtEnd(Node **head, int data) {
  Node *node = createNode(data);

  if (*head == NULL) {
    *head = node;
    return;
  }

  Node *temp = *head;
  while (temp->next != NULL)
    temp = temp->next;

  temp->next = node;
}

void insertAtPosition(Node **head, int data, int position) {
  if (position == 0) {
    insertAtBeginning(head, data);
    return;
  }

  Node *node = createNode(data);
  Node *temp = *head;

  for (int i = 0; temp != NULL && i < position - 1; i++)
    temp = temp->next;

  if (temp == NULL) {
    fprintf(stderr, "Position out of bounds.\n");
    free(node);
    return;
  }

  node->next = temp->next;
  temp->next = node;
}

void displayList(Node *head) {
  if (head == NULL) {
    printf("List is empty\n");
    return;
  }

  Node *temp = head;
  while (temp != NULL) {
    printf("%d -> ", temp->data);
    temp = temp->next;
  }
  printf("NULL\n");
}

void freeList(Node **head) {
  if (head == NULL)
    return;

  Node *temp;
  while (*head != NULL) {
    temp = *head;
    *head = (*head)->next;
    free(temp);
  }
}

void deleteFromBeginning(Node **head) {
  if (*head == NULL) {
    printf("List is empty.\n");
    return;
  }

  Node *temp = *head;
  *head = (*head)->next;
  free(temp);
}

void deleteFromEnd(Node **head) {
  if (*head == NULL) {
    printf("List is empty.\n");
    return;
  }

  if ((*head)->next == NULL) {
    free(*head);
    *head = NULL;
    return;
  }

  Node *temp = *head;
  while (temp->next->next != NULL)
    temp = temp->next;

  free(temp->next);
  temp->next = NULL;
}

void deleteByValue(Node **head, int value) {
  if (*head == NULL)
    return;

  if ((*head)->data == value) {
    Node *temp = *head;
    *head = (*head)->next;
    free(temp);
    return;
  }

  Node *temp = *head;
  while (temp->next != NULL && temp->next->data != value)
    temp = temp->next;

  if (temp->next == NULL) {
    printf("Value %d not found in the list.\n", value);
    return;
  }

  Node *nodeToDelete = temp->next;
  temp->next = temp->next->next;
  free(nodeToDelete);
}

int search(Node *head, int target) {
  Node *temp = head;
  while (temp != NULL) {
    if (temp->data == target)
      return 1;
    temp = temp->next;
  }

  return -1;
}

void reverseList(Node **head) {
  Node *prev = NULL;
  Node *current = *head;
  Node *next = NULL;

  while (current != NULL) {
    next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }
  *head = prev;
}

int main() {
  Node *head = NULL;

  insertAtBeginning(&head, 10);
  insertAtEnd(&head, 30);
  insertAtPosition(&head, 20, 1);
  displayList(head);

  int target_1 = 30;
  int target_2 = 90;
  printf("Searching for %d: %s\n", target_1,
         search(head, target_1) != -1 ? "Found" : "Not Found");
  printf("Searching for %d: %s\n", target_2,
         search(head, target_2) != -1 ? "Found" : "Not Found");

  deleteFromBeginning(&head);
  displayList(head);

  deleteFromEnd(&head);
  displayList(head);

  insertAtEnd(&head, 40);
  insertAtEnd(&head, 50);
  displayList(head);

  deleteByValue(&head, 40);
  displayList(head);

  reverseList(&head);
  displayList(head);

  freeList(&head);
  return 0;
}
