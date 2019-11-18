#include <stdio.h>
//Precision of calculate double returns are +- .000001
#define DELTA 0.000001

//Prototypes from your assign.cc file.  You must tell main 
//about the functions from your assign file.
double getPositiveAverage(double *values, int num);
int countRangeValues(double values[], int num, double value);
double getMaxAbsolute(double values[], int num);
int countInverses(int values[], int num);
int getMaxCount(double values[], int num);

//Prototypes from this file.  These are prototypes of test
//and helper functions found further down in this file.
void testGetPositiveAverageTest1();
void testGetPositiveAverageTest2();
void testCountRangeValuesTest1();
void testCountRangeValuesTest2();
void testGetMaxAbsoluteTest1();
void testGetMaxAbsoluteTest2();
void testCountInversesTest1();
void testCountInversesTest2();
void testGetMaxCountTest1();
void testGetMaxCountTest2();

//Helper functions
void printDoubleArray(double[], int);
void printIntArray(int[], int);

int main()
{
	int a[] = {1, -1, 1};
	int num = 3;
    int value = 0;
	value = countInverses(a, num);
	printf("Using array: ");
	printIntArray(a, num);
	printf("Result: %d\n", value);
	
	
// 	testGetPositiveAverageTest1();
// 	testGetPositiveAverageTest2();
// 	testCountRangeValuesTest1();
// 	testCountRangeValuesTest2();
// 	testGetMaxAbsoluteTest1();
// 	testGetMaxAbsoluteTest2();
// 	testCountInversesTest1();
// 	testCountInversesTest2();
// 	testGetMaxCountTest1();
// 	testGetMaxCountTest2();
	return 0;
}

void testGetPositiveAverageTest1()
{
	double value = 0.0;
	double exp = 0.0;
	
	double a[] = {2.2, 3.8, 4.2, 4.8};
	value = getPositiveAverage(a, 4);
	exp = 3.75;
	if (value < exp - DELTA || value > exp + DELTA)
	{
		printf("Using array: ");
		printDoubleArray(a, 4);
		printf("Failed getPositiveAverage.\n");
		printf("Expected %f (+- %.6f)\n", exp, DELTA);
		printf("Found  %f (+- %.6f)\n", value, DELTA);
	}
	
	//ADD MORE TESTS!!!  
	//SIMPLY PASSING THE ABOVE DOES NOT GUARANTEE WEB-CAT WILL PASS!!!
	//YOU MUST THOUROUGHLY TEST YOUR OWN PROGRAMS!!!
}

void testGetPositiveAverageTest2()
{
	double value = 0.0;
	double exp = 0.0;
	
	double a[] = {-1.0, 2.2, 3.8, 4.2, 4.8, 5.5, 6.5};
	value = getPositiveAverage(a, 7);
	exp = 4.5;
	if (value < exp - DELTA || value > exp + DELTA)
	{
		printf("Using array: ");
		printDoubleArray(a, 7);
		printf("Failed getPositiveAverage.\n");
		printf("Expected %f (+- %.6f)\n",exp, DELTA);
		printf("Found  %f (+- %.6f)\n", value, DELTA);
	}
	
	//ADD MORE TESTS!!!  
	//SIMPLY PASSING THE ABOVE DOES NOT GUARANTEE WEB-CAT WILL PASS!!!
	//YOU MUST THOUROUGHLY TEST YOUR OWN PROGRAMS!!!
}

void testCountRangeValuesTest1()
{
	int value = 0;
	int exp = 0;
	
	double a[] = {0.0, 1.2, 1.5, 1.8, 2.0, 2.1, 2.2, 
		2.4, 2.7, 3.0, 3.1, 3.2, 3.5, 3.7};
	value = countRangeValues(a, 14, 2.5);
	exp = 5;
	if (value != exp)
	{
		printf("Using array: ");
		printDoubleArray(a, 14);
		printf("Failed countRangeValues using 2.5.\n");
		printf("Expected: %d\n", exp);
		printf("Found   : %d\n", value);
	}
	
	//ADD MORE TESTS!!!  
	//SIMPLY PASSING THE ABOVE DOES NOT GUARANTEE WEB-CAT WILL PASS!!!
	//YOU MUST THOUROUGHLY TEST YOUR OWN PROGRAMS!!!	
}

void testCountRangeValuesTest2()
{
	int value = 0;
	int exp = 0;
	
	//-1.5 <= value < -0.5
	double a[] = {0.0, 1.2, -0.499, 1.8, -1.9, -0.51, -0.5, -1.5001,
		-100.2, 3.0, 30.1, -1.5, -1.0, 3.5, -1.499};
	value = countRangeValues(a, 15, -1.0);
	exp = 4;
	if (value != exp)
	{
		printf("Using array: ");
		printDoubleArray(a, 15);
		printf("Failed countRangeValues using -1.0.\n");
		printf("Expected: %d\n", exp);
		printf("Found   : %d\n", value);
	}
	
	//ADD MORE TESTS!!!  
	//SIMPLY PASSING THE ABOVE DOES NOT GUARANTEE WEB-CAT WILL PASS!!!
	//YOU MUST THOUROUGHLY TEST YOUR OWN PROGRAMS!!!	
}

