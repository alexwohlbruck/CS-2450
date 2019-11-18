//You shouldn't need any prototypes, #defines, or 
//typedefs in main.cc.  They should all be in structures.h
//and the following line will automatically bring them
//into main.cc.
#include "structures.h"

int main()
{
	dvd_t dvd0, dvd1, dvd2;
	dvd_t collection[NUM_DVDS];
	
	//Getting first dvd into dvd0.
	getOneDVD(&dvd0);

	//Getting next two dvds (dvd1 and dvd2) using an array.
	collection[0] = dvd1;
	collection[1] = dvd2;
	getCollection(collection, 2);
	
	//Printing the first dvd.
	printOneDVD(dvd0);

	//printing from the array.
	printCollection(collection, 2);

	return 0;
}

