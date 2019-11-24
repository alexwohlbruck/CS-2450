#include <stdio.h>
#include <string.h>

#include "assemble.cc"

void assemble(char[]);

int main() {

  char line[] = "trap x22";
  int trap = getTrap(line);

  printf("%x\n", trap);
}