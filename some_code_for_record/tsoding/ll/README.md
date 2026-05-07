# ll.h - 一个基于数组的单头文件双向链表库

`ll.h` 是一个受 `stb` 库启发的、基于 **类型安全** 且 **内存友好** 的双向链表实现。与传统的基于指针的链表不同，它将所有节点存储在一个连续的数组中，通过索引（`ptrdiff_t`）而非指针来链接节点。

### 核心特性
* **内存紧凑**：所有节点都在一个连续的堆空间内，利于缓存（Cache-friendly）。
* **无感表头**：使用类似 `stb_ds` 的技巧，表头隐藏在返回指针的前方。
* **高度灵活**：支持自定义内存管理（`realloc`/`free`）。
* **缓存行对齐**：表头自动按 64 字节（64位系统）或 32 字节（32位系统）对齐。
* **高性能删除**：删除节点时，通过将末尾节点移动到空隙处，保证数组空间始终是连续的。

---

## 1. 快速开始

### 基础集成
在**一个** `.c` 或 `.cpp` 文件中定义 `LL_IMPLEMENTATION` 宏以包含实现代码：

```c
#define LL_IMPLEMENTATION
#include "ll.h"
```

### 定义节点结构
你的结构体**必须**包含 `prev` 和 `next` 成员（类型为 `ptrdiff_t`）：

```c
typedef struct {
    int value;
    ptrdiff_t prev; // 必须
    ptrdiff_t next; // 必须
} MyNode;

MyNode *list = NULL; // 初始必须为 NULL
```

---

## 2. API 参考

### 内存管理
* **`llfree(ll)`**
    释放链表占用的所有内存。
* **`llclear(ll)`**
    清空链表逻辑，但不释放内存。
* **`llcount(ll)`**
    返回链表中的节点总数。

### 插入操作
* **`llpushfront(ll)`**
    在链表头部插入一个新节点，返回更新后的指针。
* **`llpushback(ll)`**
    在链表尾部插入一个新节点，返回更新后的指针。
* **`llinsertafter(ll, target_idx)`**
    在索引为 `target_idx` 的节点之后插入新节点。
* **`llinsertbefore(ll, target_idx)`**
    在索引为 `target_idx` 的节点之前插入新节点。

### 删除与移动
* **`lldelete(ll, index)`**
    删除索引为 `index` 的节点。内部会将数组末尾节点移至此索引以填补空隙。
* **`llpopfront(ll)`** / **`llpopback(ll)`**
    弹出（删除）头部或尾部节点。
* **`llmovefront(ll, index)`**
    将索引为 `index` 的节点移至链表逻辑头部。
* **`llmoveback(ll, index)`**
    将索引为 `index` 的节点移至链表逻辑尾部。
* **`llexchange(ll, idx_a, idx_b)`**
    交换两个节点的逻辑位置。

### 遍历与查询
* **`llfront(ll)`** / **`llback(ll)`**
    获取逻辑头部/尾部节点的索引。若为空则返回 `-1`。
* **`llforeach(pos, ll)`**
    从前向后遍历。`pos` 为当前节点的 `ptrdiff_t` 索引。
* **`llforeach_rev(pos, ll)`**
    从后向前遍历。
* **`llforeach_safe(pos, nxt_pos, ll)`**
    安全遍历（允许在循环中删除当前节点 `pos`）。
* **`llfind(ll, cmp_func, user_data)`**
    查找节点。`cmp_func` 需符合 `int (*)(void *node, void *user_data)` 签名，匹配返回索引。

### 调试
* **`lldump(ll)`**
    向控制台打印链表的完整结构信息（索引链接关系）。

---

## 3. 使用示例

```c
#define LL_IMPLEMENTATION
#include "ll.h"

int main() {
    struct { int val; ptrdiff_t prev, next; } *list = NULL;

    // 添加数据
    list = llpushback(list);
    list[llback(list)].val = 10;
    
    list = llpushback(list);
    list[llback(list)].val = 20;

    // 遍历
    llforeach(i, list) {
        printf("Node %ld: %d\n", i, list[i].val);
    }

    // 删除
    lldelete(list, 0);

    llfree(list);
    return 0;
}
```

---

## 4. 配置宏
你可以在包含 `ll.h` 之前定义以下宏来自定义行为：

| 宏名称 | 默认值 | 说明 |
| :--- | :--- | :--- |
| `LL_INIT_CAP` | `32` | 初始分配的节点容量 |
| `LL_ASSERT` | `assert` | 断言函数 |
| `LL_REALLOC` | `realloc` | 内存重新分配函数 |
| `LL_FREE` | `free` | 内存释放函数 |
| `LL_INFO` | `printf` | 调试信息输出函数 |

---

## 5. 注意事项
1.  **索引失效**：调用 `lldelete` 后，由于内部会移动数组末尾节点，原本指向**数组末尾**的索引可能会改变。
2.  **指针重分配**：`llpush` 和 `llinsert` 宏可能会触发 `realloc`，因此必须接收返回值：`list = llpushback(list);`。
3.  **对齐**：该库利用了 `__attribute__((aligned))`，确保在嵌入式或高性能场景下的存取效率。