#include "parser.h"
#include "string.h"
//ftell, fseek, fread

/**  
 * @file main.c
 * @brief Contains main function
*/

int main() {
	//Read file
	FILE *input = fopen("onegin.txt", "rb");
	int file_byte_size = count_symbols(input);
	//Convert file to char* array
	char* file_buffer = fileToString(input, file_byte_size);
	//Split one char* array in a two-dimensional array of strings
	int lines_count = (int)count_lines(file_buffer, file_byte_size);
	int actual_lines_count = formatString(file_buffer, file_byte_size);
	char** strings = (char**)calloc(actual_lines_count, sizeof(char*));
	strings = buildString(file_buffer, strings, file_byte_size);
	//sorting two-dimesional array
	qsort(strings, actual_lines_count, sizeof(char*), comp);
	//output
	printf("The number of bytes in the file = %d\n\n", file_byte_size);
	printf("Initial number of lines = %d\n\n", lines_count);
	printf("Actual number of lines = %d\n\n", actual_lines_count);
	for(int i = 0; i < actual_lines_count; i++) {
		printf("%s\n", strings[i]);
	}
	free(strings);
	free(file_buffer);
	fclose(input);
	//printf("\nНеужели мы справились?\nCongratulations, all tests passed!\n\n");
	return 0;
}