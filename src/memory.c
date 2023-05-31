#include "memory.h"
#include <stdio.h>

//this file contains the realloc functions for every dynamic array in the project 
//to avoid boilerplates; dynamic arrays reallocations will be handled here

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    /*
    exits on realloc failure
    */
  if (newSize == 0) {
    if(pointer) free(pointer);
    return NULL;
  }

  void* result = realloc(pointer, newSize);
  if(!result)fprintf(stderr, "failed to reallocate \n");
  
  return result;
}