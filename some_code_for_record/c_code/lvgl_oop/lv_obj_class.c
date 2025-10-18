/*************************************************************************
	> File Name: lv_obj_class.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sat 18 Oct 2025 02:45:38 PM CST
 ************************************************************************/

/******************
*	  INCLUDES
*******************/
#include "lv_common.h"
#include "lv_obj.h"

/******************
*	  DEFINES
*******************/
#define MY_CLASS &lv_obj_class

/******************
*	  TYPEDEFS
*******************/


/******************
* GLOBAL PROTOTYPES
*******************/
static void lv_obj_construct(lv_obj_t *obj);
static uint32_t git_instance_size(const lv_obj_class_t * class_p);
static uint32_t get_instance_size(const lv_obj_class_t * class_p);

/******************
* GLOBAL FUNCTIONS
*******************/
lv_obj_t * lv_obj_class_create_obj(const lv_obj_class_t * class_p, lv_obj * parent)
{
	uint32_t s = get_instance_size(class_p);
	lv_obj_t * obj =(lv_obj_t *)malloc(s);
	if(obj == NULL) return NULL;
	memset(obj,0,sizeof(obj));
	obj->class_p = class_p;
	obj->parent = parent;

	if(parent == NULL){
		printf("do not have parent.\n");
	}
	else{
		if(parent->spec_attr == NULL){
			lv_obj_spec_attr_t * spec_attr = (lv_obj_spec_attr_t *)malloc(sizeof(lv_obj_spec_attr_t));
			parent->spec_attr = spec_attr;
		}
		if(parent->spec_attr->children == NULL){
			parent -> spec_attr -> chaildren = (lv_obj_t *)malloc(sizeof(lv_obj_t *));
			parent -> spec_attr -> children[0]=obj;
			parent -> spec_attr -> child_cnt = 1;
		}else{
			parent -> spec_attr -> child_cnt++;
			parent -> spec_attr -> children = (lv_obj_t *)malloc(sizeof(lv_obj_t *));
			parent -> spec_attr -> children[parent->spec_attr->child_cnt - 1] = obj;

		}
	}
	return obj;
}

void lv_obj_class_init_obj(lv_obj_t * obj)
{
	lv_obj_construct(ob);
	//todo event pass to parent
}

static void lv_obj_construct(lv_obj_t * obj)
{
	const lv_obj_class_t * original_class_p = obj->class_p;
	if(obj->class_p->base_class){
		obj->class_p = obj->class_p->base_class;
		lv_obj_construct(obj);
	}
	obj->class_p = original_class_p;
	if(obj->class_p->constructor_cb) obj->class_p->constructor_cb(obj->class_p,obj);
}

static uint32_t get_instance_size(const lv_obj_class_t * class_p)
{
	const lv_obj_class_t * base = class_p;

	while(base && base->instance_size == 0) base = base->base_class;
	if(base == NULL) return 0;

	return base->instance_size;
}