void testGetMaxAbsoluteTest1()
{
	double value = 0;
	double exp = 0;
	
	double a[] = {-1.0, -1.2, -1.5, -1.8, -2.0, -2.1, -2.2,
		2.4, 2.7, 3.0, 3.1, 3.2, 3.5, 0.0};
	value = getMaxAbsolute(a, 14);
	exp = 3.5;
	if (value != exp)
	{
		printf("Using array: ");
		printDoubleArray(a, 14);
		printf("Failed getMaxAbsolute.\n");
		printf("Expected: %f\n", exp);
		printf("Found   : %f\n", value);
	}
	
	//ADD MORE TESTS!!!  
	//SIMPLY PASSING THE ABOVE DOES NOT GUARANTEE WEB-CAT WILL PASS!!!
	//YOU MUST THOUROUGHLY TEST YOUR OWN PROGRAMS!!!	
}

void testGetMaxAbsoluteTest2()
{
	double value = 0;
	double exp = 0;
	
	double a[] = {0.0, 1.2, -0.499, 1.8, -1.9, -0.51, -0.5, -1.5001,
		-100.2, 3.0, 30.1, -1.5, -1.0, 3.5, -1.499};
	value = getMaxAbsolute(a, 15);
	exp = -100.2;
	if (value != exp)
	{
		printf("Using array: ");
		printDoubleArray(a, 15);
		printf("Failed getMaxAbsolute.\n");
		printf("Expected: %f\n", exp);
		printf("Found   : %f\n", value);
	}
	
	//ADD MORE TESTS!!!  
	//SIMPLY PASSING THE ABOVE DOES NOT GUARANTEE WEB-CAT WILL PASS!!!
	//YOU MUST THOUROUGHLY TEST YOUR OWN PROGRAMS!!!	
}

void testCountInversesTest1()
{
	int value = 0;
	int exp = 0;
	
	int a[] = {0, -1, 1, -2, 2, 3, 3, -4, -4, 6, -6};
	value = countInverses(a, 11);
	exp = 3;
	if (value != exp)
	{
		printf("Using array: ");
		printIntArray(a, 11);
		printf("Failed countInverses.\n");
		printf("Expected: %d\n", exp);
		printf("Found   : %d\n", value);
	}
	
	//ADD MORE TESTS!!!  
	//SIMPLY PASSING THE ABOVE DOES NOT GUARANTEE WEB-CAT WILL PASS!!!
	//YOU MUST THOUROUGHLY TEST YOUR OWN PROGRAMS!!!	
}
void testCountInversesTest2()
{
	int value = 0;
	int exp = 0;
	
	int a[] = {0, 1, 3, 5, 7, 9, 11, 10};
	value = countInverses(a, 8);
	exp = 0;
	if (value != exp)
	{
		printf("Using array: ");
		printIntArray(a, 8);
		printf("Failed countInverses.\n");
		printf("Expected: %d\n", exp);
		printf("Found   : %d\n", value);
	}
	
	//ADD MORE TESTS!!!  
	//SIMPLY PASSING THE ABOVE DOES NOT GUARANTEE WEB-CAT WILL PASS!!!
	//YOU MUST THOUROUGHLY TEST YOUR OWN PROGRAMS!!!	
}

void testGetMaxCountTest1()
{
	int value = 0;
	int exp = 0;
	
	double a[] = {-5.4, -3.1, -5.9, 4.1, -4.5, 4.2, -3.2, -5.55};
	value = getMaxCount(a, 8);
	exp = 2;
	if (value != exp)
	{
		printf("Using array: ");
		printDoubleArray(a, 8);
		printf("Failed getMaxCount.\n");
		printf("Expected: %d\n", exp);
		printf("Found   : %d\n", value);
	}
	
	//ADD MORE TESTS!!!  
	//SIMPLY PASSING THE ABOVE DOES NOT GUARANTEE WEB-CAT WILL PASS!!!
	//YOU MUST THOUROUGHLY TEST YOUR OWN PROGRAMS!!!	
}

void testGetMaxCountTest2()
{
	int value = 0;
	int exp = 0;
	
	double a[] = {-5.3, -4.4, -5.5, 1.0, -5.1, 5.2, 1.0, -5.0, 1.0};
	value = getMaxCount(a, 9);
	exp = 3;
	if (value != exp)
	{
		printf("Using array: ");
		printDoubleArray(a, 9);
		printf("Failed getMaxCount.\n");
		printf("Expected: %d\n", exp);
		printf("Found   : %d\n", value);
	}
	
	//ADD MORE TESTS!!!  
	//SIMPLY PASSING THE ABOVE DOES NOT GUARANTEE WEB-CAT WILL PASS!!!
	//YOU MUST THOUROUGHLY TEST YOUR OWN PROGRAMS!!!	
}

void printDoubleArray(double myArray[], int num)
{
	printf("{");
	for (int i = 0; i < num - 1; i++)
	{
		printf("%.3f,", myArray[i]);
	}
	printf("%.3f}\n", myArray[num - 1]);	
}

void printIntArray(int myArray[], int num)
{
	printf("{");
	for (int i = 0; i < num - 1; i++)
	{
		printf("%d,", myArray[i]);
	}
	printf("%d}\n", myArray[num - 1]);	
}


