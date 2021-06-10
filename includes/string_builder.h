#ifndef __STRING_BUILDER__
#define __STRING_BUILDER__

#include <stdint.h>

typedef struct string_builder {
  char *str;
  uint32_t length;
  uint32_t current;
} String;

// freeing allocated memory for string
void free_string(String *);
/*
  Allocate memory for string
  default size:256 will be allocated if no reserve parameter will be given.
*/
String *allocate_string(uint32_t);

//insert string into string builder
int insert_string(String **, char *);
#endif
