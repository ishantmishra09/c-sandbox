#include "../include/dynarr_void.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char name[32];
  float score;
} player_t;

static void fmt_player(const void *e) {

  const player_t *p = (const player_t *)e;
  printf("{%s: %.1f}", p->name, p->score);
}

int main(void) {
  dynarr_t players;
  if (!dynarr_init(&players, sizeof(player_t))) {

    fputs("Failed to initialise array\n", stderr);
    return EXIT_FAILURE;
  }

  player_t players_list[] = {
      {"Sachin", 97.5f},
      {"Virat", 83.0f},
      {"MSD", 91.2f},
      {"IM", 76.8f},
  };

  for (size_t i = 0; i < sizeof(players_list) / sizeof(players_list[0]); i++) {

    if (!dynarr_push(&players, &players_list[i])) {

      fputs("Push failed\n", stderr);
      dynarr_free(&players);
      return EXIT_FAILURE;
    }
  }
  dynarr_fmt(&players, fmt_player);

  if (!dynarr_pop(&players, NULL)) {

    fputs("Pop failed\n", stderr);
    dynarr_free(&players);
    return EXIT_FAILURE;
  }
  dynarr_fmt(&players, fmt_player);

  player_t player;
  if (!dynarr_get(&players, 0, &player)) {

    fputs("Get failed\n", stderr);
    dynarr_free(&players);
    return EXIT_FAILURE;
  }
  printf("First player: %s (%.1f)\n", player.name, player.score);

  player_t updated = {"Alice", 100.0f};
  if (!dynarr_set(&players, 0, &updated)) {

    fputs("Set failed\n", stderr);
    dynarr_free(&players);
    return EXIT_FAILURE;
  }
  dynarr_fmt(&players, fmt_player);

  dynarr_free(&players);
  return EXIT_SUCCESS;
}
