/*************************************************************************
	> File Name: nob.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sat 07 Feb 2026 11:18:19 PM CST
 ************************************************************************/


#ifndef __NOB_H__
#define __NOB_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct _cmd_t{
	char ** items;
	size_t count;
	size_t capacity;
} cmd_t ;

#define cmd_append(cmd,s)														\
	do{																			\
		if((cmd).count >= (cmd).capacity){										\
			size_t tmp_capacity = (cmd).capacity == 0 ? 255:2*(cmd).capacity;	\
			char **tmp_items	= realloc((cmd).items,tmp_capacity*sizeof(char *));	\
			if(tmp_items != NULL){												\
				(cmd).capacity = tmp_capacity;									\
				(cmd).items	   = tmp_items;										\
			} else {															\
				perror("ERROR: realloc fail.");									\
				exit(EXIT_FAILURE);												\
			}																	\
		}																		\
		(cmd).items[(cmd).count++] = (s);											\
	}while(0)

#endif

#ifdef NOB_IMPLEMENTATION

#include <sys/wait.h>
#include <unistd.h>

int cmd_run(cmd_t cmd)
{
	cmd_append(cmd,NULL);

	pid_t cpid = fork();
	if(cpid == -1){
		perror("ERROR:fork");
		exit(EXIT_FAILURE);
	}

	if(cpid == 0){
		if(execvp(cmd.items[0],cmd.items)<0){
			perror("ERROR: execvp fail");
			exit(EXIT_FAILURE);
		}
	}else{
		int status;
		waitpid(cpid,&status,0);
		return WIFEXITED(status) && WEXITSTATUS(status) == 0;
	}
	return 0;

}

#define BUILD_PROJECT(cmd)					\
	do{										\
		printf("Building......\n");			\
		if(cmd_run(cmd)<0){					\
			perror("ERROR: cmd_run fail");	\
			exit(EXIT_FAILURE);				\
		}									\
	}while(0)

#endif
