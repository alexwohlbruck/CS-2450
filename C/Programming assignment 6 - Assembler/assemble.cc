#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LIMIT 10000
#define LINE_SIZE 128

// For part 1
void assemble(char[]);
int findOrigin(FILE*);
int firstPass(FILE*, int[], int);
void printLabels(int[]);

// For part 2
int getAdd(char[]);
int getAnd(char[]);
int getTrap(char[]);
int getNot(char[]);
int getLd(char[], int[], int);
int getLdr(char[]);
int getSt(char[], int[], int);
int getStr(char[]);
int getBr(char[], int[], int);
int getIt(int, char[]);
int secondPass(FILE*, int[], int);

// Helper functions
void trim(char*);
char isspace(unsigned char);
void strlower(char*);
int strequals(const char*, const char*);
void rc(char*, int);
const char* chartobin(char, int);
char* inttobin(int, int);
int bintoint(char*);

void assemble(char filename[]) {
	//Open the file for reading
  FILE *infile = fopen( filename , "r" );
    
	if (infile != NULL)  {    
		//Create labels array and set all elements to -1.  
		int labels[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
		
		int lc = findOrigin(infile);
		
		if (lc > -1) {
			//Read in label values
			if (!firstPass(infile, labels, lc)) {
				//Show the labels.
				// printLabels(labels);
				
				//The following is for part 2
				secondPass(infile, labels, lc);
			}
		}
		
		//Close the file
		fclose(infile);
		
	} 
	else {	
		printf("Can't open input file.\n");		
	}
}

int findOrigin(FILE *infile) {
	//Each trip through the while loop will read the next line of infile
	//into the line char array as a null terminated string.
	char line[LINE_SIZE];
	
	//The variable lineCount keeps track of how many lines have been read.
	//It is used to guard against infinite loops.  Don't remove anything
	//that has to do with linecount.
	int lineCount = 0; //
	
	//For getting out of the while loop.
	int done = 0;
	
	//For getting rid of the trailing newline
	char c = 0;
	
	//Read lines until EOF reached or LIMIT lines read.  Limit prevent infinite loop.
	while (!done && lineCount < LIMIT && fscanf(infile, "%[^\n]s", line) != EOF) {
		lineCount++;
		
		fscanf(infile, "%c", &c);  //Get rid of extra newline.
		
		//At this point, line contains the next line of the ASM file.
		//Put your code here for seeing if line is an origin.
		//Options:
		//	1. line is an origin (save value, set done = 1).  
		//  2. line is a blank line (skip).
		//  3. line is a comment (skip).
		//  4. line is anything else (print error, set done = 1).	

		trim(line);
		strlower(line);

		// Get first 5 chars of line
		char findOrigin[6];
		strncpy(findOrigin, line, 5);
		findOrigin[5] = '\0';

		int lineIsEmpty = strlen(line) == 0;
		int lineIsComment = line[0] == ';';
		int lineIsOrigin = strcmp(findOrigin, ".orig") == 0;

		if (!lineIsEmpty && !lineIsComment && lineIsOrigin) {
			// Found ".orig", search the rest of line
			char *address = line;
			address += 5; // Remove first 5 chars (.orig) from line
			trim(address);

			int output = 0;

			if (address[0] == 'x') {
				// Address is a hexadecimal value
				address += 1; // Remove x from address

				sscanf(address, "%x", &output);
			}
			else {
				// Address is a decimal value
				sscanf(address, "%d", &output);
			}

			if (output > 0xFFFF) {
				printf("ERROR 2: Bad origin address. Address too big for 16 bits.\n");
				return -1;
			}

			return output;
		}
		
		//Set the line to empty string for the next pass.
		line[0] = 0;
	}
	
	
	//At this point you must decide if an origin was found or not.
	//How you do this is up to you.
	//If a good origin was found, check the size.  
	//		if it is too big, print an error and return -1.
	//      if it is not too big, return the value.
	//If a good origin was NOT found, print the error message and return -1.

	printf("ERROR 1: Missing origin directive. Origin must be first line in program.\n");
	return -1;
}

int firstPass(FILE *infile, int labels[], int lc) {
	//Create a while loop similar to findOrigin.
	//You can rewind if you want to but you shouldn't have to.

	//Each trip through the while loop will read the next line of infile
	//into the line char array as a null terminated string.
	char line[LINE_SIZE];
	
	//The variable lineCount keeps track of how many lines have been read.
	//It is used to guard against infinite loops.  Don't remove anything
	//that has to do with linecount.
	int lineCount = 0; //
	
	//For getting out of the while loop.
	int done = 0;
	
	//For getting rid of the trailing newline
	char c = 0;
	
	//Read lines until EOF reached or LIMIT lines read.  Limit prevent infinite loop.
	while (!done && lineCount < LIMIT && fscanf(infile, "%[^\n]s", line) != EOF) {
		lineCount++;
		
		fscanf(infile, "%c", &c);  //Get rid of extra newline.
		
		//Read a line.

		trim(line);
		strlower(line);

		int lineIsEmpty = strlen(line) == 0;
		int lineIsComment = line[0] == ';';
		int lineIsOrigin = strequals(line, ".orig");
		int lineIsEnd = strequals(line, ".end");
		int lineIsLabel = line[0] == 'l' && line[1] >= '0' && line[1] <= '9' && strlen(line) == 2;
		int lineIsFill = strstr(line, ".fill") != NULL;

		int lineIsAdd = strequals(line, "add");
		int lineIsAnd = strequals(line, "and");
		int lineIsNot = strequals(line, "not");
		int lineIsLd = strequals(line, "ld");
		int lineIsLdr = strequals(line, "ldr");
		int lineIsSt = strequals(line, "st");
		int lineIsStr = strequals(line, "str");
		int lineIsBr = strequals(line, "br");
		int lineIsTrap = strequals(line, "trap");
		int lineIsInstruction = lineIsAdd || lineIsAnd || lineIsNot || lineIsLd || lineIsLdr || lineIsSt || lineIsStr || lineIsBr || lineIsTrap;


		// If the line is a comment, a blank line or the .orig directive, don't do anything.
		if (!lineIsEmpty && !lineIsComment && !lineIsOrigin) {
			
			int labelNum = 0;
			
			// If the line is a label on a line by itself, save the lc to the labels array at the appropriate position.
			//	For example, if L3, store the lc value in labels[3].
			if (lineIsFill || lineIsLabel) {
				char* num = &line[1];
				sscanf(num, "%d", &labelNum);
				labels[labelNum] = lc;
			}

			// If the line is a label followed by .fill, save the lc in labels AND increment lc.
			if (lineIsFill) {
				lc++;
			}

			// If the line is one of the allowed instructions (ADD, AND, NOT, LD, LDR, ST, STR, BR, and TRAP) increment the lc.
			// I am so sorry that I did this
			if (lineIsInstruction) {
				lc++;
			}

			// If the line is anything else print the unknown instruction error and return -1.
			else if (!lineIsLabel && !lineIsFill && !lineIsEnd) {
				printf("ERROR 3: Unknown instruction.\n");
				return -1;
			}
			
			//If the line is .end, return 0 for done with no error.
			if (lineIsEnd) {
				done = 1;
				return 0;
			}
			
		}

		//Set the line to empty string for the next pass.
		line[0] = 0;
	}

	// If the end of file is reached before .end is found print the error and return -1.
	if (!done) {
		printf("ERROR 4: Missing end directive.\n");
		return -1;
	}
}

int secondPass(FILE *infile, int labels[], int lc) {
	rewind(infile);

	//Create a while loop similar to findOrigin.
	//You can rewind if you want to but you shouldn't have to.

	//Each trip through the while loop will read the next line of infile
	//into the line char array as a null terminated string.
	char line[LINE_SIZE];
	
	//The variable lineCount keeps track of how many lines have been read.
	//It is used to guard against infinite loops.  Don't remove anything
	//that has to do with linecount.
	int lineCount = 0; //
	
	//For getting out of the while loop.
	int done = 0;
	
	//For getting rid of the trailing newline
	char c = 0;
	
	//Read lines until EOF reached or LIMIT lines read.  Limit prevent infinite loop.
	while (!done && lineCount < LIMIT && fscanf(infile, "%[^\n]s", line) != EOF) {
		lineCount++;
		
		fscanf(infile, "%c", &c);  //Get rid of extra newline.
		
		//Read a line.

		trim(line);
		strlower(line);

		int lineIsEmpty = strlen(line) == 0;
		int lineIsComment = line[0] == ';';
		int lineIsOrigin = strequals(line, ".orig");
		int lineIsEnd = strequals(line, ".end");
		int lineIsLabel = line[0] == 'l' && line[1] >= '0' && line[1] <= '9' && strlen(line) == 2;
		int lineIsFill = strstr(line, ".fill") != NULL;

		int lineIsAdd = strequals(line, "add");
		int lineIsAnd = strequals(line, "and");
		int lineIsNot = strequals(line, "not");
		int lineIsLd = strequals(line, "ld");
		int lineIsLdr = strequals(line, "ldr");
		int lineIsSt = strequals(line, "st");
		int lineIsStr = strequals(line, "str");
		int lineIsBr = strequals(line, "br");
		int lineIsTrap = strequals(line, "trap");
		int lineIsInstruction = lineIsAdd || lineIsAnd || lineIsNot || lineIsLd || lineIsLdr || lineIsSt || lineIsStr || lineIsBr || lineIsTrap;

		// Logic here

		if (!lineIsEmpty && !lineIsComment && !lineIsOrigin && !lineIsLabel) {
			// Ignore line
		}
		else if (lineIsFill) {
			lc++;
			printf("0000\n");
		}
		else if (lineIsEnd) {
			return 0;
		}
		
		if (lineIsInstruction) {
			lc++;
			
			// Call appropriate method to deal with the instruction
			if (lineIsAdd) {
				getAdd(line);
			} else if (lineIsAnd) {
				getAnd(line);
			} else if (lineIsNot) {
				getNot(line);
			} else if (lineIsLd) {
				getLd(line, labels, lc);
			} else if (lineIsLdr) {
				getLdr(line);
			} else if (lineIsSt) {
				getSt(line, labels, lc);
			} else if (lineIsStr) {
				getStr(line);
			} else if (lineIsBr) {
				getBr(line, labels, lc);
			} else if (lineIsTrap) {
				getTrap(line);
			}
		}
	}
}

int getAdd(char line[]) {
	/* 
		ADD R1, R2, R3
		0001 DR(3) SR1(3) 0 00 SR2(3)
		
		ADD R0, R3, #10
		0001 DR(3) SR1(3) 1 imm5(5)
	 */
	char* output = (char*) malloc(16 * sizeof(char));
	strcat(output, "0001");

	rc(line, 3); // Remove "ADD"

	char DR = line[1];
	strcat(output, chartobin(DR, 3)); // Append DR to output
	
	rc(line, 2); // Remove DR
	rc(line, 1); // Remove ","

	char SR1 = line[1];
	strcat(output, chartobin(SR1, 3)); // Append SR1 to output

	rc(line, 2); // Remove SR1
	rc(line, 1); // Remove ","
	

	if (line[0] == 'r') {
		// Normal mode 

		int SR2 = line[1];
		strcat(output, "000"); // Append placeholder to output
		strcat(output, chartobin(SR2, 3)); // Append SR2 to output
	}
	else if (line[0] == '#') {
		// Immediate mode

		rc(line, 1); // Remove "#"
		
		int num = 0;
		sscanf(line, "%d", &num); // Scan number into int

		strcat(output, "1");
		strcat(output, inttobin(num, 5)); // Append immediate val to output
	}

	return bintoint(output);
}

int getAnd(char line[]) {
	/*
		AND R1, R1, R2
		0101 DR(3) SR1(3) 0 00 SR2(3)

		AND R0, R3, #-10
		0101 DR(3) SR1(3) 1 imm5(5)
	*/
	char* output = (char*) malloc(16 * sizeof(char));
	strcat(output, "0101");

	rc(line, 3); // Remove "ADD"

	char DR = line[1];
	strcat(output, chartobin(DR, 3)); // Append DR to output
	
	rc(line, 2); // Remove DR
	rc(line, 1); // Remove ","

	char SR1 = line[1];
	strcat(output, chartobin(SR1, 3)); // Append SR1 to output

	rc(line, 2); // Remove SR1
	rc(line, 1); // Remove ","
	

	if (line[0] == 'r') {
		// Normal mode 

		int SR2 = line[1];
		strcat(output, "000"); // Append placeholder to output
		strcat(output, chartobin(SR2, 3)); // Append SR2 to output
	}
	else if (line[0] == '#') {
		// Immediate mode

		rc(line, 1); // Remove "#"
		
		int num = 0;
		sscanf(line, "%d", &num); // Scan number into int

		strcat(output, "1");
		strcat(output, inttobin(num, 5)); // Append immediate val to output
	}

	return bintoint(output);
}

int getTrap(char line[]) {
	/*
		TRAP x22
		1111 0000 trapvect8(8)
	*/

	char* output = (char*) malloc(16 * sizeof(char));
	strcat(output, "11110000");

	rc(line, 4); // Remove "TRAP"
	rc(line, 1); // Remove "x"
	
	int num = 0;
	sscanf(line, "%d", &num); // Scan number into int
	strcat(output, inttobin(num, 8)); // Append trap vector to output

	return bintoint(output);
}

int getNot(char line[]) {
	
}

int getLd(char line[], int[], int) {

}

int getLdr(char line[]) {

}

int getSt(char line[], int[], int) {

}

int getStr(char line[]) {

}

int getBr(char line[], int[], int) {

}

/* int getIt(int, char[]) {

} */


void printLabels(int labels[]) {
	printf("labels = {");
	for (int i = 0; i < 10; i++) {
		printf("%d", labels[i]);
		if (i != 9) {
			printf(", ");
		}
	}
	printf("}\n");
}

// Remove whitespace before and after a string
void trim(char *str) {
		int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((unsigned char) str[begin]))
        begin++;

    while ((end >= begin) && isspace((unsigned char) str[end]))
        end--;

    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0'; // Null terminate string.
}

