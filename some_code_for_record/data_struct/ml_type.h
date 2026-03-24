/*************************************************************************
	> File Name: ml_type.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 13 Mar 2026 01:50:00 PM CST
 ************************************************************************/


#ifndef ML_TYPES_H
#define ML_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

enum _ml_result_t {
	ML_RESULT_INVALID = 0,
	ML_RESULT_OK,
};
typedef uint8_t ml_result_t;

typedef uintptr_t ml_uintptr_t;
typedef intptr_t ml_intptr_t;

#endif
