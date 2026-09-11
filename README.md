# LibLL

[![CMakeGoogletest](https://github.com/OwlSurf/LibLL/actions/workflows/cmake.yml/badge.svg)](https://github.com/OwlSurf/LibLL/actions/workflows/cmake.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/owlsurf/libll/badge)](https://www.codefactor.io/repository/github/owlsurf/libll)

**Intrusive circular doubly-linked lists for C** — no heap inside the library, static or dynamic nodes, safe iteration while modifying.

Built for embedded / bare-metal and systems code where objects already exist (pools, BSS, arenas) and the list only wires pointers.

[Full docs (EN)](description.en.md) · [Документация (RU)](description.ru.md) · [Doxygen](https://owlsurf.github.io/LibLL/)

---

## Why LibLL

| Need | LibLL |
|------|--------|
| List of integers / owned values | Use another container |
| Objects in a static pool / scheduler queues | Yes |
| `malloc` inside Connect/Disconnect | Never |
| Empty list without `NULL` checks everywhere | Stub sentinel |
| Move a node between queues | `LL_Change_List` |

Similar idea to Linux kernel `list.h`, in a small standalone library with tests.

---

## Quick start

```c
#include "LibLL.h"

typedef struct {
    NODE node;      /* first field */
    int  id;
} Task;

mCREATE_LIST(ready, ready_tmp);

Task t1 = {0};
t1.id = 1;
mINIT_NODE(&t1.node);

LL_Connect(&ready, &t1.node, &ready_tmp);
/* ... */
LL_Disconnect(&ready, &t1.node, &ready_tmp);
```

Rules of thumb:

1. Put `NODE` first in your struct (or use `offsetof` / `container_of`).
2. Call `mINIT_NODE` before the first `LL_Connect` / `LL_Insert`.
3. Give **each** list its own `tmp` cursor (`mCREATE_LIST` does that).
4. `Connect` ≠ allocate. `Disconnect` ≠ `free`.

---

## Build

```bash
cmake -S . -B build -DLIBLL_BUILD_EXAMPLES=ON -DLIBLL_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Run examples:

```bash
./build/examples/static_tasks
./build/examples/ready_blocked
```

### As a CMake dependency (FetchContent)

```cmake
include(FetchContent)
FetchContent_Declare(libll
    GIT_REPOSITORY https://github.com/OwlSurf/LibLL.git
    GIT_TAG        v0.1.0   # pin a release when tagged
)
set(LIBLL_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LIBLL_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(libll)

target_link_libraries(your_app PRIVATE LibLL::LibLL)
```

### Drop-in

Copy `LibLL.c` + `LibLL.h` into your tree and compile `LibLL.c` with the rest of the firmware.

---

## Examples

| Example | What it shows |
|---------|----------------|
| [`examples/static_tasks.c`](examples/static_tasks.c) | Static intrusive objects, iterate, disconnect |
| [`examples/ready_blocked.c`](examples/ready_blocked.c) | Move a job between ready / blocked queues |

---

## Pitfalls

- **Already linked node** — `LL_Connect` / `LL_Insert` reject nodes that are not isolated (`next`/`prev` ≠ self). Always `mINIT_NODE` (or disconnect) first.
- **One `tmp` per list** — sharing cursors across lists corrupts iteration during insert/disconnect.
- **`LL_Clear_List`** — disconnects and `free`s every node. Use only for heap-allocated nodes.
- **Not thread-safe** — protect lists yourself if used from ISR / multiple threads.
- **License is GPL-3.0** — copyleft; check compatibility with your product before shipping.

---

## API overview

| Function | Role |
|----------|------|
| `LL_Connect` | Append node to list |
| `LL_Insert` | Insert after a node |
| `LL_Disconnect` / `LL_Disconnect_First` | Remove without freeing |
| `LL_Change_List` / `LL_Change_List_Insert` | Move between lists |
| `LL_Find` | Search with a predicate |
| `LL_ForEach` | Safe walk (uses `tmp`) |
| `LL_Clear_List` | Empty list + `free` nodes |

---

## License

[GNU GPL v3](LICENSE)
