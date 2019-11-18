#include <stdio.h>

void printIP(int ip) {

  for (int i = 1; i <= 4; i++) {
    int mask = (ip & 0xFF000000) >> 24;
    printf("%d", mask < 0 ? mask * -1 : mask);
    if (i != 4) printf(".");
    ip = ip << 8;
  }

  printf("\n");
}

int reverseEndian(int num) {
  int reverse = 0x00000000;

  for (int i = 1; i <= 4; i++) {
    reverse = reverse | (num & 0xFF000000);
    num = num << 8;
    if (i < 4) reverse = (reverse >> 8) & 0x00FFFFFF;
  }

  return reverse;
}

int countGroups(int num) {

  int count = 0;
  int countingGroup = 0;

  for (int i = 0; i < 32; i++) {

    int bit = num & 1;

    if (countingGroup == 1 && bit == 0) {
      countingGroup = 0;
    }
    else if (countingGroup == 0 && bit == 1) {
      count++;
      countingGroup = 1;
    }

    num = num >> 1;
  }

  return count;
}