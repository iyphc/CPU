#include "asm.h"

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
  fwrite(output_buffer, sizeof(elem_t), actual_lines_count*4, executable);
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
      init_label(labels+cnt, -1, i, name);
      cnt++;
    }
    else if (strstr(strings[i], "call")) {
      name = (char *)calloc(strlen(strings[i])-5, sizeof(char));
      for(int j = 0; j < strlen(strings[i])-5; j++) {
        name[j] = strings[i][j+5];
      }
      init_label(labels+cnt, -1, i, name);
      cnt++;
    }
  }
  return labels;
}

size_t count_label_numbers (char** strings, size_t size) { 
  size_t label_num = 0;
  for(int i = 0; i < size; i++) {
    if (strstr(strings[i], ":")) { 
      label_num++; 
    }

  }
  return label_num;
}

void init_label (label * label, int number_of_lines, int str_pos, char * name) {
  label->label = name;
  label->pointer = number_of_lines;
  label->str_pos = str_pos;
}

void dist_label (label * label) {
  free(label->label);
  free(label);
}

int find_reg(char* string) {
  int flag = -1;
  if (strstr(string, "ax")) {
    if (string[0] == 'a' || string[1] == 'a') {
      return 0;
    }
    else {
      flag = 1;
    }
  }
  if (strstr(string, "bx")) {
    if (string[0] == 'b' || string[1] == 'b') {
      return 1;
    }
    else {
      flag = 1;
    }
  }
  if (strstr(string, "cx")) {
    if (string[0] == 'c' || string[1] == 'c') {
      return 2;
    }
    else {
      flag = 1;
    }
  }
  if (strstr(string, "dx")) {
    if (string[0] == 'd' || string[1] == 'd') {
      return 3;
    }
    else {
      flag = 1;
    }
  }
  return flag;
}

label * find_label(char * label_name, label * labels, size_t label_num) {
  for(int i = 0; i < label_num; i++) {
    if (strstr(label_name, labels[i].label)) return (labels + i);
  }
  return NULL;
}

elem_t* fill_command_buffer(char**strings, int actual_lines_count, int * cnt) {
  elem_t* output_buffer = (elem_t*)calloc(sizeof(elem_t), actual_lines_count*4);
  //Makes marks array
  int labels_num = count_label_numbers(strings, actual_lines_count);
  label * labels = fill_labels_array(strings, actual_lines_count, labels_num);
  for(int i = 0; i < labels_num; i++) {
    labels[i].pointer = -1;
  }
  //
  for(int i = 0; i < actual_lines_count*4; i++) {
    output_buffer[i] = -1000;
  }
  //Find start of program
  output_buffer[0] = 7;
  output_buffer[1] = find_label("start", labels, labels_num)->str_pos;
  *cnt+=2;
  //Fill comands buffer
  for(int i = 0; i < actual_lines_count; i++) {
    if (strstr(strings[i], "push") != NULL) {
      if (find_reg(strings[i]) == -1) {
        output_buffer[*cnt] = 1;
        (*cnt)++;
        output_buffer[*cnt] = atof(strings[i]+5);
        (*cnt)++;
      }
      else {
        output_buffer[*cnt] = 0;
        (*cnt)++;
        output_buffer[*cnt] = find_reg(strings[i]+5);
        (*cnt)++;
      }
    }
    else if (strstr(strings[i], "pop") != NULL) {
      output_buffer[*cnt] = 2;
      (*cnt)++;
      output_buffer[*cnt] = find_reg(strings[i]+4);
      (*cnt)++;
    }
    else if (strstr(strings[i], "add") != NULL) {
      output_buffer[*cnt] = 3;
      (*cnt)++;
      output_buffer[*cnt] = find_reg(strings[i]+4);
      (*cnt)++;
      output_buffer[*cnt] = find_reg(strings[i]+7);
      (*cnt)++;
    }
    else if (strstr(strings[i], "mov") != NULL) {
      output_buffer[*cnt] = 4;
      (*cnt)++;
      output_buffer[*cnt] = find_reg(strings[i]+4);
      (*cnt)++;
      output_buffer[*cnt] = find_reg(strings[i]+7);
      (*cnt)++;
    }
    else if (strstr(strings[i], "jmp")) {
      label * des_label = find_label(strings[i]+4, labels, labels_num);
      if (des_label) {
        output_buffer[*cnt] = 7;
        (*cnt)++;
        output_buffer[*cnt] = (elem_t)des_label->str_pos;
        (*cnt)++;
      }
    }
    else if (strstr(strings[i], "call")) {
      label * des_label = find_label(strings[i]+5, labels, labels_num);
      if (des_label) {
        output_buffer[*cnt] = 8;
        (*cnt)++;
        output_buffer[*cnt] = (elem_t)des_label->str_pos;
        (*cnt)++;
      }
    }
    else if (strstr(strings[i], "ret")) {
      output_buffer[*cnt] = 9;
      (*cnt)++;
    }
    else if (strstr(strings[i], ":")) {
      label * des_label = find_label(strings[i], labels, labels_num); //desired_label
      if (des_label && des_label->pointer == -1) {
        des_label->pointer = *cnt;
      }
    }
  }
  //если идёт 7, а потом следом -1, то надо ещё раз посмотреть буфер и заменить -1 на значение
  //прохо
  for(int i = 0; i < (*cnt); i++) {
    if (output_buffer[i] == (elem_t)7 || output_buffer[i] == (elem_t)8) {
      label * des_label = find_label(strings[(int)output_buffer[i+1]], labels, labels_num); //ошибка
      if (des_label != NULL) output_buffer[i+1] = des_label->pointer;
      else output_buffer[i+1] = 123;
    }
  }
  return output_buffer;
}
