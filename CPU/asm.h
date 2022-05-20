#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../parser/parser.h"

typedef float elem_t;
#define MAX_BUF_SIZE 1024

typedef struct {
  char* label;
  int pointer;
  int str_pos; //развить идею с str_pos
} label;

elem_t* fill_command_buffer(char**strings, int actual_lines_count, int * cnt);
label * fill_labels_array(char ** strings, size_t array_size, size_t label_num);
label * find_label(char * label_name, label * labels, size_t label_num);
size_t count_label_numbers (char** strings, size_t size);
void init_label (label * label, int number_of_lines, int str_pos, char * name);
void dist_label (label * label);
int find_reg(char*);
