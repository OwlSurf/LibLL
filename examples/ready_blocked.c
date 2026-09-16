/**
 * Example: move objects between ready and blocked queues.
 *
 * Shows LL_Change_List — disconnect from one circular list and append
 * to another without allocating memory.
 */

#include <stdio.h>

#include "LibLL.h"

typedef struct {
	NODE node;
	int id;
} Job;

static NODE *print_job(NODE *node)
{
	Job *job = (Job *)node;
	printf(" %d", job->id);
	return node;
}

static void print_queue(const char *title, NODE **list, NODE **tmp)
{
	printf("%s:", title);
	if (*list == (NODE *)&Stub) {
		printf(" (empty)\n");
		return;
	}
	LL_ForEach(list, print_job, tmp);
	printf("\n");
}

int main(void)
{
	mCREATE_LIST(ready, ready_tmp);
	mCREATE_LIST(blocked, blocked_tmp);

	Job jobs[3] = {0};
	for (int i = 0; i < 3; ++i) {
		jobs[i].id = i + 1;
		mINIT_NODE(&jobs[i].node);
		LL_Connect(&ready, &jobs[i].node, &ready_tmp);
	}

	print_queue("ready", &ready, &ready_tmp);
	print_queue("blocked", &blocked, &blocked_tmp);

	/* Job 2 blocks (e.g. waiting for I/O). */
	LL_Change_List(&ready, &blocked, &jobs[1].node, &ready_tmp, &blocked_tmp);

	print_queue("ready", &ready, &ready_tmp);
	print_queue("blocked", &blocked, &blocked_tmp);

	/* Job 2 becomes ready again. */
	LL_Change_List(&blocked, &ready, &jobs[1].node, &blocked_tmp, &ready_tmp);

	print_queue("ready", &ready, &ready_tmp);
	print_queue("blocked", &blocked, &blocked_tmp);

	return 0;
}
