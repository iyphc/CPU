#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../parser/parser.h"

typedef float elem_t;
int find_reg(char*);
const int MAX_BUF_SIZE = 1024;

//выкидывает ошибку на buildString, output_buffer дальше пытается в нулевую память что-то закинуть

int main(const int argc, const char* argv[]) {
  //Translates text into an array of strings
  FILE* asm_file = fopen(argv[1], "r");
  int file_byte_size = count_symbols(asm_file) + 1;
  char* file_buffer = fileToString(asm_file, file_byte_size); //работает
  int lines_count = (int)count_lines(file_buffer, file_byte_size);
	int actual_lines_count = formatString(file_buffer, file_byte_size);
	char** strings = (char**)calloc(actual_lines_count, sizeof(char*));
	strings = buildString(file_buffer, strings, file_byte_size);
  //Makes comands buffer
	fclose(asm_file);
  elem_t* output_buffer = (elem_t*)calloc((MAX_BUF_SIZE), sizeof(elem_t));//ошибка здесь
  for(int i = 0; i < MAX_BUF_SIZE; i++) {
    output_buffer[i] = -10e3;
  }
  int k = 0;
  for(int i = 0; i < actual_lines_count; i++) {
    if (strstr(strings[i], "push") != NULL) {
      if (find_reg(strings[i]) == -1) {
        output_buffer[k] = 1;
        k++;
        output_buffer[k] = atof(strings[i]+4);
        k++;
      }
      else {
        output_buffer[k] = 0;
        k++;
        output_buffer[k] = find_reg(strings[i]);
        k++;
      }
    }
    else if (strstr(strings[i], "pop") != NULL) {
      output_buffer[k] = 2;
      k++;
      output_buffer[k] = find_reg(strings[i]);
      k++;
    }
    else if (strstr(strings[i], "add") != NULL) {
      output_buffer[k] = 3;
      k++;
      output_buffer[k] = find_reg(strings[i]+4);
      k++;
      output_buffer[k] = find_reg(strings[i]+7);
      k++;
    }
    else if (strstr(strings[i], "mov") != NULL) {
      //printf("strlen = %d\n", strlen(strings[i]));
      output_buffer[k] = 4;
      k++;
      output_buffer[k] = find_reg(strings[i]+4);
      k++;
      output_buffer[k] = find_reg(strings[i]+7);
      k++;
    }
  }
  //Output
  printf("%d\n", k); //
  for(int i = 0; i < MAX_BUF_SIZE; i++) {
    printf("%.2f ", output_buffer[i]);
  }
  printf("\n");
  //Puts data into a file
  FILE* executable = fopen("a.myexe", "wb");
  fwrite(output_buffer, sizeof(elem_t), MAX_BUF_SIZE, executable);
  fclose(executable);
  //free(strings);
	
  printf("Серьёзно?\n");
  return 0;
} 

int find_reg(char* string) {
  if (strstr(string, "ax")) {
    return 0;
  }
  if (strstr(string, "bx")) {
    return 1;
  }
  if (strstr(string, "cx")) {
    return 2;
  }
  if (strstr(string, "dx")) {
    return 3;
  }
  return -1;
}