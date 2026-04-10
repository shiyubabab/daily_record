/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 05 Mar 2026 04:16:53 PM CST
 ************************************************************************/

#define CREPL_IMPLEMENTATION
#include "crepl.h"
#undef CREPL_IMPLEMENTATION

#define INPUT_PRO_IMPLEMENTATION
#include "input_processing.h"
#undef INPUT_PRO_IMPLEMENTATION

#define SRT_MAX_LEN 1024
char line[SRT_MAX_LEN];

int main(void)
{
	cmd_t cmds = {0};
	while(1){
		printf("crepl> ");
		fflush(stdout);

		if(!fgets(line,sizeof(line),stdin)){
			break;
		}
		get_cmd(line,SRT_MAX_LEN,&cmds);
		printf("%s",cmds.items[cmds.count]);

		

		call_jit_function();

		//TODO implemented.
		input_cmd(line);
		build_so();
		

		printf("Got %zu chars.\n",strlen(line));
	}
}
