# LibLL

[![CMakeGoogletest](https://github.com/OwlSurf/LibLL/actions/workflows/cmake.yml/badge.svg)](https://github.com/OwlSurf/LibLL/actions/workflows/cmake.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/owlsurf/libll/badge)](https://www.codefactor.io/repository/github/owlsurf/libll)

[Doxygen](https://owlsurf.github.io/LibLL/)

---

## English

Embedded circular doubly-linked lists for C (intrusive) — no heap inside the library, static or dynamic nodes, safe iteration while modifying. For bare-metal and systems code where objects already exist (pools, BSS, arenas) and the list only wires pointers.

### Why LibLL

| Need | LibLL |
|------|--------|
| List of integers / owned values | Use another container |
| Objects in a static pool / scheduler queues | Yes |
| `malloc` inside Connect/Disconnect | Never |
| Empty list without `NULL` checks everywhere | Stub sentinel |
| Move a node between queues | `LL_Change_List` |

Similar idea to Linux kernel `list.h`, in a small standalone library with tests.

### Quick start

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

### Build

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

#### As a CMake dependency (FetchContent)

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

#### Drop-in

Copy `LibLL.c` + `LibLL.h` into your tree and compile `LibLL.c` with the rest of the firmware.

### Examples

| Example | What it shows |
|---------|----------------|
| [`examples/static_tasks.c`](examples/static_tasks.c) | Static embedded objects (intrusive), iterate, disconnect |
| [`examples/ready_blocked.c`](examples/ready_blocked.c) | Move a job between ready / blocked queues |

### Pitfalls

- **Already linked node** — `LL_Connect` / `LL_Insert` reject nodes that are not isolated (`next`/`prev` ≠ self). Always `mINIT_NODE` (or disconnect) first.
- **One `tmp` per list** — sharing cursors across lists corrupts iteration during insert/disconnect.
- **`LL_Clear_List`** — disconnects and `free`s every node. Use only for heap-allocated nodes.
- **Not thread-safe** — protect lists yourself if used from ISR / multiple threads.
- **License is GPL-3.0** — copyleft; check compatibility with your product before shipping.

### API overview

| Function | Role |
|----------|------|
| `LL_Connect` | Append node to list |
| `LL_Insert` | Insert after a node |
| `LL_Disconnect` / `LL_Disconnect_First` | Remove without freeing |
| `LL_Change_List` / `LL_Change_List_Insert` | Move between lists |
| `LL_Find` | Search with a predicate |
| `LL_ForEach` | Safe walk (uses `tmp`) |
| `LL_Clear_List` | Empty list + `free` nodes |

### License

[GNU GPL v3](LICENSE)

---

## Русский

Встраиваемые кольцевые двусвязные списки на C (intrusive) — без выделения памяти внутри библиотеки, узлы статические или динамические, безопасный обход при изменении списка. Для bare-metal и системного кода, где объекты уже существуют (пулы, BSS, арены), а список только связывает указатели.

### Зачем LibLL

| Задача | LibLL |
|------|--------|
| Список целых / владение значениями | Другой контейнер |
| Объекты в статическом пуле / очереди планировщика | Да |
| `malloc` внутри Connect/Disconnect | Никогда |
| Пустой список без проверок на `NULL` повсюду | Sentinel Stub |
| Перенос узла между очередями | `LL_Change_List` |

Идея близка к `list.h` ядра Linux: небольшая самостоятельная библиотека с тестами.

### Быстрый старт

```c
#include "LibLL.h"

typedef struct {
    NODE node;      /* первое поле */
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

Кратко:

1. Поле `NODE` должно быть первым в структуре (или используйте `offsetof` / `container_of`).
2. Вызывайте `mINIT_NODE` перед первым `LL_Connect` / `LL_Insert`.
3. У **каждого** списка свой курсор `tmp` (`mCREATE_LIST` создаёт его).
4. `Connect` ≠ выделение памяти. `Disconnect` ≠ `free`.

### Сборка

```bash
cmake -S . -B build -DLIBLL_BUILD_EXAMPLES=ON -DLIBLL_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Запуск примеров:

```bash
./build/examples/static_tasks
./build/examples/ready_blocked
```

#### Как зависимость CMake (FetchContent)

```cmake
include(FetchContent)
FetchContent_Declare(libll
    GIT_REPOSITORY https://github.com/OwlSurf/LibLL.git
    GIT_TAG        v0.1.0   # зафиксируйте релизный тег
)
set(LIBLL_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LIBLL_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(libll)

target_link_libraries(your_app PRIVATE LibLL::LibLL)
```

#### Drop-in

Скопируйте `LibLL.c` + `LibLL.h` в свой проект и скомпилируйте `LibLL.c` вместе с прошивкой.

### Примеры

| Пример | Что показывает |
|---------|----------------|
| [`examples/static_tasks.c`](examples/static_tasks.c) | Статические встраиваемые объекты (intrusive), обход, отключение |
| [`examples/ready_blocked.c`](examples/ready_blocked.c) | Перенос задачи между очередями ready / blocked |

### Подводные камни

- **Узел уже в списке** — `LL_Connect` / `LL_Insert` отклоняют узлы, которые не изолированы (`next`/`prev` ≠ self). Сначала `mINIT_NODE` (или disconnect).
- **Один `tmp` на список** — общий курсор для разных списков ломает итерацию при insert/disconnect.
- **`LL_Clear_List`** — отключает и делает `free` каждому узлу. Только для узлов из кучи.
- **Не потокобезопасно** — при ISR / нескольких потоках синхронизацию обеспечиваете сами.
- **Лицензия GPL-3.0** — copyleft; проверьте совместимость с вашим продуктом.

### Обзор API

| Функция | Назначение |
|----------|------|
| `LL_Connect` | Добавить узел в конец списка |
| `LL_Insert` | Вставить после узла |
| `LL_Disconnect` / `LL_Disconnect_First` | Отключить без освобождения памяти |
| `LL_Change_List` / `LL_Change_List_Insert` | Переместить между списками |
| `LL_Find` | Поиск по предикату |
| `LL_ForEach` | Безопасный обход (через `tmp`) |
| `LL_Clear_List` | Очистить список + `free` узлов |

### Лицензия

[GNU GPL v3](LICENSE)
