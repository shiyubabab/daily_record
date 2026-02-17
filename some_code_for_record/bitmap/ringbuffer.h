/*************************************************************************
	> File Name: ringbuffer.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 17 Feb 2026 04:32:51 PM CST
 ************************************************************************/

#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <stdio.h>
#include <stdbool.h>

#define RB_SIZE 6
typedef struct {
	int buffer[RB_SIZE];
	int head;
	int tail;
} FastRB;

void rb_init(FastRB *rb);
bool isEmpty(FastRB *rb);
bool isFull(FastRB *rb);
void rb_write(FastRB *rb, int data);
bool rb_read(FastRB *rb,int *data);

#endif

#ifdef RINGBUFFER_IMPLEMENTATION

void rb_init(FastRB *rb)
{
	rb->head = 0;
	rb->tail = 0;
}

bool isEmpty(FastRB *rb)
{
	return rb->head == rb->tail;
}

bool isFull(FastRB *rb)
{
	return (rb->head + 1) % RB_SIZE == rb->tail;
}

void rb_write(FastRB *rb, int data)
{
	rb->buffer[rb->head] = data;
	if(isFull(rb)){
		rb->tail = (rb->tail + 1) % RB_SIZE;
	} else {
		// TODO print some log
	}
	rb->head = (rb->head + 1) % RB_SIZE;
}

bool rb_read(FastRB *rb,int *data)
{
	if(isEmpty(rb)){
		return false;
	}

	*data = rb->buffer[rb->tail];
	rb->tail = (rb->tail + 1) % RB_SIZE;
	return true;
}

#endif
