#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int ctoi(char c) {
  return c - 48; // where ascii numbers start (i don't like it but hey)
}

int dissectline(char* str, size_t len) {
  char first = 0;
  char last = 0;

  for (size_t i = 0; i < len; i++) {
    char c = str[i];
    if (c == '\n' || c == '\0') break;

    if (isdigit(c)) {
      if (first == 0) first = c;
      last = c;
    }
  }
  if (first == 0) return 0;

  return (ctoi(first) * 10) + ctoi(last);
}

int parse(FILE* fptr) {
  int total = 0;

  size_t len = 8; // default
  char* str = malloc(8);

  while (1) {
    getline(&str, &len, fptr);
    if (feof(fptr)) break;

    total += dissectline(str, len);
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
