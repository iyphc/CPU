#include <stdio.h>
#include <stdlib.h>
#include "../stack/stack.h"

typedef unsigned long long ull;
typedef float elem_t;
const int number_of_registers;

typedef struct cpu {
	elem_t * registers; //5-й -- fx (значение cmp)
	stack* stack;
} cpu;

cpu * cpu_construct();
void push_reg(cpu * cpu, int type_of_register);
void cpu_push(cpu * cpu, elem_t elem);
void pop_reg(cpu * cpu, int type_of_register);
void add(cpu * cpu, int first_reg, int second_reg);
void mov_reg_reg(cpu * cpu, int first_reg, int second_reg);
