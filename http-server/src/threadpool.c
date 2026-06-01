#include "../include/threadpool.h"

#include <stdio.h>
#include <stdlib.h>

void *worker_loop(void *arg) {

  ThreadPool *tpool = (ThreadPool *)arg;

  while (1) {

    pthread_mutex_lock(&tpool->pt_lock);

    while (tpool->head == NULL)
      pthread_cond_wait(&tpool->pt_hw, &tpool->pt_lock);

    Task *task = tpool->head;
    tpool->head = task->next;
    if (tpool->head == NULL)
      tpool->tail = NULL;

    pthread_mutex_unlock(&tpool->pt_lock);

    task->fn(task->client_fd);
    free(task);
  }

  return NULL;
}

ThreadPool *tpool_create(void) {

  ThreadPool *tpool = (ThreadPool *)malloc(sizeof(ThreadPool));
  if (tpool == NULL) {

    perror("tpool_create: malloc");
    exit(EXIT_FAILURE);
  }

  tpool->head = NULL;
  tpool->tail = NULL;
  pthread_mutex_init(&tpool->pt_lock, NULL);
  pthread_cond_init(&tpool->pt_hw, NULL);

  for (int i = 0; i < N_THREADS; i++)
    pthread_create(&tpool->threads[i], NULL, worker_loop, tpool);

  printf("Thread pool ready (%d workers)\n", N_THREADS);
  return tpool;
}

void tpool_add_task(ThreadPool *tpool, void (*fn)(int), int client_fd) {

  Task *t = (Task *)malloc(sizeof(Task));
  if (t == NULL) {

    perror("tpool_add_task: malloc\n");
    exit(EXIT_FAILURE);
  }

  t->fn = fn;
  t->client_fd = client_fd;
  t->next = NULL;

  pthread_mutex_lock(&tpool->pt_lock);

  if (tpool->tail)
    tpool->tail->next = t;
  else
    tpool->head = t;

  tpool->tail = t;

  pthread_cond_signal(&tpool->pt_hw);

  pthread_mutex_unlock(&tpool->pt_lock);
}
