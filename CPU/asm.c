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
  fclose(asm_file); //когда не в коментарии -- работает до конца и только потом малок
  file_byte_size++;//потому что \n в конце
  printf("file_buffer = %s\n", file_buffer);
  int lines_count = (int)count_lines(file_buffer, file_byte_size);
  printf("lines_count = %d\n", lines_count);
	int actual_lines_count = formatString(file_buffer, file_byte_size);
  printf("actual_lines_count = %d\n", actual_lines_count);
	char** strings = (char**)calloc(actual_lines_count, sizeof(char*));
  printf("5\n");
	strings = buildString(file_buffer, strings, file_byte_size);
  for(int i = 0; i < actual_lines_count; i++) {
    for(int j = 0; j < strlen(strings[i]); j++) {
      printf("%c", strings[i][j]);
    }
    printf("\n");
  }
  //Makes comands buffer
  printf("7\n");
  int* output_buffer = (int*)calloc(MAX_BUF_SIZE, sizeof(int));
  printf("8\n"); 
  printf("9\n");
  int k = 0;
  /*for(int i = 0; i < actual_lines_count; i++) {
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
  printf("k = %d\n", k); //
  printf("\n");
  //Puts data into a file
  FILE* executable = fopen("a.myexe", "wb");
  fwrite(output_buffer, sizeof(elem_t), MAX_BUF_SIZE, executable);
  fclose(executable);
  */
  free(strings);
  free(file_buffer);
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