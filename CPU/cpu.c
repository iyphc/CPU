#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

const int number_of_registers = 4;

cpu cpu_construct() { 
  cpu * cpu_tmp = (cpu*)calloc(1, (sizeof(cpu)));
  ull * registers = calloc(4, sizeof(ull));
  cpu_tmp->stack = construct();
}

void push_reg(cpu * cpu, int type_of_register) { //0
  if (type_of_register >= 0 && type_of_register < number_of_registers) {
    cpu_push(cpu, cpu->registers[type_of_register]);
  }
}

void cpu_push(cpu * cpu, elem_t elem) { //1
  stack_push(cpu->stack, elem);
}

void pop_reg(cpu * cpu, int type_of_register) { //2
  if (type_of_register >= 0 && type_of_register < number_of_registers) {
    cpu->registers[type_of_register] = stack_pop(cpu->stack);
  }
}

//add === 3

ull mov_reg_reg(cpu * cpu, int first_reg, int second_reg) { //4
  if (first_reg >= number_of_registers || first_reg < 0) return 0;
  if (second_reg >= number_of_registers || second_reg < 0)
  cpu->registers[first_reg] = cpu->registers[second_reg];
}

//компиляция - перевод ассемблерного кода в исполняемый файл (.myexe)
//процессор - выполнение .myexe