#ifndef LSTACK_H
#define LSTACK_H

typedef struct Node {
  int data;
  struct Node *next;
} Node;

typedef struct {
  Node *top;
} Stack;

void initStack(Stack *s);

int isEmpty(Stack *s);

void push(Stack *s, int value);
int pop(Stack *s);
int peek(Stack *s);

void display(Stack *s);

#endif
