//The following is a sample main.cc.  Modify it to test your program.
//Make sure to test thoroughly.  All the pieces below have to be included.

#include <stdio.h>

//These are called prototypes and they have to be here.
//Any function in assign1.cc that you also call from main must be listed here. 
//For now, make sure you include the following 3 lines in main.cc
void printIP(int);
int reverseEndian(int);
int countGroups(int);

//This function will run when you run your program from the command line.
int main(void)
{
	int answer;
	
	//For the print functions you will have to verify that the correct
	//output was printed yourself.
	printf("Verify the following manually\n");
	printIP(33783932);
	printIP(0x1234567A);

	//Test reverseEndian.  Note that this only prints tests that fail.
	//Put an else in if you want to see tests that pass also.
	
	//reverseEndian test 1
	answer = reverseEndian(0x12345678); 
	if (answer != 0x78563412)
	{
		printf("Failed reverseEndian test 1\n");
		printf("Expected: %X\t", 0x78563412);
		printf("Found: %X\n", answer);		
	}
	
	//**** PUT MORE REVERSE ENDIAN TESTS HERE ****

	//Test countGroups. Note that this only prints tests that fail.
	//Put an else in if you want to see tests that pass also.

	//countGroups test 1
	answer = countGroups(0x8000000F);
	if (answer != 2)
	{
		printf("Failed countGroups test 1\n");
		printf("Expected: %d\t", 2);
		printf("Found: %d\n", answer);		
	}

	//**** PUT MORE COUNT GROUPS TESTS HERE ****

	return 0;
}

