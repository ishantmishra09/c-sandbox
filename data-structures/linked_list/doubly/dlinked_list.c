#include "dlinked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *createNode(const char *data) {
  Node *node = malloc(sizeof(Node));
  if (node == NULL)
    return NULL;

  node->data = malloc(strlen(data) + 1);
  if (node->data == NULL) {
    free(node);
    return NULL;
  }

  strcpy(node->data, data);
  node->next = NULL;
  node->prev = NULL;

  return node;
}

void insertAtBeginning(Node **head, const char *data) {
  Node *node = createNode(data);
  if (node == NULL)
    return;

  node->next = *head;

  if (*head != NULL)
    (*head)->prev = node;

  *head = node;
}

void insertAtEnd(Node **head, const char *data) {
  Node *node = createNode(data);
  if (node == NULL)
    return;

  if (*head == NULL) {
    *head = node;
    return;
  }

  Node *temp = *head;

  while (temp->next != NULL)
    temp = temp->next;

  temp->next = node;
  node->prev = temp;
}

void insertAtPosition(Node **head, const char *data, int position) {
  if (position < 0) {
    printf("Invalid position.\n");
    return;
  }

  if (position == 0) {
    insertAtBeginning(head, data);
    return;
  }

  Node *node = createNode(data);
  if (node == NULL)
    return;

  Node *temp = *head;

  for (int i = 0; temp != NULL && i < position - 1; i++)
    temp = temp->next;

  if (temp == NULL) {
    printf("Position out of bounds.\n");
    free(node->data);
    free(node);
    return;
  }

  node->next = temp->next;
  node->prev = temp;

  if (temp->next != NULL)
    temp->next->prev = node;

  temp->next = node;
}

void displayList(Node *head) {
  if (head == NULL) {
    printf("List is empty\n");
    return;
  }

  Node *temp = head;

  while (temp != NULL) {
    printf("%s <-> ", temp->data);
    temp = temp->next;
  }

  printf("NULL\n");
}

void displayReverseList(Node *head) {
  if (head == NULL) {
    printf("List is empty\n");
    return;
  }

  Node *temp = head;
  while (temp->next != NULL)
    temp = temp->next;

  printf("NULL");
  while (temp != NULL) {
    printf(" <-> %s", temp->data);
    temp = temp->prev;
  }
  printf("\n");
}

void freeList(Node **head) {
  if (head == NULL)
    return;

  Node *temp;

  while (*head != NULL) {
    temp = *head;
    *head = (*head)->next;

    free(temp->data);
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

  if (*head != NULL)
    (*head)->prev = NULL;

  free(temp->data);
  free(temp);
}

void deleteFromEnd(Node **head) {
  if (*head == NULL) {
    printf("List is empty.\n");
    return;
  }

  if ((*head)->next == NULL) {
    free((*head)->data);
    free(*head);
    *head = NULL;
    return;
  }

  Node *temp = *head;

  while (temp->next != NULL)
    temp = temp->next;

  temp->prev->next = NULL;

  free(temp->data);
  free(temp);
}

void deleteByValue(Node **head, const char *value) {
  if (*head == NULL)
    return;

  if (strcmp((*head)->data, value) == 0) {
    deleteFromBeginning(head);
    return;
  }

  Node *temp = *head;

  while (temp != NULL && strcmp(temp->data, value) != 0)
    temp = temp->next;

  if (temp->next == NULL) {
    printf("Value \"%s\" not found.\n", value);
    return;
  }

  if (temp->prev != NULL)
    temp->prev->next = temp->next;

  if (temp != NULL)
    temp->next->prev = temp->prev;

  free(temp->data);
  free(temp);
}

int search(Node *head, const char *target) {
  Node *temp = head;

  while (temp != NULL) {
    if (strcmp(temp->data, target) == 0)
      return 1;

    temp = temp->next;
  }

  return -1;
}

void reverseList(Node **head) {
  Node *current = *head;
  Node *temp = NULL;

  while (current != NULL) {
    temp = current->prev;
    current->prev = current->next;
    current->next = temp;

    current = current->prev;
  }

  if (temp != NULL)
    *head = temp->prev;
}

int main(void) {
  Node *head = NULL;

  insertAtBeginning(&head, "A");
  insertAtEnd(&head, "C");
  insertAtPosition(&head, "B", 1);

  printf("Initial list:\n");
  displayList(head);
  displayReverseList(head);

  printf("\nSearching:\n");
  printf("B: %s\n", search(head, "B") == 1 ? "Found" : "Not Found");

  printf("D: %s\n", search(head, "D") == 1 ? "Found" : "Not Found");

  printf("\nDelete from beginning:\n");
  deleteFromBeginning(&head);
  displayList(head);
  displayReverseList(head);

  printf("\nDelete from end:\n");
  deleteFromEnd(&head);
  displayList(head);
  displayReverseList(head);

  printf("\nInsert more items:\n");
  insertAtEnd(&head, "E");
  insertAtEnd(&head, "F");
  displayList(head);
  displayReverseList(head);

  printf("\nDelete by value (F):\n");
  deleteByValue(&head, "F");
  displayList(head);
  displayReverseList(head);

  printf("\nReverse list:\n");
  reverseList(&head);
  displayList(head);
  displayReverseList(head);

  freeList(&head);

  return 0;
}
