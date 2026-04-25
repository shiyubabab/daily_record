/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 24 Apr 2026 03:21:22 PM CST
 ************************************************************************/

#include <stdio.h>
#define LL_IMPLEMENTATION
#include "ll.h"
#include <string.h>

typedef struct {
    char data[32];
} Word;

typedef struct {
    ptrdiff_t prev;
    ptrdiff_t next;
    Word value;
} mlxh_t;

// 查找回调：按字符串匹配
int cmp_word(void *node, void *user_data) {
    mlxh_t *m = (mlxh_t *)node;
    return strcmp(m->value.data, (char *)user_data);
}

int main(void) {
    mlxh_t *mlxh = NULL;

    printf("=== 1. 基础插入测试 (pushfront/pushback) ===\n");
    llpushback(mlxh);  strcpy(mlxh[llback(mlxh)].value.data, "Node_B");
    llpushfront(mlxh); strcpy(mlxh[llfront(mlxh)].value.data, "Node_A");
    llpushback(mlxh);  strcpy(mlxh[llback(mlxh)].value.data, "Node_C");
    // 逻辑预期: Node_A -> Node_B -> Node_C
    lldump(mlxh);

    printf("\n=== 2. 指定位置插入测试 (insertafter/insertbefore) ===\n");
    // 在 A 后面插入 A+
    ptrdiff_t idx_a = llfront(mlxh);
    llinsertafter(mlxh, idx_a);
    strcpy(mlxh[llcount(mlxh)-1].value.data, "Node_A+");
    
    // 在 C 前面插入 B+
    ptrdiff_t idx_c = llback(mlxh);
    llinsertbefore(mlxh, idx_c);
    strcpy(mlxh[llcount(mlxh)-1].value.data, "Node_B+");
    // 预期: A -> A+ -> B -> B+ -> C
    lldump(mlxh);

    printf("\n=== 3. 逻辑移动测试 (movefront/moveback) ===\n");
    llmovefront(mlxh, idx_c); // 把 C 移到最前
    llmoveback(mlxh, idx_a);  // 把 A 移到最后
    // 预期: C -> A+ -> B -> B+ -> A
    lldump(mlxh);

    printf("\n=== 4. 交换测试 (exchange) ===\n");
    ptrdiff_t head = llfront(mlxh);
    ptrdiff_t tail = llback(mlxh);
    llexchange(mlxh, head, tail); // 交换头尾
    // 预期: A -> A+ -> B -> B+ -> C
    lldump(mlxh);

    printf("\n=== 5. 查找与遍历测试 (find/foreach) ===\n");
    ptrdiff_t found = llfind(mlxh, cmp_word, "Node_B");
    printf("Find 'Node_B': index %ld\n", (long)found);
    
    printf("正向遍历: ");
    llforeach(i, mlxh) printf("[%s] ", mlxh[i].value.data);
    printf("\n反向遍历: ");
    llforeach_rev(i, mlxh) printf("[%s] ", mlxh[i].value.data);
    printf("\n");

    printf("\n=== 6. 删除与安全遍历测试 (delete/foreach_safe) ===\n");
    // 安全删除所有带 '+' 的节点
    llforeach_safe(i, nxt, mlxh) {
        if(strchr(mlxh[i].value.data, '+')) {
            printf("Deleting %s...\n", mlxh[i].value.data);
            lldelete(mlxh, i);
        }
    }
    // 预期: A -> B -> C
    lldump(mlxh);

    printf("\n=== 7. 队列操作测试 (popfront/popback) ===\n");
    llpopfront(mlxh); // 弹出 A
    llpopback(mlxh);  // 弹出 C
    // 预期: 仅剩 B
    lldump(mlxh);

    printf("\n=== 8. 清空与释放测试 (clear/free) ===\n");
    llclear(mlxh);
    printf("After clear: count = %zu\n", llcount(mlxh));
    llfree(mlxh);
    printf("Memory freed.\n");

    return 0;
}