// Determine whether a char is whitespace
char isspace(unsigned char c) {
  if ( c == ' '
    || c == '\f'
    || c == '\n'
    || c == '\r'
    || c == '\t'
    || c == '\v' ) 
      return 1;

  return 0;
}

// Transform string to lowercase
void strlower(char *s) {
	int i = 0;
	while (s[i]!='\0') {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			s[i] = s[i] + 32;
		}
		++i;
	}
}

// Determine if a string is equal to another based on length of the second string
int strequals(const char *s1, const char *s2) {
	return strncmp(s1, s2, strlen(s2)) == 0;
}

// 'Remove Chunk' of text from string and trim it
void rc(char *line, int num) {
	memmove(line, line + num, strlen(line));
	trim(line);
}

// Convert integer to binary string with defined length
char* inttobin(int num, int length) {
	char* output = (char*) malloc(length * sizeof(char));
	char one[] = "1";
	char zero[] = "0";

	for (int i = length - 1; i >= 0; i--) {

		if (num & 1) {
			output[i] = '1';
		} else {
			output[i] = '0';
		}
		num = num >> 1; 
	}

	return output;
}

// Convert ascii-represented integer character to 4 digit binary string
// This is definately a terrible implementation for this
const char* chartobin(char num, int length) {
	switch (num) {
		case '0':
			return inttobin(0, length);
		case '1':
			return inttobin(1, length);
		case '2':
			return inttobin(2, length);
		case '3':
			return inttobin(3, length);
		case '4':
			return inttobin(4, length);
		case '5':
			return inttobin(5, length);
		case '6':
			return inttobin(6, length);
		case '7':
			return inttobin(7, length);
		case '8':
			return inttobin(8, length);
		case '9':
			return inttobin(9, length);
	}
}

// Convert binary string to integer
int bintoint(char *binstr) {
  int output = 0;

	for (int i = 0; i < strlen(binstr); i++) {
		output = output << 1;
		if (binstr[i] == '1') {
			output += 1;
		}
	}

	return output;
}