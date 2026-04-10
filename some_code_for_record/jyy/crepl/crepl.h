/*************************************************************************
	> File Name: crepl.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 06 Mar 2026 10:52:49 AM CST
 ************************************************************************/

#ifndef __CREPL_H__
#define __CREPL_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/stat.h>

#define NOB_IMPLEMENTATION
#include "nob.h"
#undef NOB_IMPLEMENTATION

char *path="/tmp/crepl/";
char *src_file="/tmp/crepl/hello.c";
char *src_file_tmp="/tmp/crepl/hello_tmp.c";
char *so_file="/tmp/crepl/libcrepl.so";

int input_cmd(const char * expr);
int build_so(void);
void call_jit_function(void);

#endif

#ifdef CREPL_IMPLEMENTATION
static uint32_t calculate_file_hash(const char * path)
{
	FILE *f = fopen(path,"rb");
	if(!f) return 0;
	uint32_t hash = 5381;
	int c;
	while ((c=fgetc(f)) != EOF){
		hash = ((hash << 5) + hash) + c;
	}
	fclose(f);
	return hash;
}


int input_cmd(const char * expr)
{
	cmd_t cmd_cp = {0};
	cmd_append(cmd_cp,"cp");
	cmd_append(cmd_cp,src_file);
	cmd_append(cmd_cp,src_file_tmp);
	BUILD_PROJECT(cmd_cp);

	FILE *f = fopen(src_file_tmp,"r+");
	if(!f) return -1;

	fseek(f,0,SEEK_END);
	
	fprintf(f,"// This is a header comment.\n");


	fprintf(f,"%s\n",expr);
	fclose(f);
}


int build_so(void)
{
	uint32_t before_hash = calculate_file_hash(so_file);

	cmd_t cmd = {0};
	cmd_append(cmd,"gcc");
	cmd_append(cmd,"-fPIC");
	cmd_append(cmd,"-shared");
	cmd_append(cmd,"-o");
	cmd_append(cmd,so_file);
	cmd_append(cmd,src_file_tmp);
	BUILD_PROJECT(cmd);

	uint32_t after_hash = calculate_file_hash(so_file);
	if(after_hash != before_hash){
		cmd_t cmd_mv = {0};
		cmd_append(cmd_mv,"mv");
		cmd_append(cmd_mv,src_file_tmp);
		cmd_append(cmd_mv,src_file);
		BUILD_PROJECT(cmd_mv);
	}else{
		cmd_t cmd_rm = {0};
		cmd_append(cmd_rm,"rm");
		cmd_append(cmd_rm,"-rf");
		cmd_append(cmd_rm,src_file_tmp);
		BUILD_PROJECT(cmd_rm);
	}

	return 0;
}

void call_jit_function()
{
	void *handle = dlopen(so_file,RTLD_LAZY);
	if(!handle){
		fprintf(stderr,"加载失败: %s\n",dlerror());
		return;
	}

	dlerror();
	void (*my_func)(void);
	my_func = (void (*)(void))dlsym(handle,"hello");

	char *error = dlerror();
	if(error != NULL){
		fprintf(stderr,"找不到符号: %s\n",dlerror());
		dlclose(handle);
		return;
	}

	printf("JIT 执行结果: \n");
	my_func();

	dlclose(handle);
}

#endif
