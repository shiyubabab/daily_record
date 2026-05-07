/*************************************************************************
	> File Name: ll.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 23 Apr 2026 01:37:21 PM CST
 ************************************************************************/


#ifndef LL_H_
#define LL_H_

#ifndef LL_ASSERT
#include <assert.h>
#define LL_ASSERT assert

#if defined(LL_REALLOC) && !defined(LL_FREE) || !defined(LL_REALLOC) && defined(LL_FREE)
#error "You must define both LL_REALLOC and LL_FREE, or neither."
#endif

#if !defined(LL_REALLOC) && !defined(LL_FREE)
#include <stdlib.h>
#define LL_REALLOC(p,s) realloc(p,s)
#define LL_FREE(p) free(p)
#endif

#ifndef LL_INIT_CAP
#define LL_INIT_CAP 32
#endif

#ifndef LL_INFO
#include <stdio.h>
#define LL_INFO printf
#endif

#include <stddef.h>
#include <string.h>
#include <stdint.h>

#if UINTPTR_MAX == 0xffffffffffffffff
	#define PLATFORM_CACHE_LINE 64
#else
	#define PLATFORM_CACHE_LINE 32
#endif

#define LL_ALIGN __attribute__((aligned(PLATFORM_CACHE_LINE)))

typedef struct {
	size_t count;
	size_t capacity;
	ptrdiff_t front;
	ptrdiff_t back;
} LL_ALIGN ll__header;

#define LL_GET_HEADER(data_ptr) ((ll__header *)(data_ptr) - 1)
#define LL_GET_DATA(header_ptr) ((char *)((ll__header *)(header_ptr) + 1))

ll__header *ll__grow(ll__header *header, size_t node_size);
void *ll__pushfront(void *ptr, size_t node_size, size_t prev_offset, size_t next_offset);
#define llpushfront(ll) \
	((ll) = ll__pushfront((ll),sizeof(*(ll)), (char *)&(ll)->prev - (char *)(ll), (char *)&(ll)->next - (char *)(ll)))

void *ll__pushback(void *ptr, size_t node_size, size_t prev_offset, size_t next_offset);
#define llpushback(ll) \
	((ll) = ll__pushback((ll),sizeof(*(ll)), (char *)&(ll)->prev - (char *)(ll), (char *)&(ll)->next - (char *)(ll)))

size_t llcount(void *ll);
ptrdiff_t llback(void *ll);
ptrdiff_t llfront(void *ll);
void llfree(void *ll);

void ll__movefront(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t index);
#define llmovefront(ll,index) \
	ll__movefront((ll),sizeof(*(ll)), (char *)&(ll)->prev - (char *)(ll), (char *)&(ll)->next - (char *)(ll),(index))

void ll__moveback(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t index);
#define llmoveback(ll,index) \
	ll__moveback((ll),sizeof(*(ll)), (char *)&(ll)->prev - (char *)(ll), (char *)&(ll)->next - (char *)(ll),(index))

void ll__delete(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t index);
#define lldelete(ll,index) \
	ll__delete((ll),sizeof(*(ll)), (char *)&(ll)->prev - (char *)(ll), (char *)&(ll)->next - (char *)(ll),(index))

void ll__exchange(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t idx_a, ptrdiff_t idx_b);
#define llexchange(ll, idx_a, idx_b) \
	ll__exchange((ll), sizeof(*(ll)), (char *)&(ll)->prev - (char *)(ll), (char *)&(ll)->next - (char *)(ll), (idx_a), (idx_b))

void *ll__insert_after(void *ptr, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t target_idx);
#define llinsertafter(ll, target_idx) \
	((ll) = ll__insert_after((ll), sizeof(*(ll)), (char *)&(ll)->prev - (char *)(ll), (char *)&(ll)->next - (char *)(ll), (target_idx)))

void *ll__insert_before(void *ptr, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t target_idx);
#define llinsertbefore(ll, target_idx) \
		((ll) = ll__insert_before((ll), sizeof(*(ll)), (char *)&(ll)->prev - (char *)(ll), (char *)&(ll)->next - (char *)(ll), (target_idx)))

void ll__clear(void *ll);
#define llclear(ll) ll__clear(ll)

ptrdiff_t ll__find(void *ll, size_t node_size, size_t next_offset, int (*cmp_func)(void *, void *), void *user_data);
#define llfind(ll, cmp_func, user_data) \
	ll__find((ll), sizeof(*(ll)), (char *)&(ll)->next - (char *)(ll), (cmp_func), (user_data))

