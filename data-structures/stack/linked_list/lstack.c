#include "lstack.h"

#include <stdio.h>
#include <stdlib.h>

void initStack(Stack *s) { s->top = NULL; }

int isEmpty(Stack *s) { return s->top == NULL; }

void push(Stack *s, int value) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL)
    return;

  node->data = value;
  node->next = s->top;
  s->top = node;
}

int pop(Stack *s) {
  if (isEmpty(s)) {
    printf("Stack Underflow\n");
    return -1;
  }

  Node *temp = s->top;
  int value = temp->data;

  s->top = temp->next;
  free(temp);

  return value;
}

int peek(Stack *s) {
  if (isEmpty(s)) {
    printf("Stack is empty\n");
    return -1;
  }

  return s->top->data;
}

void display(Stack *s) {
  if (isEmpty(s)) {
    printf("Stack is empty\n");
    return;
  }

  Node *temp = s->top;
  while (temp != NULL) {
    printf("%d ", temp->data);
    temp = temp->next;
  }
  printf("\n");
}

int main(void) {
  Stack s;

  initStack(&s);

  push(&s, 110);
  push(&s, 120);
  push(&s, 130);

  display(&s);

  printf("Top: %d\n", peek(&s));
  printf("Popped: %d\n", pop(&s));

  display(&s);

  return 0;
}
