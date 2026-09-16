Linked List Library
============
## Статус:
[![CodeFactor](https://www.codefactor.io/repository/github/owlsurf/libll/badge)](https://www.codefactor.io/repository/github/owlsurf/libll)
[![CMakeGoogletest](https://github.com/OwlSurf/LibLL/actions/workflows/cmake.yml/badge.svg)](https://github.com/OwlSurf/LibLL/actions/workflows/cmake.yml)
## Краткое описание:

Встраиваемые кольцевые двусвязные списки на C (intrusive) — без выделения памяти внутри библиотеки, узлы статические или динамические, безопасный обход при изменении списка. Для bare-metal и системного кода, где объекты уже существуют (пулы, BSS, арены), а список только связывает указатели.

[Документация](https://owlsurf.github.io/LibLL/) (Doxygen)

## Содержание:
* [Добавление узла в конец связного списка.](https://owlsurf.github.io/LibLL/LibLL_8c.html#a8274f49f06aa1cf53309da647eb4b06f)
* [Вставка узла в связный список.](https://owlsurf.github.io/LibLL/LibLL_8c.html#a0d58c85ad363abb9b05062ab5e48c1cf)
* [Отключение узла из связного списка.](https://owlsurf.github.io/LibLL/LibLL_8c.html#a4dace3d699f8e0e8eccc721ef5b64c21)
* [Отключение первого узла от связного списка.](https://owlsurf.github.io/LibLL/LibLL_8c.html#a6a39a1c6328eb13c0c6166fb13d1e43c)
* [Перемещение узла из одного связного списка в конец другого.](https://owlsurf.github.io/LibLL/LibLL_8c.html#a87970dc9c75edeeab8ac03eb63452baf)
* [Перемещение узла из одного связного списка в другой.](https://owlsurf.github.io/LibLL/LibLL_8c.html#a84a82de5aa962aa5de7c2239dff19a16)
* [Поиск узла в связном списке.](https://owlsurf.github.io/LibLL/LibLL_8c.html#ab9dbdec177ff191a766d28a90e2d9ba4)
* [Перебор листа.](https://owlsurf.github.io/LibLL/LibLL_8c.html#a22eddbec2ce1ca0eac1d7385558044e6)
* [Удаление листа](https://owlsurf.github.io/LibLL/LibLL_8c.html#a2659814c359a3f001fdc7701e5515bbb) Только для листов созданных динамически.

### Внимание!: 
Добавление - не ознаяает создание узла в памяти.Пользователю необходимо создать узел перед добавлением.

Отключение - Не означает удаление узла из памяти.

## Термины и определения:

## Описание:
Это простая библиотека для создания и управления связными списками написанная на C.

Основной элемент связного списка - узел. Узел это просто структура которая содержит два указателя:

- "next" - Указатель на следующий узел в списке.
- "prev" - Указатель на предыдущий указатель в списке.

Реализация данной библиотеки предполагает:
- Узел может быть создан как динамически так и статически;
- Узлы могут существовать не зависимо от связного списка; 
- Указатели независимых узлов указыват на этот же узел т.е. узел вне списка указывает сам на себя;
- Связный список замкнутый. Это означает, что указатель next последнего узла указывает на превый узел, а указатель prev первого узла указывает на последний узел;
- Каждый список имеет свой собственный указатель, который всегда указывает на первый узел списка;
- Каждый список имеет свой отдельный дополнительный указатель для безопасного управления списком;
- Если список пустой, то указатель списка указывает на узел-заглушку который реализован в библиотеке; 
## Как собрать тесты.
Для покрытия тестами используется  [googletest framework](https://github.com/google/googletest). 	

Для сборки нужны следующие программы:
- cmake;
- gcc;
- g++;
- libgtest-dev;

Чтобы собрать тесты наберите в командной строку:

    cmake -S gtest/ -B gtest/build 
    cmake --build gtest/build
    make -C gtest/build/

Для запуска тестов наберите в командной строке:

    cd gtest/build/; ctest; cd ../../
    or 
    ./gtest/build/runtests

## Как пользоваться:
1) Создайте тип узла для нового списка на основего базового типа узла NODE. При этом тип NODE Должен быть первым в структуре нового типа узла.

Пример:
```C
typedef struct obj {
    NODE node;         
    uint16_t id;       
    void (*action)();
} OBJ;
```
2) Создайте указатель на связный список. Этот указатель должен указывать на узел заглушку определенную в библиотеке (NODE Stub).
3) Создайте дополнительный указатель для списка. Этот указатель будет использоваться для безопасного управления связным списком.

Для этого можно использовать макрос mCREATE_LIST.

Example: 
```C
    mCREATE_LIST(ObjList, ObjListTmp);
```
Макрос развернется в код кооторый создает два указателя:
```C
   	NODE* ObjList = (NODE*)&Stub;
	NODE* ObjListTmp = (NODE*)&Stub;
```

4) Создайте узлы на основе нового типа узла.

Пример: 
```C
OBJ * obj1 = malloc(sizeof(OBJ));
obj1->id  = 1;
obj1->action = some_real_function1;

OBJ * obj2 = malloc(sizeof(OBJ));
obj2->id  = 2;
obj2->action = some_real_function2;
```

5) Управляейте списком с помощью функций библиотеки. Перед каждым действием приволдите тип узла к базовому типу узла NODE. 

Пример:
```C
LL_Connect(&ObjList, (NODE*)obj1, &ObjListTmp);
LL_Connect(&ObjList, (NODE*)obj2, &ObjListTmp);

NODE* node = Objlist;
do {
	ObjListTmp = node->next;
	node->action();
	node = ObjListTmp;
    } while ((ObjListTmp != ObjList));

LL_Disconnect(&ObjList, (NODE*)obj1, &ObjListTmp);
LL_Disconnect(&ObjList, (NODE*)obj2, &ObjListTmp);

free(obj1);
free(obj2);
```
