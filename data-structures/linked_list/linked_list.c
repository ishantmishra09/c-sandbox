#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  void *data;
  struct Node *next;
} Node;

Node *createNode(void *data) {

  Node *node = (Node *)malloc(sizeof(*node));
  if (node == NULL) {

    perror("createNode: malloc");
    exit(EXIT_FAILURE);
  }

  node->data = data;
  node->next = NULL;

  return node;
}

void addToBeginning(Node **head, void *data) {

  if (head == NULL)
    return;

  Node *node = createNode(data);
  node->next = *head;
  *head = node;
}

void addToEnd(Node **head, void *data) {

  if (head == NULL)
    return;

  Node *node = createNode(data);

  if (*head == NULL) {

    *head = node;
    return;
  }

  Node *current = *head;
  while (current->next != NULL)
    current = current->next;

  current->next = node;
}

void freeList(Node **head, void (*freeData)(void *)) {

  if (head == NULL)
    return;

  Node *current = *head;
  while (current != NULL) {

    Node *next = current->next;

    if (freeData != NULL)
      freeData(current->data);

    free(current);
    current = next;
  }

  *head = NULL;
}

void printIntList(const Node *head) {

  while (head != NULL) {

    printf("%d->", *(int *)head->data);
    head = head->next;
  }
  printf("NULL\n");
}

int main() {

  Node *head = NULL;

  int *x = malloc(sizeof *x);
  int *y = malloc(sizeof *y);
  int *z = malloc(sizeof *z);

  if (x == NULL || y == NULL || z == NULL) {

    perror("malloc");
    return EXIT_FAILURE;
  }

  *x = 50;
  *y = 70;
  *z = 90;

  addToBeginning(&head, x);
  addToEnd(&head, y);
  addToEnd(&head, z);

  printf("List: ");
  printIntList(head);

  freeList(&head, free);

  return EXIT_SUCCESS;
}
