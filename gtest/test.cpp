#include <gtest/gtest.h>

#include "LibLL.h"

TEST(ListCreate, CreaeteWithMacro)
{
	mCREATE_LIST(test_list, test_tmp);
	ASSERT_EQ(test_list, (NODE*)&Stub);
       	ASSERT_EQ(test_list->next, test_list);
	ASSERT_EQ(test_list->prev, test_list);	
}

TEST(Connect, OneStaticNode)
{
	NODE first;
	NODE* f = &first;
	mINIT_NODE(f);
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, f, &test_tmp);
	
	ASSERT_EQ(test_list, f);
	ASSERT_NE(test_list, (NODE*)&Stub);	
	ASSERT_EQ(test_list->next, test_list);
	ASSERT_EQ(test_list->prev, test_list);
}

TEST(Connect, TwoStaticNodes)
{
	NODE first;
	NODE second;
	NODE* f = &first;
	NODE* s = &second;
	mINIT_NODE(f);
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, f, &test_tmp);
	LL_Connect(&test_list, s, &test_tmp);

	ASSERT_NE(test_list, (NODE*)&Stub);

	ASSERT_EQ(test_list, f);

	ASSERT_EQ(test_list->next, s);
	ASSERT_EQ(test_list->prev, s);
	ASSERT_EQ(test_list, f);
	ASSERT_EQ(f->next, s);
	ASSERT_EQ(f->prev, s);
	ASSERT_EQ(s->next, f);
	ASSERT_EQ(s->prev, f);
}

TEST(Connect, ThreeDynamicNodes)
{
	NODE* f = (NODE*)malloc(sizeof(NODE));
	NODE* s = (NODE*)malloc(sizeof(NODE));
	NODE* t = (NODE*)malloc(sizeof(NODE));
	mINIT_NODE(f);
	mINIT_NODE(s);
	mINIT_NODE(t);
	
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, f, &test_tmp);
	LL_Connect(&test_list, s, &test_tmp);
	LL_Connect(&test_list, t, &test_tmp);

	ASSERT_EQ(f->prev, t);
	ASSERT_EQ(t->next, f);

	ASSERT_EQ(f->next, s);
	ASSERT_EQ(t->prev, s);

	ASSERT_EQ(s->next, t);
	ASSERT_EQ(s->prev, f);
	
	LL_Disconnect(&test_list, f, &test_tmp);
	LL_Disconnect(&test_list, s, &test_tmp);
	LL_Disconnect(&test_list, t, &test_tmp);

	ASSERT_EQ(f->next, f);
	ASSERT_EQ(f->prev, f);

	ASSERT_EQ(s->next, s);
	ASSERT_EQ(s->prev, s);

	ASSERT_EQ(t->next, t);
	ASSERT_EQ(t->prev, t);


	ASSERT_EQ(test_list, (NODE*)&Stub);


	free(f);free(s);free(t);
}

TEST(InsertOneDynamicNode, InsertBeetwenDynamicTwo)
{
	NODE* f = (NODE*)malloc(sizeof(NODE));
	NODE* s = (NODE*)malloc(sizeof(NODE));
	NODE* t = (NODE*)malloc(sizeof(NODE));
	mINIT_NODE(f);
	mINIT_NODE(s);
	mINIT_NODE(t);
	
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, f, &test_tmp);
	LL_Connect(&test_list, s, &test_tmp);

	LL_Insert(f, t, &test_tmp);

	ASSERT_EQ(t->next, s);
	ASSERT_EQ(t->prev, f);

	ASSERT_EQ(f->prev, s);
	ASSERT_EQ(s->next, f);

	ASSERT_EQ(t->next, s);
	ASSERT_EQ(t->prev, f);
	
	LL_Disconnect(&test_list, f, &test_tmp);
	LL_Disconnect(&test_list, s, &test_tmp);
	LL_Disconnect(&test_list, t, &test_tmp);

	ASSERT_EQ(f->next, f);
	ASSERT_EQ(f->prev, f);

	ASSERT_EQ(s->next, s);
	ASSERT_EQ(s->prev, s);

	ASSERT_EQ(t->next, t);
	ASSERT_EQ(t->prev, t);

	ASSERT_EQ(test_list, (NODE*)&Stub);

	free(f);free(s);free(t);
}

