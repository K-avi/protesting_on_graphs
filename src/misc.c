#include "misc.h"
#include "common.h"
#include <stdint.h>
#include <stdio.h>
//this file contains the realloc functions for every dynamic array in the project 
//to avoid boilerplates; dynamic arrays reallocations will be handled here
//actually now most of the project is static so it's kinda useless ig ? 
//I might keep it cuz I think it's a nice wrapper around malloc

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


char * str_flag(uint8_t flag){

  switch (flag) {

  case FG_OK : return "no error; how did you get here?\n";
  default : return "unknown error ; how did you get here?\n";
  }
 
}

void report_err( char * msg , uint8_t flag){

  fprintf(stderr, "%s%s\n", str_flag(flag), msg);
  return;
}