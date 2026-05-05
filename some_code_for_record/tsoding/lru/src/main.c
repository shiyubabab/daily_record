/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 22 Apr 2026 09:56:14 AM CST
 ************************************************************************/

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define SV_IMPLEMENTATION
#include "sv.h"

#define LL_IMPLEMENTATION
#include "ll.h"

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

char *path = "../shakespeare.txt";


typedef struct {
	char data[32];
} Word;

/************
 *  llcache
 ***********/

#ifndef LL_IMPLEMENTATION
bool cache_get(Word word, size_t *freq){
	(void) word;
	(void) freq;
	return false;
}

void cache_put(Word word, size_t freq){
	(void) word;
	(void) freq;
}
#else
#define CACHE_CAP 1024

typedef struct {
	ptrdiff_t prev;
	ptrdiff_t next;
	Word key;
	size_t value;
} Node;

Node *queue = NULL;
bool cache_get(Word key, size_t *value)
{
	llforeach(i,queue){
		if(strcmp(queue[i].key.data,key.data) == 0){
			if(value) *value = queue[i].value;
			llmovefront(queue,i);
			return true;
		}
	}
	return false;
}

void cache_put(Word key, size_t value)
{
	if(llcount(queue) < CACHE_CAP){
		llpushfront(queue);
	} else {
		llmovefront(queue,llback(queue));
	}

	queue[llfront(queue)].key = key;
	queue[llfront(queue)].value = value;
}

void cache_cleanup(void)
{
	llfree(queue);
}

#endif


// end of llcache

Word sv_as_word(String_View sv)
{
	Word result = {0};
	assert(sv.count < sizeof(result.data));
	memcpy(result.data,sv.data,sv.count);
	return result;
}

Word word_norm(Word word)
{
	Word result = {0};
	char *in  = word.data;
	char *out = result.data;
	while(*in){
		char c = *in++;
		if(isalnum(c)){
			*out++ = toupper(c);
		}
	}
	return result;
}

size_t word_count(String_View content, Word needle)
{
	size_t count = 0;
	while(content.count > 0){
		String_View sv_line = sv_chop_by_delim(&content,'\n');
		while(sv_line.count > 0){
			String_View sv_word = sv_chop_by_delim(&sv_line,' ');
			if(sv_word.count > 0){
				Word word = word_norm(sv_as_word(sv_word));
				if(strcmp(word.data,needle.data) == 0){
					count++;
				}
			}
		}
	}
	return count;
}

int main(void)
{
	int fd;
	fd = open(path,O_RDONLY);
	if(fd == -1)
		handle_error("open");

	struct stat sb;
	if(fstat(fd,&sb) == -1)
		handle_error("fstat");

	size_t content_size = sb.st_size;

	char *content_data = mmap(NULL,content_size,PROT_READ,MAP_PRIVATE,fd,0);
	if(content_data == MAP_FAILED)
		handle_error("mmap");

	String_View content = sv_from_part(content_data,content_size);

	while(content.count > 0){
		String_View line = sv_chop_by_delim(&content,'\n');

		while(line.count > 0){
			String_View sv_word = sv_trim(sv_chop_by_delim(&line,' '));
			if(sv_word.count > 0){
				Word needle = word_norm(sv_as_word(sv_word));
				//printf("(%s)\n",word.data);
				size_t frep = 0;
				if(!cache_get(needle,&frep)){
					frep = word_count(sv_from_part(content_data,content_size),needle);
					cache_put(needle,frep);
				}
				printf("%s(%zu) ",needle.data,frep);
			}
		}
		printf("\n");
	}

	munmap(content_data,content_size);
	close(fd);

	return 0;
}

