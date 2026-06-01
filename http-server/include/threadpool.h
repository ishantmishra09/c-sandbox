#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <pthread.h>

#define N_THREADS 4

typedef struct Task {
  void (*fn)(int);
  int client_fd;
  struct Task *next;
} Task;

typedef struct {
  pthread_t threads[N_THREADS];
  Task *head;
  Task *tail;
  pthread_mutex_t pt_lock;
  pthread_cond_t pt_wa; // work available
  int running;
} ThreadPool;

ThreadPool *tpool_create(void);

void tpool_add_task(ThreadPool *tpool, void (*fn)(int), int client_fd);

void tpool_destroy(ThreadPool *tpool);

#endif