void ll__dump(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, char *name);
#define lldump(ll) \
	ll__dump((ll),sizeof(*(ll)),(char *)&(ll)->prev - (char *)(ll), (char *)&(ll)->next - (char *)(ll), #ll)


#define ll__foreach(pos, ll, node_size, next_offset) \
	for(ptrdiff_t pos = llfront(ll); pos != -1; pos = *(ptrdiff_t *)((char *)(ll) + (node_size) * (pos) + (next_offset)))

#define llforeach(pos,ll) \
	ll__foreach(pos,ll,sizeof(*(ll)),(char *)&(ll)->next - (char *)(ll))

#define llforeach_safe(pos, nxt_pos, ll) \
	for(ptrdiff_t pos = llfront(ll), nxt_pos = (pos >= 0 ? *((ptrdiff_t *)((char *)(ll) + sizeof(*(ll)) * pos + ((char *)&(ll)->next - (char *)(ll)))) : -1); \
		pos != -1; \
		pos = nxt_pos, nxt_pos = (pos >= 0 ? *((ptrdiff_t *)((char *)(ll) + sizeof(*(ll)) * pos + ((char *)&(ll)->next - (char *)(ll)))) : -1))

#define ll__foreach_rev(pos, ll, node_size, prev_offset) \
	for(ptrdiff_t pos = llback(ll); pos != -1; pos = *(ptrdiff_t *)((char *)(ll) + (node_size) * (pos) + (prev_offset)))

#define llforeach_rev(pos,ll) \
	ll__foreach_rev(pos,ll,sizeof(*(ll)),(char *)&(ll)->prev - (char *)(ll))

#define llpopfront(ll) (llfront(ll) >= 0 ? (lldelete(ll, llfront(ll)), 1) : 0)
#define llpopback(ll)  (llback(ll) >= 0 ? (lldelete(ll, llback(ll)), 1) : 0)

#endif


#endif // LL_H_

#ifdef LL_IMPLEMENTATION

ll__header *ll__grow(ll__header *header, size_t node_size)
{
	if(header == NULL){
		header = LL_REALLOC(NULL,sizeof(ll__header) + LL_INIT_CAP * node_size);
		LL_ASSERT(header);
		header->count = 0;
		header->capacity = LL_INIT_CAP;
		header->front = -1;
		header->back = -1;
	} else {
		header->capacity *= 2;
		header = LL_REALLOC(header, sizeof(ll__header) + header->capacity * node_size);
	}
	return header;
}

void *ll__pushfront(void *ptr, size_t node_size, size_t prev_offset, size_t next_offset)
{
	ll__header *header = NULL;

	if(ptr == NULL){
		header = ll__grow(NULL,node_size);
	} else {
		header = LL_GET_HEADER(ptr);
		if(header->count >= header->capacity){
			header = ll__grow(header,node_size);
		}
	}

	char *nodes = LL_GET_DATA(header);
	#define DEREF(index, offset) *(ptrdiff_t *) (nodes + index * node_size + offset)

	ptrdiff_t node_index = header->count++;
	DEREF(node_index, prev_offset) = -1;

	if(node_index == 0){
		DEREF(node_index, next_offset) = -1;
		header->back  = node_index;
	} else {
		DEREF(node_index, next_offset) = header->front;
		DEREF(header->front, prev_offset) = node_index;
	}
	header->front = node_index;

	#undef DEREF

	return nodes;
}

void *ll__pushback(void *ptr, size_t node_size, size_t prev_offset, size_t next_offset)
{
	ll__header *header = NULL;

	if(ptr == NULL){
		header = ll__grow(NULL,node_size);
	} else {
		header = LL_GET_HEADER(ptr);
		if(header->count >= header->capacity){
			header = ll__grow(header,node_size);
		}
	}

	char *nodes = LL_GET_DATA(header);
	#define DEREF(index, offset) *(ptrdiff_t *) (nodes + index * node_size + offset)

	ptrdiff_t node_index = header->count++;
	DEREF(node_index, next_offset) = -1;

	if(node_index == 0){
		DEREF(node_index, prev_offset) = -1;
		header->front = node_index;
	} else {
		DEREF(node_index, prev_offset) = header->back;
		if(header->back >= 0) {
			DEREF(header->back,next_offset) = node_index;
		}
	}
	header->back  = node_index;
	#undef DEREF
	return nodes;
}

size_t llcount(void *ll)
{
	return ll ? (LL_GET_HEADER(ll))->count : 0;
}

ptrdiff_t  llback(void *ll)
{
	return ll ? (LL_GET_HEADER(ll))->back : -1;
}

ptrdiff_t llfront(void *ll)
{
	return ll ? (LL_GET_HEADER(ll))->front : -1;
}

void llfree(void *ll)
{
	if(ll) LL_FREE(LL_GET_HEADER(ll));
}

void ll__movefront(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t index)
{
	LL_ASSERT(ll != NULL && index >= 0);
	ll__header *header = LL_GET_HEADER(ll);
	LL_ASSERT(index < (ptrdiff_t)header->count);
					
	if(header->front == index) return;

	char *nodes = (char *)ll;
	#define DEREF(idx, off) *(ptrdiff_t *)(nodes + node_size * (idx) + (off))

	ptrdiff_t prv = DEREF(index, prev_offset);
	ptrdiff_t nxt = DEREF(index, next_offset);

	// del
	if(prv >= 0) DEREF(prv, next_offset) = nxt;
	else header->front = nxt;

	if(nxt >= 0) DEREF(nxt, prev_offset) = prv;
	else header->back = prv;

	// add to front
	DEREF(index, prev_offset) = -1;
	DEREF(index, next_offset) = header->front;
															
	if(header->front >= 0) DEREF(header->front, prev_offset) = index;
	header->front = index;
	#undef DEREF
}

void ll__moveback(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t index)
{
	LL_ASSERT(ll != NULL && index >= 0);
	ll__header *header = LL_GET_HEADER(ll);
	LL_ASSERT(index < (ptrdiff_t)header->count);
					
	if(header->back == index) return;

	char *nodes = (char *)ll;
	#define DEREF(idx, off) *(ptrdiff_t *)(nodes + node_size * (idx) + (off))

	ptrdiff_t prv = DEREF(index, prev_offset);
	ptrdiff_t nxt = DEREF(index, next_offset);

	if(prv >= 0) DEREF(prv, next_offset) = nxt;
	else header->front = nxt;

	if(nxt >= 0) DEREF(nxt, prev_offset) = prv;
	else header->back = prv;

	DEREF(index, prev_offset) = header->back;
	DEREF(index, next_offset) = -1;
															
	if(header->back >= 0) {
		DEREF(header->back, next_offset) = index;
	} else {
		header->front = index;
	}
	header->back = index;
	#undef DEREF
}

void ll__delete(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t index)
{
	LL_ASSERT(ll != NULL);
	ll__header *header = LL_GET_HEADER(ll);
	char * nodes = (char *)ll;

	LL_ASSERT(index >= 0 && index < (ptrdiff_t)header->count);

	#define DEREF(idx, off) *(ptrdiff_t *)(nodes + node_size * (idx) + (off))

	ptrdiff_t prv = DEREF(index, prev_offset);
	ptrdiff_t nxt = DEREF(index, next_offset);

	//del
	if(prv >= 0) DEREF(prv, next_offset) = nxt;
	else header->front = nxt; 

	if(nxt >= 0) DEREF(nxt, prev_offset) = prv;
	else header->back = prv;

	// move last node to deleted node space.
	ptrdiff_t last_idx = header->count - 1;
	if(last_idx != index){
		memcpy(nodes + index * node_size, nodes + last_idx * node_size, node_size);

		ptrdiff_t moved_prv = DEREF(index, prev_offset);
		ptrdiff_t moved_nxt = DEREF(index, next_offset);
																						
		if(moved_prv >= 0) DEREF(moved_prv, next_offset) = index;
		if(moved_nxt >= 0) DEREF(moved_nxt, prev_offset) = index;

		if(last_idx == header->front) header->front = index;
		if(last_idx == header->back)  header->back  = index;
	}

	header->count -= 1;
	#undef DEREF
}

void ll__exchange(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t idx_a, ptrdiff_t idx_b)
{
	LL_ASSERT(ll != NULL && idx_a >= 0 && idx_b >= 0);
	if (idx_a == idx_b) return;

	ll__header *header = LL_GET_HEADER(ll);
	char *nodes = (char *)ll;
	#define DEREF(idx, off) *(ptrdiff_t *)(nodes + node_size * (idx) + (off))

	ptrdiff_t a_prv = DEREF(idx_a, prev_offset);
	ptrdiff_t a_nxt = DEREF(idx_a, next_offset);
	ptrdiff_t b_prv = DEREF(idx_b, prev_offset);
	ptrdiff_t b_nxt = DEREF(idx_b, next_offset);

	if (a_nxt == idx_b) {
		DEREF(idx_a, next_offset) = b_nxt;
		DEREF(idx_a, prev_offset) = idx_b;
		DEREF(idx_b, next_offset) = idx_a;
		DEREF(idx_b, prev_offset) = a_prv;
		if (b_nxt >= 0) DEREF(b_nxt, prev_offset) = idx_a;
		if (a_prv >= 0) DEREF(a_prv, next_offset) = idx_b;
	} else if (b_nxt == idx_a) {
		DEREF(idx_b, next_offset) = a_nxt;
		DEREF(idx_b, prev_offset) = idx_a;
		DEREF(idx_a, next_offset) = idx_b;
		DEREF(idx_a, prev_offset) = b_prv;
		if (a_nxt >= 0) DEREF(a_nxt, prev_offset) = idx_b;
		if (b_prv >= 0) DEREF(b_prv, next_offset) = idx_a;
	} else {
		if (a_prv >= 0) DEREF(a_prv, next_offset) = idx_b;
		if (a_nxt >= 0) DEREF(a_nxt, prev_offset) = idx_b;
		if (b_prv >= 0) DEREF(b_prv, next_offset) = idx_a;
		if (b_nxt >= 0) DEREF(b_nxt, prev_offset) = idx_a;
		DEREF(idx_a, prev_offset) = b_prv;
		DEREF(idx_a, next_offset) = b_nxt;
		DEREF(idx_b, prev_offset) = a_prv;
		DEREF(idx_b, next_offset) = a_nxt;
	}

	if (header->front == idx_a) header->front = idx_b;
	else if (header->front == idx_b) header->front = idx_a;
	if (header->back == idx_a) header->back = idx_b;
	else if (header->back == idx_b) header->back = idx_a;
	#undef DEREF
}

void *ll__insert_after(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t target_idx)
{
	ll__header *header = LL_GET_HEADER(ll);
	if(header->count >= header->capacity){
		header = ll__grow(header, node_size);
	}

	char *nodes = (char *)ll;
	#define DEREF(idx, off) *(ptrdiff_t *)(nodes + node_size * (idx) + (off))

	ptrdiff_t new_idx = header->count++;
	ptrdiff_t old_nxt = DEREF(target_idx, next_offset);

	DEREF(new_idx, prev_offset) = target_idx;
	DEREF(new_idx, next_offset) = old_nxt;

	DEREF(target_idx, next_offset) = new_idx;
	if (old_nxt >= 0) {
		DEREF(old_nxt, prev_offset) = new_idx;
	} else {
		header->back = new_idx;  
	}
	#undef DEREF
	return nodes;
}

void *ll__insert_before(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, ptrdiff_t target_idx)
{
	ll__header *header = LL_GET_HEADER(ll);
	if(header->count >= header->capacity){
		header = ll__grow(header, node_size);
	}

	char *nodes = (char *)ll;
	#define DEREF(idx, off) *(ptrdiff_t *)(nodes + node_size * (idx) + (off))

	ptrdiff_t new_idx = header->count++;
	ptrdiff_t old_prv = DEREF(target_idx, prev_offset);

	DEREF(new_idx, next_offset) = target_idx;
	DEREF(new_idx, prev_offset) = old_prv;

	DEREF(target_idx, prev_offset) = new_idx;
	if (old_prv >= 0) {
		DEREF(old_prv, next_offset) = new_idx;
	} else {
		header->front = new_idx;
	}

	#undef DEREF
	return nodes;
}

void ll__clear(void *ll)
{
	if(ll) {
		ll__header *header = LL_GET_HEADER(ll);
		header->count = 0;
		header->front = -1;
		header->back = -1;
	}
}

ptrdiff_t ll__find(void *ll, size_t node_size, size_t next_offset, int (*cmp_func)(void *, void *), void *user_data)
{
	if(!ll) return -1;
	char *nodes = (char *)ll;
	ptrdiff_t curr = (LL_GET_HEADER(ll))->front;

	while(curr >= 0) {
		if(cmp_func(nodes + curr * node_size, user_data) == 0) {
			return curr;
		}
		curr = *(ptrdiff_t *)(nodes + curr * node_size + next_offset);
	}
	return -1;
}


void ll__dump(void *ll, size_t node_size, size_t prev_offset, size_t next_offset, char *name)
{
	LL_ASSERT(ll != NULL);
	ll__header *header = LL_GET_HEADER(ll);
	char * nodes = (char *)ll;

	#define DEREF(index, offset) *(ptrdiff_t *)(nodes + node_size * index + offset)
	LL_INFO("%s count[%zu] front[%ld] back [%ld]= [\n",name,header->count,(long)header->front,(long)header->back);

	ptrdiff_t apos = header->front;
	while(apos >= 0){
		ptrdiff_t prv = DEREF(apos,prev_offset);
		ptrdiff_t nxt = DEREF(apos,next_offset);
		LL_INFO(" prev[%ld] <- array_id[%ld] -> next[%ld] \n",(long)prv,(long)apos,(long)nxt);
		apos =  DEREF(apos,next_offset);
	}

	LL_INFO("]\n");

	#undef DEREF
}

#endif // LL_IMPLEMENTATION

