# LibLL {#mainpage}

**Embedded circular doubly-linked lists for C (intrusive)** — no heap inside
the library, static or dynamic nodes, safe iteration while modifying.

Built for bare-metal and systems code where objects already exist (pools, BSS,
arenas) and the list only wires pointers.

## Quick start {#quickstart}

```c
#include "LibLL.h"

typedef struct {
    NODE node;  /* first field */
    int  id;
} Task;

mCREATE_LIST(ready, ready_tmp);

Task t1 = {0};
t1.id = 1;
mINIT_NODE(&t1.node);

LL_Connect(&ready, &t1.node, &ready_tmp);
LL_Disconnect(&ready, &t1.node, &ready_tmp);
```

## Rules of thumb

1. Put `NODE` first in your struct (or use `offsetof` / `container_of`).
2. Call `mINIT_NODE` before the first `LL_Connect` / `LL_Insert`.
3. Give **each** list its own `tmp` cursor (`mCREATE_LIST` does that).
4. `Connect` ≠ allocate. `Disconnect` ≠ `free`.

## Pitfalls {#pitfalls}

- Already linked nodes are rejected by `LL_Connect` / `LL_Insert` — node must be isolated.
- Sharing one `tmp` across lists corrupts safe iteration.
- `LL_Clear_List` calls `free` on every node — only for heap-allocated nodes.
- Not thread-safe — synchronize yourself for ISR / multi-thread use.
- License is GPL-3.0.

## API groups

- @ref libll_types "Types and macros"
- @ref libll_mutate "Connect / insert / disconnect"
- @ref libll_move "Move between lists"
- @ref libll_iterate "Find and iterate"
- @ref libll_memory "Clear (heap nodes)"

See also the bilingual README on GitHub for FetchContent and examples.
