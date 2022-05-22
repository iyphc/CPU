#include <stdio.h>
#include <stdlib.h>

/**  
 * @file onegin.h
 * @brief Contains funstion prototypes
*/

size_t count_lines(char *input, size_t size);
size_t count_symbols(FILE *input);
char* fileToString(FILE* source, int * size);
char** buildString(char* source, char** target, size_t source_size, size_t target_size);
int formatString(char* source, size_t size); //rename to count_non_empty_strings
int comp(const void* p1, const void* p2);
