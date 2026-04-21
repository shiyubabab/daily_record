/*************************************************************************
	> File Name: sv.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 21 Apr 2026 02:03:19 PM CST
 ************************************************************************/


#ifndef SV_H_
#define SV_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#ifndef SV_INFO
#include <stdio.h>
#define SV_INFO printf
#endif

typedef struct {
	size_t count;
	const char *data;
} String_View;

#define SV_Fmt "%.*s"
#define SV_Arg(sv) (int)(sv).count,(sv).data
/*
	USAGE:

 */

String_View sv_from_part(const char *cstr, size_t count);
#define SV(cstr_lit) sv_from_part(cstr_lit, sizeof(cstr_lit)-1)
#define SV_NULL sv_from_part(NULL,0)
String_View sv_from_cstr(const char *cstr);
String_View sv_trim_left(String_View sv);
String_View sv_trim_right(String_View sv);
String_View sv_trim(String_View sv);
String_View sv_chop_left(String_View *sv, size_t n);
String_View sv_chop_right(String_View *sv, size_t n);
bool sv_index_of(String_View sv, char c, size_t *index);
bool sv_try_chop_by_delim(String_View *sv, char delim, String_View *chunk);
String_View sv_chop_by_delim(String_View *sv, char delim);
bool sv_eq(String_View a, String_View b);
bool sv_starts_with(String_View sv, String_View expected_prefix);
bool sv_ends_with(String_View sv, String_View expected_prefix);
uint64_t sv_to_u64(String_View sv);

void sv_dump(String_View sv,char *name,size_t padding);
#define SV_DUMP(sv) sv_dump(sv,#sv,0)

#endif // SV_H_

#ifdef SV_IMPLEMENTATION

String_View sv_from_part(const char *data, size_t count)
{
	String_View sv;
	sv.count = count;
	sv.data  = data;
	return sv;
}

String_View sv_from_cstr(const char *cstr)
{
	return sv_from_part(cstr,strlen(cstr));
}

String_View sv_trim_left(String_View sv)
{
	size_t i = 0;
	while(i < sv.count && isspace(sv.data[i])){
		i += 1;
	}
	return sv_from_part(sv.data+i,sv.count - i);
}

String_View sv_trim_right(String_View sv)
{
	size_t i = 0;
	while(i < sv.count && isspace(sv.data[sv.count - 1 - i])){
		i += 1;
	}
	return sv_from_part(sv.data,sv.count - i);
}

String_View sv_trim(String_View sv)
{
	return sv_trim_right(sv_trim_left(sv));
}

String_View sv_chop_left(String_View *sv, size_t n)
{
	if(n > sv->count){
		n = sv->count;
	}
	String_View result = sv_from_part(sv->data,n);

	sv->data  += n;
	sv->count -= n;

	return result;
}

String_View sv_chop_right(String_View *sv, size_t n)
{
	if(n > sv->count){
		n = sv->count;
	}
	String_View result = sv_from_part(sv->data + sv->count - n,n);

	sv->count -= n;

	return result;
}

bool sv_index_of(String_View sv, char c, size_t *index)
{
	size_t i = 0;
	while(i<sv.count && c != sv.data[i]){
		i += 1;
	}

	if(i<sv.count){
		if(index){
			*index = i;
		}
		return true;
	}else{
		return false;
	}
}

bool sv_try_chop_by_delim(String_View *sv, char delim, String_View *chunk)
{
	size_t i = 0;
	while(i<sv->count && delim != sv->data[i]){
		i += 1;
	}

	String_View result = sv_from_part(sv->data,i);

	if(i < sv->count){
		sv->count -= i + 1;
		sv->data  += i + 1;
		if(chunk){
			*chunk = result;
		}
		return true;
	}
	return false;
}

String_View sv_chop_by_delim(String_View *sv, char delim)
{
	size_t i = 0;
	while(i<sv->count && delim != sv->data[i]){
		i += 1;
	}

	String_View result = sv_from_part(sv->data,i);

	if(i < sv->count){
		sv->count -= i + 1;
		sv->data  += i + 1;
	}else{
		sv->count -= i;
		sv->data  += i;
	}
	return result;
}

bool sv_starts_with(String_View sv, String_View expected_prefix)
{
	if(expected_prefix.count <= sv.count){
		String_View actual_prefix = sv_from_part(sv.data,expected_prefix.count);
		return sv_eq(expected_prefix,actual_prefix);
	}
	return false;
}

bool sv_ends_with(String_View sv, String_View expected_prefix)
{
	if(expected_prefix.count <= sv.count){
		String_View actual_prefix = sv_from_part(sv.data + sv.count - expected_prefix.count, expected_prefix.count);
		return sv_eq(expected_prefix,actual_prefix);
	}
	return false;
}

bool sv_eq(String_View a, String_View b)
{
	if(a.count != b.count){
		return false;
	} else {
		return memcmp(a.data,b.data,a.count) == 0;
	}
}

uint64_t sv_to_u64(String_View sv)
{
	uint64_t result = 0;

	for(size_t i = 0;i<sv.count && isdigit(sv.data[i]);++i){
		result = result * 10 + (uint64_t) sv.data[i] - '0';
	}
	return result;
}

String_View sv_chop_left_while(String_View *sv, bool (*predicate)(char x))
{
	size_t i = 0;
	while(i<sv->count && predicate(sv->data[i])){
		i += 1;
	}
	return sv_chop_left(sv,i);
}

String_View sv_take_left_while(String_View sv, bool (*predicate)(char x))
{
	size_t i = 0;
	while(i<sv.count && predicate(sv.data[i])){
		i += 1;
	}
	return sv_from_part(sv.data,i);
}

void sv_dump(String_View sv,char *name,size_t padding)
{
	SV_INFO("%*s%s = [\n",(int)padding,"",name);
	SV_INFO("%*s"SV_Fmt"\n",(int)padding+4,"",SV_Arg(sv));
	SV_INFO("%*s]\n",(int)padding,"");
}


#endif // SV_IMPLEMENTATION
