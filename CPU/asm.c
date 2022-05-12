#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../parser/parser.h"

typedef float elem_t;
int find_reg(char*);
#define MAX_BUF_SIZE 1024

//выкидывает ошибку на buildString, output_buffer дальше пытается в нулевую память что-то закинуть

int main(const int argc, const char* argv[]) {
  //Translates text into an array of strings
  FILE* asm_file = fopen(argv[1], "rb");
  int file_byte_size = count_symbols(asm_file);
  printf("file_byte_size = %d\n", file_byte_size);
  char* file_buffer = fileToString(asm_file, file_byte_size); //работает
  fclose(asm_file);
  file_byte_size++;//потому что \n в конце
  //printf("file_buffer = \n%s", file_buffer);
  int lines_count = (int)count_lines(file_buffer, file_byte_size);
  printf("lines_count = %d\n", lines_count);
	int actual_lines_count = formatString(file_buffer, file_byte_size);
  printf("actual_lines_count = %d\n\n", actual_lines_count);
	char** strings = (char**)calloc(actual_lines_count, sizeof(char*));
	strings = buildString(file_buffer, strings, file_byte_size, actual_lines_count);
  for(int i = 0; i < actual_lines_count; i++) {
    for(int j = 0; j < strlen(strings[i]); j++) {
      if (strings[i][j]=='\n') printf("\\n");
      if (strings[i][j]=='\0') printf("\\0");
      printf("%c", strings[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  //Makes comands buffer
  int k = 0;
  elem_t* output_buffer = (elem_t*)malloc(sizeof(elem_t)*1024);

  for(int i = 0; i < MAX_BUF_SIZE; i++) {
    output_buffer[i] = -1000;
  }
  for(int i = 0; i < actual_lines_count; i++) {
    if (strstr(strings[i], "push") != NULL) {
      if (find_reg(strings[i]) == -1) {
        output_buffer[k] = 1;
        k++;
        output_buffer[k] = atof(strings[i]+5);
        k++;
      }
      else {
        output_buffer[k] = 0;
        k++;
        output_buffer[k] = find_reg(strings[i]+5);
        k++;
      }
    }
    else if (strstr(strings[i], "pop") != NULL) {
      output_buffer[k] = 2;
      k++;
      output_buffer[k] = find_reg(strings[i]+4);
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
      printf("%s\n", strings[i]);
      printf("%s %d\n", strings[i]+4, find_reg(strings[i]+4));
      output_buffer[k] = find_reg(strings[i]+4);
      k++;
      printf("%s %d\n", strings[i]+7, find_reg(strings[i]+7));
      output_buffer[k] = find_reg(strings[i]+7);
      k++;
    }
  }
  //Output
  printf("k = %d\n", k); //
  printf("\n");
  //Puts data into a file
  FILE* executable = fopen("a.myexe", "wb");
  fwrite(output_buffer, sizeof(elem_t), MAX_BUF_SIZE, executable);
  fclose(executable);
  for(int i = 0; i < MAX_BUF_SIZE; i++) { 
    printf("%.2f ", output_buffer[i]);
  }
  printf("\n");
  free(strings);
  free(file_buffer);
  printf("Серьёзно?\n");
  return 0;
} 

int find_reg(char* string) {
  if (strstr(string, "ax") && (string[0] == 'a' || string[1] == 'a')) {
    return 0;
  }
  if (strstr(string, "bx") && (string[0] == 'b' || string[1] == 'b')) {
    return 1;
  }
  if (strstr(string, "cx") && (string[0] == 'c' || string[1] == 'c')) {
    return 2;
  }
  if (strstr(string, "dx") && (string[0] == 'd' || string[1] == 'd')) {
    return 3;
  }
  return -1;
}