TEST(InsertTwoDynamicNodes, InsertBeetwenToDynamicNodes)
{
	NODE* first = (NODE*)malloc(sizeof(NODE));
	NODE* second = (NODE*)malloc(sizeof(NODE));

	NODE* third = (NODE*)malloc(sizeof(NODE));
	NODE* fours = (NODE*)malloc(sizeof(NODE));
	mINIT_NODE(first);
	mINIT_NODE(second);
	mINIT_NODE(third);
	mINIT_NODE(fours);

	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, &test_tmp);
	LL_Connect(&test_list, second, &test_tmp);

	LL_Insert(first, third, &test_tmp);
	LL_Insert(third, fours, &test_tmp);
	
	ASSERT_EQ(first->next, third);
	ASSERT_EQ(third->prev, first);
	ASSERT_EQ(third->next, fours);
	ASSERT_EQ(fours->prev, third);
	ASSERT_EQ(fours->next, second);
	ASSERT_EQ(second->prev, fours);

	ASSERT_EQ(first->prev, second);
	ASSERT_EQ(second->next, first);

	free(first);free(second);free(third);free(fours);
}

TEST(InsertLast, InsertAfterTwoDynamicNodes)
{
	NODE* first = (NODE*)malloc(sizeof(NODE));
	NODE* second = (NODE*)malloc(sizeof(NODE));

	NODE* third = (NODE*)malloc(sizeof(NODE));
	mINIT_NODE(first);
	mINIT_NODE(second);
	mINIT_NODE(third);

	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, &test_tmp);
	LL_Connect(&test_list, second, &test_tmp);

	LL_Insert(second, third, &test_tmp);
	
	ASSERT_EQ(third->next, first);
	ASSERT_EQ(third->prev, second);
	ASSERT_EQ(first->prev, third);


	free(first);free(second);free(third);
}

TEST(Disconnect, DisconnectMiddle)
{
	NODE* first = (NODE*)malloc(sizeof(NODE));
	NODE* second = (NODE*)malloc(sizeof(NODE));
	NODE* third = (NODE*)malloc(sizeof(NODE));
	mINIT_NODE(first);
	mINIT_NODE(second);
	mINIT_NODE(third);

	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, &test_tmp);
	LL_Connect(&test_list, second, &test_tmp);
	LL_Connect(&test_list, third, &test_tmp);

	LL_Disconnect(&test_list, second, &test_tmp);

	ASSERT_EQ(first->next, third);
	ASSERT_EQ(third->prev, first);
	ASSERT_EQ(first->prev, third);
	ASSERT_EQ(third->next, first);

	free(first);free(second);free(third);
}

static NODE* make_node()
{
	NODE* node = (NODE*)malloc(sizeof(NODE));
	mINIT_NODE(node);
	return node;
}

TEST(DisconnectFirst, SingleNode)
{
	NODE* first = make_node();
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, &test_tmp);

	NODE* deleted = NULL;
	LL_Disconnect_First(&test_list, &deleted, &test_tmp);

	ASSERT_EQ(deleted, first);
	ASSERT_EQ(test_list, (NODE*)&Stub);
	ASSERT_TRUE(mISOLATED_NODE(first));

	free(first);
}

TEST(DisconnectFirst, MultipleNodes)
{
	NODE* first = make_node();
	NODE* second = make_node();
	NODE* third = make_node();
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, &test_tmp);
	LL_Connect(&test_list, second, &test_tmp);
	LL_Connect(&test_list, third, &test_tmp);

	NODE* deleted = NULL;
	LL_Disconnect_First(&test_list, &deleted, &test_tmp);

	ASSERT_EQ(deleted, first);
	ASSERT_EQ(test_list, second);
	ASSERT_TRUE(mISOLATED_NODE(first));
	ASSERT_EQ(second->next, third);
	ASSERT_EQ(third->prev, second);

	free(first); free(second); free(third);
}

TEST(DisconnectFirst, EmptyList)
{
	mCREATE_LIST(test_list, test_tmp);
	NODE* deleted = (NODE*)0x1;

	LL_Disconnect_First(&test_list, &deleted, &test_tmp);

	ASSERT_EQ(deleted, (NODE*)NULL);
	ASSERT_EQ(test_list, (NODE*)&Stub);
}

