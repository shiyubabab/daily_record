/*************************************************************************
	> File Name: process.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 02 Mar 2026 10:55:12 AM CST
 ************************************************************************/

#ifndef __PROCESS_H__
#define __PROCESS_H__

#define MAX_PROCESSES 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

typedef struct Process {
	int pid;
	int ppid;
	char name[256];
	struct Process *children[128];
	int child_count;
} Process;

typedef struct Process_vector {
	Process ** items;
	size_t count;
	size_t capacity;
} Process_vector;

#define proc_append(proc,p)			\
	do{								\
		if((proc).count >= (proc).capacity){												\
			size_t tmp_capacity = (proc).capacity == 0 ? 255 : 2*(proc).capacity;			\
			Process **tmp_items = realloc((proc).items,tmp_capacity*sizeof(Process *));		\
			if(tmp_items != NULL){															\
				(proc).capacity = tmp_capacity;												\
				(proc).items = tmp_items;													\
			}else{																			\
				perror("ERROR: realloc fail.");												\
				exit(EXIT_FAILURE);															\
			}																				\
		}																					\
		(proc).items[(proc).count++] = (p);													\
	}while(0)
#endif

#ifdef MAKE_SOME_NOISE
void load_processes(Process_vector *proc)
{
	DIR *dir = opendir("/proc");
	struct dirent *entry;
	while((entry = readdir(dir)) != NULL){
		if(isdigit(entry->d_name[0])){
			char path[512];
			snprintf(path,sizeof(path),"/proc/%s/stat",entry->d_name);

			FILE *f = fopen(path,"r");
			if(f){
				Process *p = malloc(sizeof(Process));
				fscanf(f,"%d (%[^)]) %*c %d",&p->pid,p->name,&p->ppid);
				p->child_count = 0;
				proc_append(*proc,p);
				fclose(f);
			}
		}
	}
	closedir(dir);
}

void print_tree(Process *p, int level) 
{
	for(int i = 0;i<level;i++) printf("  |");
	printf("  |-- %s [%d]\n",p->name,p->pid);

	for(int i = 0;i<p->child_count;i++){
		print_tree(p->children[i],level + 1);
	}
}

void parent_construct(Process_vector proc)
{
	for(int i = 0;i<proc.count;i++){
		for(int j = 0;j<proc.count;j++){
			if(proc.items[j]->ppid == proc.items[i]->pid){
				proc.items[i]->children[proc.items[i]->child_count++] = proc.items[j];
			}
		}
	}
}

#endif
