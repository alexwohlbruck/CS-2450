#include <stdio.h>

int main()
{
	int i = 0;
	char d[] = "nnnn1234";
	char h[] = "nnnnnnnabcd";

	//Read 1200 as decimal from string d.
	char *p = &d[4];
	sscanf(p, "%d", &i);
	printf("%d\n", i);
	//Prints 1234

	//Read abcd as hexadecimal from string h.
	p = &h[7];
	sscanf(p, "%x", &i);
	printf("%d\n", i);
	//Prints 43981	

	return 0;
}


