#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/**  
  * @file stack.c
  * @brief Contains funstions that mimic ctack manipulation
*/

/**
  * @brief Constructs a stack 
  * @return Created stack
*/

const unsigned long long canary = 15000987876788768;

stack* stack_construct() {
  stack* stack = (struct stack*)calloc(1, sizeof(struct stack));
  stack->capacity = 4; // не работает при capacity < 4
  stack->size = -1;
  stack->status = OK;
  elem_t* arr_tmp = (elem_t*)calloc(stack->capacity+1, sizeof(elem_t));
  arr_tmp[stack->size+1] = (elem_t)canary;
  (stack->arr) = arr_tmp;
  (stack->hash) = 0;
  return stack;
}

/**
  * @brief Adds an element to the start of the array
  * @param stack The stack itself to which the element is being added
  * @param elem The element which is added to the stack
*/

void stack_push(stack* stack, elem_t elem) {
  if (verify(stack) != OK) {
    return;
  }
  stack->size++;
  stack->arr[stack->size] = elem;
  stack->arr[stack->size+1] = (elem_t)canary;
  resize_up(stack);
  stack->hash = MurmurHash2(stack);
  if (verify(stack) != OK) {
    return;
  }
}

/**
  * @brief Removes the element from the end of a stack
  * @param stack The stack itself to which the element is being removed
  * @return Element which is removed from the end of a stack
*/

elem_t stack_pop(stack* stack) {
  if (verify(stack) != OK) {
    return (elem_t)-10e7;
  }
  if(stack->size >= 0) {
    elem_t temp = stack->arr[stack->size];
    stack->arr[stack->size] = (elem_t)canary;
    stack->size--;
    stack->hash = MurmurHash2(stack);
    return temp;
  }
  printf("\n\n");
  if (verify(stack) != OK) {
    return (elem_t)-10e7;
  }
  return (elem_t)-10e7;
} 

/**
  * @brief Checks the stack for a canary after the last element
  * @param stack The stack which is being checked
  * @return Stack status
*/

Exceptions verify(stack* stack) {
  if(stack->arr[stack->size+1] != (elem_t)canary) {
    stack->status = size_problem;
    return size_problem;
  }
  stack->status = OK;
  if (hash_verify(stack) == OK) return OK;
  else return hash_problem;
}

/**
  * @brief Checks the hash of a stack 
  * @param stack The stack which is being checked
  * @return Stack status
*/

Exceptions hash_verify(stack* stack) {
  if(stack->hash != MurmurHash2(stack)) {
    stack->status = hash_problem;
    return hash_problem;
  }
  stack->status = OK;
  return OK;
}

/**
  * @brief Increases stack capacity
  * @param stack The stack which capacity is being increased
*/

void resize_up(stack* stack) {
  if(stack->size+1 == stack->capacity) {
    stack->arr = realloc(stack->arr, sizeof(elem_t)*(stack->capacity)*2+1);
    stack->capacity*=2;
  }
  stack->hash = MurmurHash2(stack);
}

/**
  * @brief Reduces stack capacity
  * @param stack The stack which capacity is being reduced
*/

void resize_down(stack* stack) {
  if(stack->capacity > 4) {
    stack->arr = realloc(stack->arr, sizeof(elem_t)*((stack->capacity)/2)+1);
    stack->capacity/=2;
    stack->arr[stack->capacity] = (elem_t)canary;
    stack->size = stack->capacity - 1;
    stack->hash = MurmurHash2(stack);
  }
}

/**
  * @brief Reduces stack capacity
  * @param stack The stack from which the hash is created
  * @return The hash of the stack
*/

int MurmurHash2 (stack* stack)
{
  /*
  const unsigned int m = 0x5bd1e995;
  const unsigned int seed = 0;
  const int r = 24;
  unsigned int len = stack->size+1;

  unsigned int h = seed ^ len;

  elem_t * data = stack->arr;
  unsigned int k = 0;

  while (len >= 4)
  {
      k  = data[0];
      k |= data[1] << 8;
      k |= data[2] << 16;
      k |= data[3] << 24;

      k *= m;
      k ^= k >> r;
      k *= m;

      h *= m;
      h ^= k;

      data += 4;
      len -= 4;
  }

  switch (len)
  {
    case 3:
      h ^= data[2] << 16;
    case 2:
      h ^= data[1] << 8;
    case 1:
      h ^= data[0];
      h *= m;
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  
  return h;
  */
  return 0;
}