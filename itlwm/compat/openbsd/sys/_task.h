//
//  _task.h
//  itlwm
//
//  Created by qcwap on 2020/3/1.
//  Copyright © 2020 钟先耀. All rights reserved.
//

#ifndef _task_h
#define _task_h

#include <sys/queue.h>
#include "_buf.h"

#include <IOKit/IOLocks.h>

struct task {
    TAILQ_ENTRY(task) t_entry;
    void        (*t_func)(void *);
    void        *t_arg;
    unsigned int    t_flags;
};

#define TASK_ONQUEUE        1
#define TASK_BARRIER        2

TAILQ_HEAD(task_list, task);

#define TASKQ_MPSAFE        (1 << 0)

#define TASK_INITIALIZER(_f, _a)  {{ NULL, NULL }, (_f), (_a), 0 }

#define task_pending(_t)    ((_t)->t_flags & TASK_ONQUEUE)

extern struct taskq *const systq;

void taskq_init();
struct taskq    *taskq_create(const char *, unsigned int, int, unsigned int);
void         taskq_destroy(struct taskq *);
void         taskq_barrier(struct taskq *);

void         taskq_del_barrier(struct taskq *, struct task *);

void         task_set(struct task *, void (*)(void *), void *);
int         task_add(struct taskq *, struct task *);
int         task_del(struct taskq *, struct task *);

#endif /* _task_h */
