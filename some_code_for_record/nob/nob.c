/*************************************************************************
	> File Name: nob.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sat 07 Feb 2026 11:45:57 PM CST
 ************************************************************************/

#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc,char* argv[])
{
	cmd_t cmd = {0};

	cmd_append(cmd,"gcc");
	if(argc>1){
		for(int i = 1;i < argc;i++) cmd_append(cmd,argv[i]);
	}else{
		cmd_append(cmd,"main.c");
	}
	cmd_append(cmd,"-o");
	cmd_append(cmd,"go");

	BUILD_PROJECT(cmd);
	return 0;
}

