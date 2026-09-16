/**
 * \file    LibLL.h
 * \brief   Public API for LibLL — embedded circular doubly-linked lists (intrusive).
 *
 * Abbreviations: LL = Linked list.
 *
 * \author    Roman Garanin
 * \copyright (c) Roman Garanin
 */

#ifndef LIBLL_H
#define LIBLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

/**
 * \defgroup libll_types Types and macros
 * \brief Core node type, empty-list stub, and helpers.
 * @{
 */

/**
 * \struct stub_node
 * \brief Internal empty-list sentinel (opaque to callers).
 *
 * Application code should treat an empty list as a head that points at #Stub
 * and must not modify the sentinel.
 */
struct stub_node;

/**
 * \brief Global empty-list stub shared by all lists.
 *
 * After #mCREATE_LIST, both the list head and its `tmp` cursor point here.
 * When the last node is disconnected, the head is restored to #Stub.
 */
extern struct stub_node Stub;

/**
 * \brief Declare and initialise a list head and its private `tmp` cursor.
 * \param ListName Identifier for the list head pointer.
 * \param ListTmp  Identifier for the per-list iteration cursor.
 *
 * Expands to two `NODE*` variables, both initially pointing at #Stub.
 */
#define mCREATE_LIST(ListName, ListTmp)\
	NODE* ListName = (NODE*)&Stub;\
	NODE* ListTmp = (NODE*)&Stub;

/**
 * \brief Make a node isolated (point at itself).
 * \param node Pointer to a #NODE (must not be NULL).
 *
 * Required before the first #LL_Connect / #LL_Insert for that node.
 */
#define mINIT_NODE(node)\
	(node)->next = (node);\
	(node)->prev = (node);

/**
 * \brief True if the node is isolated (`next` and `prev` point to itself).
 * \param node Pointer to a #NODE.
 */
#define mISOLATED_NODE(node)\
	((node)->next == (node) && (node)->prev == (node))

/**
 * \brief Intrusive list node — embed as the first field of your object.
 */
typedef struct _node
{
	struct _node* next;  /**< Next node in the circular list. */
	struct _node* prev;  /**< Previous node in the circular list. */
} NODE;

/**
 * \brief Search direction for #LL_Find.
 */
enum dir { Next = 0, Prev };

/** @} */

/**
 * \defgroup libll_mutate Connect, insert, disconnect
 * \brief Attach and detach nodes without allocating or freeing memory.
 * @{
 */

/**
 * \brief Append an isolated node to the end of the list.
 * \param[in,out] List List head. May change from #Stub to the first node.
 * \param[in]     node Node to append; must be isolated (#mISOLATED_NODE).
 * \param[in,out] tmp  Per-list cursor; required when the list is empty.
 *
 * \note Does not allocate memory. Silently returns if `node` is not isolated
 *       or if required pointers are NULL.
 */
void LL_Connect(NODE** List, NODE* node, NODE** tmp);

/**
 * \brief Insert an isolated node after an existing node.
 * \param[in]     node        Node after which to insert.
 * \param[in]     insert_node Node to insert; must be isolated.
 * \param[in,out] tmp         Per-list cursor; updated if it pointed past `node`.
 *
 * \note Does not allocate memory. Silently returns if `insert_node` is not isolated.
 */
void LL_Insert(NODE* node, NODE* insert_node, NODE** tmp);

/**
 * \brief Disconnect a node from the list without freeing it.
 * \param[in,out] List List head (may become #Stub if the list empties).
 * \param[in]     node Node to disconnect; left isolated on success.
 * \param[in,out] tmp  Optional per-list cursor; advanced if it pointed at `node`.
 *
 * \note Does not free memory. `tmp` may be NULL.
 */
void LL_Disconnect(NODE** List, NODE* node, NODE** tmp);

/**
 * \brief Disconnect the first node of the list without freeing it.
 * \param[in,out] List         List head.
 * \param[out]    deleted_node Receives the disconnected node, or NULL if empty.
 * \param[in,out] tmp          Per-list cursor (currently unused by the implementation).
 *
 * \note Does not free memory. Safe if `deleted_node` is non-NULL and the list is empty.
 */
void LL_Disconnect_First(NODE** List, NODE** deleted_node, NODE** tmp);

/** @} */

/**
 * \defgroup libll_move Move between lists
 * \brief Relocate a node from one list to another.
 * @{
 */

/**
 * \brief Move a node from the source list to the end of the destination list.
 * \param[in,out] SrcList  Source list head.
 * \param[in,out] DestList Destination list head.
 * \param[in]     node     Node to move.
 * \param[in,out] sl_tmp   Source list cursor.
 * \param[in,out] dl_tmp   Destination list cursor.
 *
 * Equivalent to #LL_Disconnect on the source followed by #LL_Connect on the destination.
 */
void LL_Change_List(NODE** SrcList, NODE** DestList, NODE* node, NODE** sl_tmp, NODE** dl_tmp);

/**
 * \brief Move a node from the source list and insert it after a destination node.
 * \param[in,out] SrcList      Source list head.
 * \param[in]     insert_after Destination node after which to insert.
 * \param[in]     node         Node to move.
 * \param[in,out] sl_tmp       Source list cursor.
 * \param[in,out] dl_tmp       Destination list cursor.
 *
 * Equivalent to #LL_Disconnect on the source followed by #LL_Insert on the destination.
 */
void LL_Change_List_Insert(NODE** SrcList, NODE* insert_after, NODE* node, NODE** sl_tmp, NODE** dl_tmp);

/** @} */

/**
 * \defgroup libll_memory Clear
 * \brief Operations that free heap-allocated nodes.
 * @{
 */

/**
 * \brief Disconnect every node and call `free` on each.
 * \param[in,out] List List head; becomes #Stub when done.
 *
 * \warning Only for lists whose nodes were allocated with `malloc` (or compatible).
 *          Do not use on static / pool-backed nodes.
 */
void LL_Clear_List(NODE** List);

/** @} */

/**
 * \defgroup libll_iterate Find and iterate
 * \brief Search and safe traversal.
 * @{
 */

/**
 * \brief Find a node using a caller-supplied predicate.
 * \param[in] start_node Node where the search starts (not tested first; walk begins at neighbour).
 * \param[in] direction  #Next or #Prev.
 * \param[in] pCheckSign Predicate: receives the current node and a `va_list` of caller args.
 * \param[in] ...        Arguments forwarded to `pCheckSign` via `va_list`.
 * \return Matching node, or NULL if none / on invalid input.
 *
 * \note Always ends the `va_list` before returning (including on match).
 */
NODE* LL_Find(NODE* start_node, enum dir direction, bool (*pCheckSign)(void* obj, va_list args), ...);

/**
 * \brief Call an action for every node, safe against disconnect of the current node.
 * \param[in,out] list    List head.
 * \param[in]     pAction Callback invoked with the current node.
 * \param[in,out] tmp     Per-list cursor used to remember the next node.
 *
 * If the list is empty (#Stub), invokes the stub empty-list action and returns.
 * `tmp` must be non-NULL when the list is non-empty.
 */
void LL_ForEach(NODE** list, NODE* (*pAction)(NODE* node), NODE** tmp);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* LIBLL_H */
