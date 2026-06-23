#include "stack.h"
#include <stdio.h>

void initStack(Stack *s) { s->top = -1; }

int isEmpty(Stack *s) { return s->top == -1; }

int isFull(Stack *s) { return s->top == MAX_SIZE - 1; }

void push(Stack *s, int value) {
  if (isFull(s)) {
    printf("Stack Overflow\n");
    return;
  }

  s->data[++s->top] = value;
}

int pop(Stack *s) {
  if (isEmpty(s)) {
    printf("Stack Underflow\n");
    return -1;
  }

  return s->data[s->top--];
}

int peek(Stack *s) {
  if (isEmpty(s)) {
    printf("Stack is empty\n");
    return -1;
  }

  return s->data[s->top];
}

int size(Stack *s) { return s->top + 1; }

void display(Stack *s) {
  if (isEmpty(s)) {
    printf("Stack is empty\n");
    return;
  }

  for (int i = s->top; i >= 0; i--) {
    printf("%d\n", s->data[i]);
  }
  printf("\n");
}

int main(void) {

  Stack s;

  initStack(&s);

  push(&s, 110);
  push(&s, 120);
  push(&s, 130);

  printf("Size: %d\n\n", size(&s));

  display(&s);

  printf("Top: %d\n", peek(&s));

  printf("Popped: %d\n", pop(&s));
  printf("Popped: %d\n", pop(&s));

  printf("Top: %d\n", peek(&s));

  return 0;
}
