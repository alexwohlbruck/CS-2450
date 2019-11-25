#include <stdio.h>
#include <string.h>

#include "assemble.cc"

void assemble(char[]);

int main() {  

  char line[] = "br l1";
  int labels[] = {12292, 12302, 12311, 12312, 12313, -1, -1, -1, -1, -1};
  int result = getBr(line, labels, 12311);

  // char line[] = "br l1";
  // int result = getBr(line);

  printf("%x\n", result);
}
/* 
0E09
0000 111 000001001 #9

0E12
0000 111 000010010 #18
*/