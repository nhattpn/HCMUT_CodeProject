#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t *q)
{
        if (q == NULL)
                return 1;
        return (q->size == 0);
}

void enqueue(struct queue_t *q, struct pcb_t *proc)
{
        /* TODO: put a new process to queue [q] */
        if (q->size >= MAX_QUEUE_SIZE)
                return;
        if (q->size >= 0)
        {
                #ifdef MLQ_SCHED
		if(proc->priority != proc->prio){
			proc->priority = proc->prio;
		}
		#endif
                q->proc[q->size] = proc;
                q->size++;
        }
}

struct pcb_t *dequeue(struct queue_t *q)
{
        /* TODO: return a pcb whose prioprity is the highest
         * in the queue [q] and remember to remove it from q
         * */
        if (!q->size)
                return NULL;
        int id = 0;
        for (int i = 0; i < q->size; i++)
                if (q->proc[id]->priority < q->proc[i]->priority)
                        id = i;
        struct pcb_t *temp = q->proc[id];
        q->size--;
        for (int i = id; i < q->size; i++)
                q->proc[i] = q->proc[i + 1];
        return temp;
}
