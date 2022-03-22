#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "string.h"

/**  
 * @file onegin.c
 * @brief Contains funstions for sorting strings
*/

/**
* @brief Character counting function
* @param input The file in which the number of characters is counted
* @return Number of characters
*/


size_t count_symbols(FILE *input) {
	if (input == NULL) {
    printf("\nCrashed\n\n");
    return 0;
  }
	fseek(input, 0, SEEK_END);
	size_t count = ftell(input);
	fseek(input, 0, SEEK_SET);
	return count;
}

/**
* @brief Lines counting function
* @param input The file in which the number of characters is counted
* @param size Number of characters
* @return Initial number of lines
*/


size_t count_lines(char *input, size_t size) {
	if (input == NULL) {
    printf("\nCrashed\n\n");
    return 0;
  }
	size_t ret = 0;
	for (int i = 0; i < size; i++) {
		if (input[i] == '\n') {
			ret++;
		}
	}
	return ret;
}

/**
* @brief Translates file to strings
* @param source The file 
* @param size The number of characters in the file
* @return Text from file
*/


char* fileToString(FILE* source, size_t size) {
	if (source == NULL) {
        printf("\nCrashed\n\n");
        return NULL;
    }
	char* file_buffer = (char*)calloc(size, sizeof(char));	
	fread(file_buffer, size, sizeof(char), source);
	return file_buffer;
}

/**
* @brief Changes all \n to \0 and counts the number of the strings without unnecessary hyphenation
* @param source The string in which the characters changes
* @param size Number of characters in bytes
* @return Actual number of lines
*/

int formatString(char* source, size_t size) {
	if (source == NULL) return 0;
	int flag = 0, lines_count = 0;
	char* from = source;
	while(from < source+size-1) {
		flag = 0;
		while (*from == '\n') {
			if(flag == 0) {
				lines_count++;
			}
			flag++;
			*from = '\0';
			from++;
		}
		from++;
	}
	return lines_count;
}

/**
* @brief Converts the buffer to a two-dimensional array of strings
* @param source The buffer from witch the lines are copied
* @param target The two-dimensional array into which the strings are copied
* @param size Number of characters in bytes
* @return Two-dimensional array into which the strings will be copied
*/

char** buildString(char* source, char** target, size_t size) {
	if (source == NULL || target == NULL) {
        printf("\nCrashed\n\n");
        return NULL;
    } 
    char* from = source; char* to = source;
  	int i = 1, flag = 0;
		//Для отсечения лишних \0 в самом начале
  	if(from[0] == '\0') {
  		while(*from == '\0'){
  			from++;
  		}
  	}
  	target[0] = to;
		//Само усечение большого количества \0
    while (from < source + size-1) {
    	flag = 0;
    	*to = *from;
    	while(*from == '\0' && from < source + size-1) {
	    	if (*from == '\0' && flag == 0) {
	    		target[i] = (to+1);
	    		i++;
	    	}
	    	flag++;
	    	from++;
    	}
    	if (flag != 0) {
    		from--;
    	}	
    	to++; from++;
    }
	return target;
}

/**
* @brief comparator
* @param p1 The first string
* @param p2 The second strings
* @return Difference between the first distiguished characters
*/

int comp(const void* p1, const void* p2) {
	return myStrcmp(*(const char**)p1, *(const char**)p2);
}