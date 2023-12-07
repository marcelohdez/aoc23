#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRLEN(arr) (sizeof(arr)/sizeof(*arr))

const char* WORDS[] = {
  "zero",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine"
};

int ctoi(char c) {
  return c - '0'; // highly sophistaced ascii to integer conversion /s
}

// str is assumed null terminated (for strlen)
int dissect(char* str) {
  char* firstptr = NULL;
  char* lastptr = NULL;
  int first = 0;
  int last = 0;

  for (int i = 0; i < ARRLEN(WORDS); i++) {
    char* substrptr = strstr(str, WORDS[i]);
    if (!substrptr) continue;

    while (substrptr) {
      if (firstptr == NULL || substrptr < firstptr) {
        firstptr = substrptr;
        first = i;
      }

      if (lastptr == NULL || substrptr > lastptr) {
        lastptr = substrptr;
        last = i;
      }

      substrptr = strstr(substrptr + strlen(WORDS[i]), WORDS[i]);
    }
  }

  for (int i = 0; i < strlen(str); i++) {
    if (isdigit(str[i])) {
      if (firstptr == NULL || &str[i] < firstptr) {
        firstptr = &str[i];
        first = ctoi(str[i]);
      }

      if (lastptr == NULL || &str[i] > lastptr) {
        lastptr = &str[i];
        last = ctoi(str[i]);
      }
    }
  }

  return (first * 10) + last;
}

int parse(FILE* fptr) {
  int total = 0;

  size_t len = 8; // default
  char* str = malloc(len);

  while (1) {
    getline(&str, &len, fptr);
    if (feof(fptr)) break;

    total += dissect(str);
  }

  free(str);
  return total;
}

int main() {
  FILE* fptr;

  fptr = fopen("day-1/input.txt", "r");
  if (!fptr) {
    printf("Could not read input.txt!\n");
    return 1;
  }

  printf("%d\n", parse(fptr));

  fclose(fptr);
  return 0;
}
