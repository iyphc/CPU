#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <assert.h>

typedef enum Exceptions {
  OK,
  size_problem,
  hash_problem  
} Exceptions;

typedef float elem_t;

typedef struct stack {
  int capacity;
  int size;
  int hash;
  Exceptions status;
  elem_t* arr;
} stack;


// const unsigned long long canary = 15000987876788768;
extern const unsigned long long canary;

stack* stack_construct();
void stack_push(stack* stack, elem_t elem);
elem_t stack_pop(stack* stack);
Exceptions verify(stack* stack);
void resize_up(stack* stack);
void resize_down(stack* stack);
int MurmurHash2 (stack* stack);
Exceptions hash_verify(stack* stack); //выключена

#endif