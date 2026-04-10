/*************************************************************************
	> File Name: input_processing.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 12 Mar 2026 04:06:18 PM CST
 ************************************************************************/

#ifndef __INPUT_PROCESSING_H__
#define __INPUT_PROCESSING_H__

#include "nob.h"

int get_cmd(const char *src,size_t max_len,cmd_t *cmds);

#endif

#ifdef INPUT_PRO_IMPLEMENTATION
int get_cmd(const char *src,size_t max_len,cmd_t *cmds)
{
	if(!src) return -1;

	const char *paren_pos = strchr(src,'(');
	if(!paren_pos) return -1;

	const char *end = paren_pos - 1;
	while(end > src && isspace((unsigned char)*end)) {
		end--;
	}

	const char *start = end;
	while(start > src && !isspace((unsigned char)*(start -1))){
		start--;
	}

	size_t len = end - start + 1;
	if(len >= max_len) len = max_len -1;

	char out_str[len+1];
	strncpy(out_str,start,len);
	out_str[len] = '\0';
	cmd_append(*cmds,out_str);
	return 0;
}

#endif
