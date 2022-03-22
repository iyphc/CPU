#include <stdio.h>
#include <stdlib.h>

/**  
 * @file onegin.h
 * @brief Contains funstion prototypes
*/

size_t count_lines(char *input, size_t size);
size_t count_symbols(FILE *input);
char* fileToString(FILE* source, size_t size);
char** buildString(char* source, char** target, size_t size);
int formatString(char* source, size_t size);
int comp(const void* p1, const void* p2);
