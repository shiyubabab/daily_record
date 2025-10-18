/*************************************************************************
	> File Name: lv_obj_class.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sat 18 Oct 2025 01:45:40 PM CST
 ************************************************************************/

#ifndef LV_OBJ_CLASS_H
#define LV_OBJ_CLASS_H

/**************
*	INCLUCDE
***************/
#include "lv_common.h"

/**************
*	DEFINES
***************/

/**************
*	TYPEDEFS
***************/
struct lv_obj_s;
struct lv_obj_class_s;

typedef void (*class_func)(const struct lv_obj_class_s class_p, struct lv_obj_t *obj);

typedef struct lv_obj_class_s{
	const struct lv_obj_class_s * base_class;
	class_func constructor_cb;
	class_func destructor_cb;
	void * user_data;
	uint32_t width_def;
	uint32_t height_def;
	uint32_t instancew_size : 16;
} lv_obj_class_t;


/*******************
* GLOBAL PROTOTYPES
********************/

struct lv_obj_s * lv_obj_class_create_obj(const struct lv_obj_class_s * class_p,struct lv_obj_s parent);


#endif
