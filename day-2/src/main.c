#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *SET_DILIM = ";";     // delimiter for sets of cubes shown
const char *CUBE_DILIM = ", \n"; // split on ', ' and newlines

const struct GameMaxes_s {
  int max_red;
  int max_green;
  int max_blue;
} game_maxes_default = {0, 0, 0};
typedef struct GameMaxes_s GameMaxes; // lets us omit "struct" keywords below

// Will return 0 on failure
int parse_color(int value, char *color, GameMaxes *maxes) {
  if (!strcmp(color, "red")) {
    if (value > maxes->max_red)
      maxes->max_red = value;
  } else if (!strcmp(color, "green")) {
    if (value > maxes->max_green)
      maxes->max_green = value;
  } else if (!strcmp(color, "blue")) {
    if (value > maxes->max_blue)
      maxes->max_blue = value;
  } else {
    return 0;
  }

  return 1;
}

GameMaxes parse_game(char *start) {
  GameMaxes maxes = game_maxes_default;

  char *outer_ptr;
  char *set_split = strtok_r(start, SET_DILIM, &outer_ptr);

  while (set_split) {
    char *inner_ptr;
    char *cube_split = strtok_r(set_split, CUBE_DILIM, &inner_ptr);

    int tmp = -1; // store last number seen
    while (cube_split) {
      // get value or color of cube
      int maybe_value = atoi(cube_split);

      if (maybe_value) {
        tmp = maybe_value;
      } else {
        if (tmp == -1) {
          printf("ERROR: tmp is -1 but we got color string: %s\n", cube_split);
          break;
        }

        if (!parse_color(tmp, cube_split, &maxes)) {
          printf("ERROR: expected color but got '%s'\n", cube_split);
          break;
        }
      }

      cube_split = strtok_r(NULL, CUBE_DILIM, &inner_ptr);
    }

    set_split = strtok_r(NULL, SET_DILIM, &outer_ptr);
  }

  return maxes;
}

int parse_file(FILE *fptr) {
  size_t len = 24;
  char *str = malloc(len);

  int game_num = 1;
  int game_power_sum = 0;
  while (1) {
    getline(&str, &len, fptr);
    if (feof(fptr))
      break;

    // move up until just after ':' of "Game XYZ: "
    char *start = NULL;
    for (int i = 0; i + 2 < strlen(str); i++) {
      if (str[i] == ':') {
        start = str + i + 2;
        break;
      }
    }

    if (!start) { // empty line or incorrect formatting
      printf("failed parsing game %d! empty line or bad format?\n", game_num);
      break;
    }

    GameMaxes info = parse_game(start);
    game_power_sum += info.max_red * info.max_green * info.max_blue;

    game_num++;
  }

  free(str);
  return game_power_sum;
}

int main() {
  FILE *fptr = fopen("day-2/input.txt", "r");

  if (!fptr) {
    printf("Could not read input.txt!\n");
    return 1;
  }

  printf("%d\n", parse_file(fptr));

  fclose(fptr);
  return 0;
}
