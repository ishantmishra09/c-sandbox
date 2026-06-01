#include "../include/threadpool.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void *worker_loop(void *arg) {

  ThreadPool *tpool = (ThreadPool *)arg;

  while (1) {

    pthread_mutex_lock(&tpool->pt_lock);

    while (tpool->head == NULL && tpool->running)
      pthread_cond_wait(&tpool->pt_wa, &tpool->pt_lock);

    if (!tpool->running && tpool->head == NULL) {

      pthread_mutex_unlock(&tpool->pt_lock);
      break;
    }

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

  ThreadPool *tpool = (ThreadPool *)calloc(1, sizeof(ThreadPool));
  if (tpool == NULL) {

    perror("tpool_create: calloc");
    return NULL;
  }

  tpool->running = 1;
  tpool->head = NULL;
  tpool->tail = NULL;
  pthread_mutex_init(&tpool->pt_lock, NULL);
  pthread_cond_init(&tpool->pt_wa, NULL);

  for (int i = 0; i < N_THREADS; i++) {

    if (pthread_create(&tpool->threads[i], NULL, worker_loop, tpool) != 0) {

      perror("tpool_create: pthread_create");
      tpool->running = 0;

      pthread_cond_broadcast(&tpool->pt_wa);
      for (int j = 0; j < i; j++)
        pthread_join(tpool->threads[j], NULL);

      free(tpool);
      return NULL;
    }
  }

  printf("[threadpool] %d workers started\n", N_THREADS);
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

  pthread_cond_signal(&tpool->pt_wa);

  pthread_mutex_unlock(&tpool->pt_lock);
}

void tpool_destroy(ThreadPool *tpool) {

  if (tpool == NULL)
    return;

  pthread_mutex_lock(&tpool->pt_lock);
  tpool->running = 0;
  pthread_cond_broadcast(&tpool->pt_wa);
  pthread_mutex_unlock(&tpool->pt_lock);

  for (int i = 0; i < N_THREADS; i++)
    pthread_join(tpool->threads[i], NULL);

  pthread_cond_destroy(&tpool->pt_wa);
  pthread_mutex_destroy(&tpool->pt_lock);
  free(tpool);
  printf("[threadpool] destroyed\n");
}
