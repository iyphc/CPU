#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../parser/parser.h"

typedef float elem_t;
int find_reg(char*);

//выкидывает ошибку на buildString, output_buffer дальше пытается в нулевую память что-то закинуть

int main(const int argc, const char* argv[]) {
  //Translates text into an array of strings
  FILE* asm_file = fopen(argv[1], "r");
  int file_byte_size = count_symbols(asm_file);
  printf("1\n");
  char* file_buffer = fileToString(asm_file, file_byte_size);
  printf("2\n");
  int lines_count = (int)count_lines(file_buffer, file_byte_size);
  printf("3\n");
	int actual_lines_count = formatString(file_buffer, file_byte_size);
  printf("4\n");
	char** strings = (char**)calloc(actual_lines_count, sizeof(char*));
  printf("5\n");
	strings = buildString(file_buffer, strings, file_byte_size);
  printf("6\n");
  //Makes comands buffer
  elem_t* output_buffer = (elem_t*)calloc(actual_lines_count*3, sizeof(elem_t));
  printf("7\n");
  for(int i = 0; i < actual_lines_count*3; i++) {
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
    }
  }
  //Output
  printf("%d\n", k);
  for(int i = 0; i < actual_lines_count*3; i++) {
    printf("%.2f ", output_buffer[i]);
  }
  printf("\n");
  //Puts data into a file
  FILE* executable = fopen("a.myexe", "wb");
  fwrite(output_buffer, sizeof(float), actual_lines_count, executable);
  free(strings);
	free(file_buffer);
	fclose(asm_file);
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