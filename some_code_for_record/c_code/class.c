/*************************************************************************
	> File Name: class.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sat 20 Sep 2025 02:22:26 PM CST
 ************************************************************************/

#include<stdio.h>

typedef struct{
	char *name;
	int number;
	int		 a;
	int		 b;
}data_t;

typedef int (*function_return_int_t) (void *self);
typedef void (*function_t) (void *self);

typedef struct{
	function_return_int_t add;
	function_return_int_t sub;
	function_t			print;
} ops_t;

typedef struct{
	data_t Data;
	ops_t   Ops;
	void (*init) (void *self, data_t *data, ops_t *ops);
}class_t;

/*************************************************************************
  > function setting ops
*************************************************************************/

int my_add(void *self){
	class_t *my_self = (class_t *)self;
	return my_self->Data.a + my_self->Data.b;
}

int my_sub(void *self){
	class_t *my_self = (class_t *)self;
	return my_self->Data.a - my_self->Data.b;
}

void my_print(void *self){
	class_t *my_self = (class_t *)self;
	printf("I am %s and ID is %d \n",my_self->Data.name,my_self->Data.number);
}

void init_class_instence(void *self, data_t *data , ops_t *ops){
	class_t *my_self = (class_t *)self;
	my_self->Data = *data;
	my_self->Ops  =  *ops;
}

int main(void){
	data_t data = {
		.name   = "mlxh",
		.number =	   0,
		.a		=	  19,
		.b		=	  10,
	};
	ops_t ops = {
		.add	=	my_add,
		.sub	=	my_sub,
		.print	= my_print,
	};
	class_t mlxh;
	mlxh.init = init_class_instence;

	mlxh.init((void*)&mlxh,&data,&ops);

	int c = mlxh.Ops.add((void*)&mlxh);
	int d = mlxh.Ops.sub((void*)&mlxh);
	printf("add result is %d and sub result is %d \n",c,d);
	mlxh.Ops.print((void*)&mlxh);

	return 0;
}
