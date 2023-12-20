#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *SET_DILIM = ";"; // delimiter for sets of cubes shown
const char *CUBE_DILIM = ", ";

const int RED_LIMIT = 12;
const int GREEN_LIMIT = 13;
const int BLUE_LIMIT = 14;

// ignores newline on s1 when comparing to s2
int strcmpnl1(char *s1, const char *s2) {
  int len = strlen(s1);
  char *trimmed1 = s1;

  if (s1[len - 1] == '\n') {
    s1[len - 1] = '\0'; // replace newline with null so strcpy stops there

    trimmed1 = malloc(len - 1);
    strcpy(trimmed1, s1);

    s1[len - 1] = '\n'; // put newline back
  }

  int res = strcmp(trimmed1, s2);

  if (trimmed1 != s1)
    free(trimmed1);

  return res;
}

int is_game_possible(char *start) {
  char *outer_ptr;
  char *set_split = strtok_r(start, SET_DILIM, &outer_ptr);

  while (set_split) {
    char *inner_ptr;
    char *cube_split = strtok_r(set_split, CUBE_DILIM, &inner_ptr);

    int tmp = -1;
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

        if (!strcmpnl1(cube_split, "red")) {
          if (tmp > RED_LIMIT)
            return 0;
        } else if (!strcmpnl1(cube_split, "green")) {
          if (tmp > GREEN_LIMIT)
            return 0;
        } else if (!strcmpnl1(cube_split, "blue")) {
          if (tmp > BLUE_LIMIT)
            return 0;
        } else {
          printf("ERROR: expected color but got '%s'\n", cube_split);
          break;
        }
      }

      cube_split = strtok_r(NULL, CUBE_DILIM, &inner_ptr);
    }

    set_split = strtok_r(NULL, SET_DILIM, &outer_ptr);
  }

  return 1;
}

int parse(FILE *fptr) {
  size_t len = 16;
  char *str = malloc(len);

  int game_num = 1;
  int possible_id_sum = 0;
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

    if (!start) // empty line at end of file or I done goofed
      break;

    if (is_game_possible(start)) {
      possible_id_sum += game_num;
    }

    game_num++;
  }

  free(str);
  return possible_id_sum;
}

int main() {
  FILE *fptr = fopen("day-2/input.txt", "r");

  if (!fptr) {
    printf("Could not read input.txt!\n");
    return 1;
  }

  printf("%d\n", parse(fptr));

  fclose(fptr);
  return 0;
}
