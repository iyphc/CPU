#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../parser/parser.h"

typedef float elem_t;
#define MAX_BUF_SIZE 1024

typedef struct {
  char* label;
  int pointer;
} label;

elem_t* fill_command_buffer(char**strings, int actual_lines_count, int * cnt);
label * fill_labels_array(char ** strings, size_t array_size, size_t label_num);
size_t count_label_numbers (char** strings, size_t size);
void init_label (label * label, int number_of_lines, char * name);
void dist_label (label * label);
int find_reg(char*);

int main(const int argc, const char* argv[]) {
  //Translates text into an array of strings
  FILE* asm_file = fopen(argv[1], "rb");
  //Counts number of symbols in the file
  int file_byte_size = count_symbols(asm_file); 
  //Converting a file to a string
  char* file_buffer = fileToString(asm_file, &file_byte_size); 
  //Closes the file
  fclose(asm_file);
  //Counting numbers of lines in the file
  int lines_count = (int)count_lines(file_buffer, file_byte_size);
  //Counting numbers of commands in the file
	int actual_lines_count = formatString(file_buffer, file_byte_size);
  //Translates one dimensional array to two dimensional array
	char** strings = (char**)calloc(actual_lines_count, sizeof(char*));
	strings = buildString(file_buffer, strings, file_byte_size, actual_lines_count);
  //Checks for labels
  int labels_num = count_label_numbers(strings, actual_lines_count);
  //...................
  //Makes comands buffer
  int k = 0;
  elem_t* output_buffer = fill_command_buffer(strings, actual_lines_count, & k);
  //Puts data into a file
  FILE* executable = fopen("a.myexe", "wb");
  fwrite(output_buffer, sizeof(elem_t), MAX_BUF_SIZE, executable);
  fclose(executable);
  free(strings);
  free(file_buffer);
  printf("COMPLETED\n");
  return 0;
} 

label * fill_labels_array(char ** strings, size_t array_size, size_t label_num) {
  label * labels = (label *)calloc(label_num, sizeof(label));
  int cnt = 0;
  char * name = NULL;
  for(int i = 0; i < array_size; i++) {
    if (strstr(strings[i], ":")) {
      name = (char *)calloc(strlen(strings[i])-1, sizeof(char));
      for(int j = 0; j < strlen(strings[i])-1; j++) {
        name[j] = strings[i][j];
      }
      init_label(labels+cnt, 0, name);
      cnt++;
    }
  }
  return labels;
}

size_t count_label_numbers (char** strings, size_t size) {
  size_t label_num = 0;
  for(int i = 0; i < size; i++) {
    if (strstr(strings[i], ":")) label_num++; 
  }
  return label_num;
}

void init_label (label * label, int number_of_lines, char * name) {
  label->label = name;
  label->pointer = number_of_lines;
}

void dist_label (label * label) {
  free(label->label);
  free(label);
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

elem_t* fill_command_buffer(char**strings, int actual_lines_count, int * cnt) {
  elem_t* output_buffer = (elem_t*)calloc(sizeof(elem_t), actual_lines_count*4);
  //Makes marks array
  int labels_num = count_label_numbers(strings, actual_lines_count);
  label * labels = fill_labels_array(strings, actual_lines_count, labels_num);
  //
  for(int i = 0; i < actual_lines_count*4; i++) {
    output_buffer[i] = -1000;
  }
  //Fill comands buffer
  for(int i = 0; i < actual_lines_count; i++) {
    if (strstr(strings[i], "push") != NULL) {
      if (find_reg(strings[i]) == -1) {
        output_buffer[*cnt] = 1;
        cnt[0]++;
        output_buffer[*cnt] = atof(strings[i]+5);
        cnt[0]++;
      }
      else {
        output_buffer[*cnt] = 0;
        cnt[0]++;
        output_buffer[*cnt] = find_reg(strings[i]+5);
        cnt[0]++;
      }
    }
    else if (strstr(strings[i], "pop") != NULL) {
      output_buffer[*cnt] = 2;
      cnt[0]++;
      output_buffer[*cnt] = find_reg(strings[i]+4);
      cnt[0]++;
    }
    else if (strstr(strings[i], "add") != NULL) {
      output_buffer[*cnt] = 3;
      cnt[0]++;
      output_buffer[*cnt] = find_reg(strings[i]+4);
      cnt[0]++;
      output_buffer[*cnt] = find_reg(strings[i]+7);
      cnt[0]++;
    }
    else if (strstr(strings[i], "mov") != NULL) {
      output_buffer[*cnt] = 4;
      cnt[0]++;
      output_buffer[*cnt] = find_reg(strings[i]+4);
      cnt[0]++;
      output_buffer[*cnt] = find_reg(strings[i]+7);
      cnt[0]++;
    }
    else if (strstr(strings[i], ":") != NULL) {
      //обработка метки
    }
  }
  return output_buffer;
}
