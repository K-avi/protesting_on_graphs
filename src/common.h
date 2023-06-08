#ifndef COMMON_H
#define COMMON_H

#define debug_mode


typedef enum {
    FG_OK, GT_NULL, GT_MALLOC, GT_REALLOC, GT_READ, GT_WRITE , GT_INDEX, GT_NOREAD, GT_READFAIL,
    GT_PARSE, GT_NOWRITE , GT_OPENFAIL, ERRFLAG_NOFILE,  GE_NULL, GE_MALLOC, GE_REALLOC, 
    GT_ARFULL, T_NULL, T_REALLOC, WA_NULL, WA_ALLOC,  WTE_NULL, WTE_REALLOC, WTE_IN, 
    WT_NULL, WT_REALLOC, WT_INDEX_TOO_BIG, WT_NOT_FOUND, GT_SIZE, GTE_NULL, NDREF_NULL, 
    MV_NONEIGHBOORS, WKR_NULL, LINEREF_NULL, WRS_NULL, WRS_MALLOC, WRS_REALLOC, WRS_EMPTYSTACK , 
    WKCN_NULL, WKCN_MALLOC, WKCN_REALLOC, MV_NULL, L_ARRNULL, T_CANTCHOOSE, ERRFLAG_REALLOC,
    ERRFLAG_INVALID_ARG, PRS_NULL, PRS_INVALID_FORMAT, PRS_COEFF, AR_MALLOC, AR_NULL, AR_FULL,
}ErrFlag;

//ndref stands for node ref 
//wkr stands for walker ref
//fg just stands for flag
#define GT_OK FG_OK //gt stands for graph table
#define ERRFLAG_OK FG_OK //errflag are the global/ generic flags for errors
#define MV_OK FG_OK //mv stands for movement
#define T_OK FG_OK //t stands for tactics 
#define WA_OK FG_OK //wa stands from walker array
#define WTE_OK FG_OK //wte stands for walker table entry (deprecated)
#define WT_OK FG_OK //wt stands for walker table (deprecated)
#define GE_OK FG_OK //ge stands for graph table entry  
#define WRS_OK FG_OK//wrf stands for walker ref stack
#define IT_OK FG_OK //it stands for iteration 
#define WKCN_OK FG_OK// walker cur next arrays
#define LARR_OK FG_OK //line arr
#define PFN_OK FG_OK // parse function
#define AR_OK FG_OK //arena 

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //not necessary ?
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <unistd.h> //windows compatibility is overrated anyways


#endif 