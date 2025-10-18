/*************************************************************************
	> File Name: lv_obj.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sat 18 Oct 2025 01:40:51 PM CST
 ************************************************************************/

#ifndef LV_OBJ_H
#define LV_OBJ_H

/*************
*	INCLUDE
**************/
#include "lv_common.h"
#include "lv_obj_class.h"

/*************
*	DEFINES
**************/


/*************
*	TYPEDEFS
**************/

struct lv_obj_s;

extern const lv_obj_class_t lv_obj_class;

typedef struct lv_obj_spec_attr_s{
	struct lv_obj_s ** children;
	uint32_t child_cnt;
} lv_obj_spec_attr_t;

typedef struct lv_obj_s{
	const lv_obj_class_t * class_p;
	struct lv_obj_s * parent;
	lv_obj_spec_attr_t * spec_attr;
	void * user_data;
} lv_obj_t;

#endif
