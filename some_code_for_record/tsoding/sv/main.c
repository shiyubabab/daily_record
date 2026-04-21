/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 21 Apr 2026 01:57:17 PM CST
 ************************************************************************/

#include "stdio.h"

#define SV_IMPLEMENTATION
#include "sv.h"

char *cstr = "                   my name is mlxh.                         ";
char *cstr_start = "my";
char *cstr_end = "mlxh.";
char *cstr_num = "478329";

int main(void){
	String_View sv = sv_from_cstr(cstr);
	SV_DUMP(sv);

	sv = sv_trim(sv);
	SV_DUMP(sv);

	String_View sv_start = sv_from_cstr(cstr_start);
	if(sv_starts_with(sv,sv_start)){
		printf("it is start with %s\n",cstr_start);
	}

	String_View sv_end = sv_from_cstr(cstr_end);
	if(sv_ends_with(sv,sv_end)){
		printf("it is end with %s\n",cstr_end);
	}

	String_View sv_left = sv_chop_left(&sv,1);
	SV_DUMP(sv);
	SV_DUMP(sv_left);


	String_View sv_right = sv_chop_right(&sv,2);
	SV_DUMP(sv);
	SV_DUMP(sv_right);
	size_t index = 0;
	if(sv_index_of(sv,'s',&index)){
		printf("Get it and the index is %zu\n",index);
	}

	String_View result  = sv_from_part(sv.data,1);
	if(sv_try_chop_by_delim(&sv,'m', &result)){
		SV_DUMP(sv);
		SV_DUMP(result);
	}
	SV_DUMP(sv_chop_by_delim(&sv,'b'));
	SV_DUMP(sv);
		
	String_View sv_num = sv_from_cstr(cstr_num);
	uint64_t num = sv_to_u64(sv_num);
	printf("num is %ld\n",num);
	
	return 0;
}
