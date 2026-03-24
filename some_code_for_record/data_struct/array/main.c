/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 13 Mar 2026 02:03:13 PM CST
 ************************************************************************/

#define ML_ARRAY_IMPLEMENTATION
#include "ml_array.h"
#undef ML_ARRAY_IMPLEMENTATION

int main(void){
	ml_array_t array;
	ml_array_init(&array,100,sizeof(int));
	for(int i = 0;i<10;i++) ml_array_push_back(&array,(void *)&i);

	for(int i = 0;i<array.size;i++) printf("array [%d] \n",*(int *)ml_array_at(&array,i));
	printf("\n");
	ml_array_erase(&array,2,5);
	for(int i = 0;i<array.size;i++) printf("array [%d] \n",*(int *)ml_array_at(&array,i));
	printf("\n");

	ml_array_t array_other;
	ml_array_init(&array_other,100,sizeof(int));

	ml_array_copy(&array_other,&array);
	for(int i = 0;i<array_other.size;i++) printf("array [%d] \n",*(int *)ml_array_at(&array_other,i));
	printf("\n");

	ml_array_remove(&array_other,0);
	for(int i = 0;i<array_other.size;i++) printf("array [%d] \n",*(int *)ml_array_at(&array_other,i));

	return 0;
}
