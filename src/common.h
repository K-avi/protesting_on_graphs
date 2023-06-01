#ifndef COMMON_H
#define COMMON_H

#define debug_mode


typedef enum {
    FG_OK, GT_NULL, GT_MALLOC, GT_REALLOC, GT_READ, GT_WRITE , GT_INDEX, GT_NOREAD, GT_READFAIL,
    GT_PARSE, GT_NOWRITE , GT_OPENFAIL, ERRFLAG_NOFILE, GE_OK, GE_NULL, GE_MALLOC, GE_REALLOC, 
    GT_ARFULL, T_NULL, T_REALLOC, WA_NULL, WA_ALLOC,  WTE_NULL, WTE_REALLOC, WTE_IN, 
    WT_NULL, WT_REALLOC, WT_INDEX_TOO_BIG, WT_NOT_FOUND, GT_SIZE, GTE_NULL,
}ErrFlag;

#define GT_OK FG_OK
#define ERRFLAG_OK FG_OK
#define MV_OK FG_OK
#define T_OK FG_OK
#define WA_OK FG_OK
#define WTE_OK FG_OK 
#define WT_OK FG_OK

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //not necessary ?
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <unistd.h> //windows compatibility is overrated anyways


#endif 