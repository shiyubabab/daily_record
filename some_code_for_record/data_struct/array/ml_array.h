/*************************************************************************
	> File Name: ml_array.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 13 Mar 2026 02:00:17 PM CST
 ************************************************************************/

#ifndef ML_ARRAY_H
#define ML_ARRAY_H

#include "../ml_type.h"

typedef struct {
	uint8_t * data;
	uint32_t size;
	uint32_t capacity;
	uint32_t element_size;
} ml_array_t;

static inline void err_print(const char *s)
{
	printf("line __%d__ Error: %s fail.",__LINE__,s);
	exit(EXIT_FAILURE);
}

void ml_array_init(ml_array_t * array, uint32_t capacity, uint32_t element_size);
void ml_array_resize(ml_array_t * array, uint32_t new_capacity);
void ml_array_deinit(ml_array_t * array);

static inline uint32_t ml_array_size(const ml_array_t * array)
{
	return array->size;
}

static inline uint32_t ml_array_capacity(const ml_array_t * array)
{
	return array->capacity;
}

static inline bool ml_array_is_empty(const ml_array_t * array)
{
	return array->size == 0;
}

static inline bool ml_array_is_full(const ml_array_t *array)
{
	return array->size == array->capacity;
}

void ml_array_copy(ml_array_t * target, const ml_array_t *source);

static inline void ml_array_clear(ml_array_t * array)
{
	array->size = 0;
}

ml_result_t ml_array_remove(ml_array_t * array, uint32_t index);

ml_result_t ml_array_erase(ml_array_t * array, uint32_t start, uint32_t end);

ml_result_t ml_array_concat(ml_array_t * array, const ml_array_t * other);

ml_result_t ml_array_push_back(ml_array_t * array, const void * element);

ml_result_t ml_array_assign(ml_array_t * array, uint32_t index, const void * value);

void * ml_array_at(const ml_array_t * array, uint32_t index);

static inline void * ml_array_front(const ml_array_t * array)
{
	return ml_array_at(array,ml_array_size(array) - 1);
}

#endif

#ifdef ML_ARRAY_IMPLEMENTATION
void ml_array_init(ml_array_t * array, uint32_t capacity, uint32_t element_size)
{
	array->size = 0;
	array->capacity = capacity;
	array->element_size = element_size;

	array->data = malloc(capacity * element_size);
	if(!array->data) err_print("malloc");
}

void ml_array_resize(ml_array_t * array, uint32_t new_capacity)
{
	uint8_t * data = realloc(array->data,array->element_size * new_capacity);
	if(!data) err_print("realloc");
	array->data = data;
	array->capacity = new_capacity;
	if(array->size > new_capacity){
		array->size = new_capacity;
	}
}

void ml_array_deinit(ml_array_t * array)
{
	if(array->data){
		free(array->data);
		array->data = NULL;
	}

	array->size = 0;
	array->capacity = 0;
}

void ml_array_copy(ml_array_t * target, const ml_array_t *source)
{
	if(ml_array_is_empty(source)){
		return;
	}
	ml_array_deinit(target);
	ml_array_init(target, source->capacity, source->element_size);
	memcpy(target->data, source->data, source->element_size * source->size);
	target->size = source->size;
}


ml_result_t ml_array_remove(ml_array_t * array, uint32_t index)
{
	if(index >= array->size){
		return ML_RESULT_INVALID;
	}

	if(index == array->size - 1){
		ml_array_resize(array,array->size - 1);
		return ML_RESULT_OK;
	}

	uint8_t * start = ml_array_at(array, index);
	uint8_t * remaining = start + array->element_size;
	uint32_t remaining_size = (array->size - index - 1) * array->element_size;
	memmove(start, remaining, remaining_size);
	ml_array_resize(array,array->size - 1);
	return ML_RESULT_OK;
}

ml_result_t ml_array_erase(ml_array_t * array, uint32_t start, uint32_t end)
{
	if(end > array->size) {
		end = array->size;
	}

	if(start >= end) {
		return ML_RESULT_INVALID;
	}

	if(end == array->size) {
		ml_array_resize(array, start);
		return ML_RESULT_OK;
	}

	uint8_t * start_p = ml_array_at(array,start);
	uint8_t * remaining = start_p + (end - start) * array->element_size;
	uint32_t remaining_size = (array->size - end) * array->element_size;
	memcpy(start_p, remaining, remaining_size);
	ml_array_resize(array, array->size - (end - start));
	return ML_RESULT_OK;
}

ml_result_t ml_array_concat(ml_array_t * array, const ml_array_t * other)
{
	if(array->data) return ML_RESULT_INVALID;
	uint32_t size = other->size;
	if(array->size + size > array->capacity) {
		ml_array_resize(array, array->size + size);
	}

	uint8_t *data = array->data + array->size * array->element_size;
	memcpy(data,other->data, array->element_size * size);
	array->size += size;
	return ML_RESULT_OK;
}

ml_result_t ml_array_push_back(ml_array_t * array, const void * element)
{
	if(!array->data) return ML_RESULT_INVALID;

	if(array->size == array->capacity) {
		ml_array_resize(array, array->capacity + 1);
	}

	uint8_t * data = array->data + array->size * array->element_size;
	memcpy(data, element, array->element_size);
	array->size++;
	return ML_RESULT_OK;
}

ml_result_t ml_array_assign(ml_array_t * array, uint32_t index, const void * value)
{
	uint8_t * data = ml_array_at(array,index);
	if(data == NULL) return ML_RESULT_INVALID;

	memcpy(data,value,array->element_size);
	return ML_RESULT_OK;
}

void * ml_array_at(const ml_array_t * array, uint32_t index)
{
	if(index >= array->size) {
		return NULL;
	}

	if(array->data == NULL) return NULL;
	return array->data + index * array->element_size;
}

#endif
