/**
 * Example: static intrusive task list (no malloc).
 *
 * Typical embedded pattern: objects live in a pool / BSS, NODE is the
 * first field, lists only rearrange pointers.
 */

#include <stdio.h>

#include "LibLL.h"

typedef struct {
	NODE node; /* must be first for a simple cast back to Task */
	int id;
	const char *name;
} Task;

static NODE *print_task(NODE *node)
{
	Task *task = (Task *)node;
	printf("  task id=%d name=%s\n", task->id, task->name);
	return node;
}

int main(void)
{
	mCREATE_LIST(ready, ready_tmp);

	Task t1 = {0};
	Task t2 = {0};
	Task t3 = {0};

	t1.id = 1;
	t1.name = "sensor";
	t2.id = 2;
	t2.name = "control";
	t3.id = 3;
	t3.name = "telemetry";

	mINIT_NODE(&t1.node);
	mINIT_NODE(&t2.node);
	mINIT_NODE(&t3.node);

	LL_Connect(&ready, &t1.node, &ready_tmp);
	LL_Connect(&ready, &t2.node, &ready_tmp);
	LL_Connect(&ready, &t3.node, &ready_tmp);

	printf("Ready queue:\n");
	LL_ForEach(&ready, print_task, &ready_tmp);

	LL_Disconnect(&ready, &t2.node, &ready_tmp);
	printf("After removing control:\n");
	LL_ForEach(&ready, print_task, &ready_tmp);

	return 0;
}