TEST(DisconnectFirst, NullDeletedNode)
{
	mCREATE_LIST(test_list, test_tmp);

	LL_Disconnect_First(&test_list, NULL, &test_tmp);

	ASSERT_EQ(test_list, (NODE*)&Stub);
}

TEST(Disconnect, DisconnectFirstNode)
{
	NODE* first = make_node();
	NODE* second = make_node();
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, &test_tmp);
	LL_Connect(&test_list, second, &test_tmp);

	LL_Disconnect(&test_list, first, &test_tmp);

	ASSERT_EQ(test_list, second);
	ASSERT_TRUE(mISOLATED_NODE(first));
	ASSERT_EQ(second->next, second);
	ASSERT_EQ(second->prev, second);

	free(first); free(second);
}

TEST(Disconnect, DisconnectLastNode)
{
	NODE* first = make_node();
	NODE* second = make_node();
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, &test_tmp);
	LL_Connect(&test_list, second, &test_tmp);

	LL_Disconnect(&test_list, second, &test_tmp);

	ASSERT_EQ(test_list, first);
	ASSERT_TRUE(mISOLATED_NODE(second));
	ASSERT_EQ(first->next, first);
	ASSERT_EQ(first->prev, first);

	free(first); free(second);
}

TEST(Disconnect, NullTmpDoesNotCrash)
{
	NODE* first = make_node();
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, &test_tmp);
	LL_Disconnect(&test_list, first, NULL);

	ASSERT_EQ(test_list, (NODE*)&Stub);
	ASSERT_TRUE(mISOLATED_NODE(first));

	free(first);
}

TEST(ChangeList, MoveNodeToAnotherList)
{
	NODE* first = make_node();
	NODE* second = make_node();
	NODE* moved = make_node();
	mCREATE_LIST(src_list, src_tmp);
	mCREATE_LIST(dst_list, dst_tmp);

	LL_Connect(&src_list, first, &src_tmp);
	LL_Connect(&src_list, second, &src_tmp);
	LL_Connect(&src_list, moved, &src_tmp);
	LL_Connect(&dst_list, make_node(), &dst_tmp);

	NODE* dst_only = dst_list;
	LL_Disconnect_First(&dst_list, &dst_only, &dst_tmp);
	free(dst_only);

	LL_Change_List(&src_list, &dst_list, moved, &src_tmp, &dst_tmp);

	ASSERT_EQ(src_list, first);
	ASSERT_EQ(first->next, second);
	ASSERT_EQ(second->prev, first);
	ASSERT_EQ(dst_list, moved);
	ASSERT_EQ(moved->next, moved);
	ASSERT_EQ(moved->prev, moved);

	free(first); free(second); free(moved);
}

TEST(ChangeListInsert, MoveNodeIntoMiddle)
{
	NODE* a = make_node();
	NODE* b = make_node();
	NODE* moved = make_node();
	mCREATE_LIST(src_list, src_tmp);
	mCREATE_LIST(dst_list, dst_tmp);

	LL_Connect(&src_list, moved, &src_tmp);
	LL_Connect(&dst_list, a, &dst_tmp);
	LL_Connect(&dst_list, b, &dst_tmp);

	LL_Change_List_Insert(&src_list, a, moved, &src_tmp, &dst_tmp);

	ASSERT_EQ(src_list, (NODE*)&Stub);
	ASSERT_EQ(a->next, moved);
	ASSERT_EQ(moved->prev, a);
	ASSERT_EQ(moved->next, b);
	ASSERT_EQ(b->prev, moved);

	free(a); free(b); free(moved);
}

TEST(ClearList, FreesAllDynamicNodes)
{
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, make_node(), &test_tmp);
	LL_Connect(&test_list, make_node(), &test_tmp);
	LL_Connect(&test_list, make_node(), &test_tmp);

	LL_Clear_List(&test_list);

	ASSERT_EQ(test_list, (NODE*)&Stub);
}

typedef struct tagged_node {
	NODE node;
	int tag;
} TAGGED_NODE;

