#include <stdio.h>
#include <stdlib.h>
#include "CPU/cpu.h"
//#include "stack/stack.h"

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
  while (i < count) {
    if ((int)buffer[i] == 0) {
      push_reg(mycpu, (int)buffer[i+1]);
      i+=2;
    }
    else if ((int)buffer[i] == 1) {
      cpu_push(mycpu, buffer[i+1]);
      i+=2;
    }
    else if ((int)buffer[i] == 2) {
      pop_reg(mycpu, (int)buffer[i+1]);
      i+=2;
    }
    else if ((int)buffer[i] == 3) {
      add(mycpu, (int)buffer[i+1], (int)buffer[i+2]);
      i+=3;
    }
    else if ((int)buffer[i] == 4) {
      mov_reg_reg(mycpu, (int)buffer[i+1], (int)buffer[i+2]);
      i+=3;
    }
    else {
      break;
    }
  }
  printf("ответ: %f\n", mycpu->registers[0]);
  return 0;
} //little endian