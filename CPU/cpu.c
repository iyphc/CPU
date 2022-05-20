#include "cpu.h"

const int number_of_registers = 5;

int main(const int argc, const char* argv[]) {
  FILE* input = fopen(argv[1], "rb");
  fseek(input, 0, SEEK_END);
	size_t count = ftell(input);
	fseek(input, 0, SEEK_SET);
  elem_t* buffer = (elem_t*)malloc(count);
  count /= sizeof(elem_t);
  fread(buffer, sizeof(elem_t), count, input);
  printf("Буфер (%ld элементов):\n", count);
  for(int i = 0; i < count; i++) {
    printf("%.2f ", buffer[i]);
  }
  printf("\n\n");
  cpu * mycpu = cpu_construct();
  int i = 0;
  int k = 0;
  while (i < count) {
    if ((int)buffer[i] == 0) {
      printf("PUSH_REG COMPLETED\n");
      push_reg(mycpu, (int)buffer[i+1]);
      i+=2;
    }
    else if ((int)buffer[i] == 1) {
      printf("PUSH_CPU COMPLETED\n");
      cpu_push(mycpu, buffer[i+1]);
      i+=2;
    }
    else if ((int)buffer[i] == 2) {
      printf("POP_REG COMPLETED\n");
      pop_reg(mycpu, (int)buffer[i+1]);
      i+=2;
    }
    else if ((int)buffer[i] == 3) {
      printf("ADD COMPLETED\n");
      add(mycpu, (int)buffer[i+1], (int)buffer[i+2]);
      i+=3;
    }
    else if ((int)buffer[i] == 4) {
      printf("MOV COMPLETED\n");
      printf("%d %d\n", (int)buffer[i+1], (int)buffer[i+2]);
      mov_reg_reg(mycpu, (int)buffer[i+1], (int)buffer[i+2]);
      i+=3;
    }
    else if ((int)buffer[i] == 7) {
      printf("MOVED TO MARK\n");
      printf("i = %d\n", buffer[i+1]);
      i = buffer[i+1];
      
    }
    else {
      printf("EMERGENCY SHUTDOWN\n");
      printf("ax = %.2f\n", mycpu->registers[0]);
      printf("bx = %.2f\n", mycpu->registers[1]);
      printf("cx = %.2f\n", mycpu->registers[2]);
      printf("dx = %.2f\n", mycpu->registers[3]);
      printf("----------------------\n");
      break;
    }
    k++;
    if (k == 15) break; 
    printf("ax = %.2f\n", mycpu->registers[0]);
    printf("bx = %.2f\n", mycpu->registers[1]);
    printf("cx = %.2f\n", mycpu->registers[2]);
    printf("dx = %.2f\n", mycpu->registers[3]);
    printf("----------------------\n");
  }
  return 0;
}

cpu * cpu_construct() { 
  cpu * cpu_tmp = (cpu*)calloc(1, (sizeof(cpu)));
  elem_t * registers = (elem_t *)calloc(4, sizeof(elem_t));
  cpu_tmp->registers = registers;
  cpu_tmp->stack = stack_construct();
  return (cpu*)cpu_tmp;
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

void add(cpu * cpu, int first_reg, int second_reg) { //3
  cpu->registers[first_reg] += cpu->registers[second_reg];
}

void mov_reg_reg(cpu * cpu, int first_reg, int second_reg) { //4
  if (first_reg >= number_of_registers || first_reg < 0) {
    printf("PROBLEM: REGISTERS ARE NOT FOUND\n");
    return;
  }
  cpu->registers[first_reg] = cpu->registers[second_reg];
}

//little endian
//компиляция - перевод ассемблерного кода в исполняемый файл (.myexe)
//процессор - выполнение .myexe