static bool find_tag(void* obj, va_list args)
{
	TAGGED_NODE* tagged = (TAGGED_NODE*)obj;
	int target = va_arg(args, int);
	return tagged->tag == target;
}

TEST(Find, FindsNextNodeByTag)
{
	TAGGED_NODE a = { .tag = 1 };
	TAGGED_NODE b = { .tag = 2 };
	TAGGED_NODE c = { .tag = 3 };
	mINIT_NODE(&a.node);
	mINIT_NODE(&b.node);
	mINIT_NODE(&c.node);
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, &a.node, &test_tmp);
	LL_Connect(&test_list, &b.node, &test_tmp);
	LL_Connect(&test_list, &c.node, &test_tmp);

	NODE* found = LL_Find(&a.node, Next, find_tag, 2);

	ASSERT_EQ(found, &b.node);
}

TEST(Find, FindsPrevNodeByTag)
{
	TAGGED_NODE a = { .tag = 1 };
	TAGGED_NODE b = { .tag = 2 };
	TAGGED_NODE c = { .tag = 3 };
	mINIT_NODE(&a.node);
	mINIT_NODE(&b.node);
	mINIT_NODE(&c.node);
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, &a.node, &test_tmp);
	LL_Connect(&test_list, &b.node, &test_tmp);
	LL_Connect(&test_list, &c.node, &test_tmp);

	NODE* found = LL_Find(&c.node, Prev, find_tag, 2);

	ASSERT_EQ(found, &b.node);
}

TEST(Find, ReturnsNullWhenNotFound)
{
	TAGGED_NODE a = { .tag = 1 };
	mINIT_NODE(&a.node);
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, &a.node, &test_tmp);

	NODE* found = LL_Find(&a.node, Next, find_tag, 99);

	ASSERT_EQ(found, (NODE*)NULL);
}

static int foreach_count = 0;

static NODE* count_nodes(NODE* node)
{
	(void)node;
	foreach_count++;
	return node;
}

TEST(ForEach, VisitsEveryNode)
{
	NODE* first = make_node();
	NODE* second = make_node();
	NODE* third = make_node();
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, &test_tmp);
	LL_Connect(&test_list, second, &test_tmp);
	LL_Connect(&test_list, third, &test_tmp);

	foreach_count = 0;
	LL_ForEach(&test_list, count_nodes, &test_tmp);

	ASSERT_EQ(foreach_count, 3);

	free(first); free(second); free(third);
}

TEST(ForEach, NullTmpDoesNotCrashOnEmptyList)
{
	mCREATE_LIST(test_list, test_tmp);

	LL_ForEach(&test_list, count_nodes, NULL);

	ASSERT_EQ(test_list, (NODE*)&Stub);
}

TEST(Connect, RejectsAlreadyConnectedNode)
{
	NODE first;
	NODE second;
	NODE* f = &first;
	NODE* s = &second;
	mINIT_NODE(f);
	mINIT_NODE(s);
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, f, &test_tmp);
	LL_Connect(&test_list, s, &test_tmp);

	NODE* extra = make_node();
	LL_Connect(&test_list, f, &test_tmp);

	ASSERT_EQ(test_list, f);
	ASSERT_EQ(f->next, s);
	ASSERT_EQ(f->prev, s);
	ASSERT_TRUE(mISOLATED_NODE(extra));

	free(extra);
}

TEST(Insert, RejectsAlreadyConnectedNode)
{
	NODE* first = make_node();
	NODE* second = make_node();
	NODE* third = make_node();
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, &test_tmp);
	LL_Connect(&test_list, second, &test_tmp);
	LL_Connect(&test_list, third, &test_tmp);

	LL_Insert(first, third, &test_tmp);

	ASSERT_EQ(first->next, second);
	ASSERT_EQ(second->prev, first);
	ASSERT_EQ(third->next, first);
	ASSERT_EQ(third->prev, second);

	free(first); free(second); free(third);
}

TEST(Connect, NullTmpOnEmptyListDoesNotModify)
{
	NODE* first = make_node();
	mCREATE_LIST(test_list, test_tmp);

	LL_Connect(&test_list, first, NULL);

	ASSERT_EQ(test_list, (NODE*)&Stub);
	ASSERT_TRUE(mISOLATED_NODE(first));

	free(first);
}
