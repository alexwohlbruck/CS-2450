#include <stdio.h>
#include <string.h>

#include "assemble.cc"

void assemble(char[]);

int firstPass(FILE* file, int* labels, int lc);
void printLabels(int* labels);

int main(int argc, char **argv)
{

	if (argc > 1)
	{
		assemble(argv[1]);
	}
	else
	{
		char filename[128] = "../inputs/testAdd.asm";		
		assemble(filename);
	}
}

int test() {
	return 1;
}