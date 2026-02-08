/*************************************************************************
	> File Name: options.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 08 Feb 2026 01:39:42 PM CST
 ************************************************************************/

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#define OP_LIST(X)	\
	X(OP_DEBUG,0,debug,"Just for debug.")		\
	X(OP_BUILD,1,build,"Buid the project.")	

#define	ENUM_GEN(name,num,cmd,dsc) name = num,
typedef enum {
	OP_LIST(ENUM_GEN)
	OP_ENUM_MAX
} op_list_t;
#undef ENUM_GEN

#define OP_GEN(name,num,cmd,dsc) \
	int op_##name##_##num##_do(void);		\
	exit int op_##name##_##num##_do(void);
OP_LIST(OP_GEN)
#undef OP_GEN

#endif
