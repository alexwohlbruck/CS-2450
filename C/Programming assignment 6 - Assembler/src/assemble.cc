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
int secondPass(FILE*, int[], int);
int getAdd(char[]);
int getAnd(char[]);
int getTrap(char[]);
int getNot(char[]);
int getLd(char[], int[], int);
int getLdr(char[]);
int getSt(char[], int[], int);
int getStr(char[]);
int getBr(char[], int[], int);

// Helper functions
void trim(char*);
char isspace(unsigned char);
void strlower(char*);
int strequals(const char*, const char*);
void rc(char*, int);
char* chartobin(char, int);
int chartoint(char);
char* inttobin(int, int);
int bintoint(char*);
int strtoint(char*);
int hextoint(char*);


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


/*** Line scans ***/

int findOrigin(FILE *infile) {
	// Each trip through the while loop will read the next line of infile
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

		int lineIsEmpty = strlen(line) == 0;
		int lineIsComment = line[0] == ';';
		int lineIsOrigin = strequals(line, ".orig");

		if (!lineIsEmpty && !lineIsComment && lineIsOrigin) {
			// Found ".orig", search the rest of line
			rc(line, 5); // Remove ".orig"

			int origin = 0;

			if (line[0] == 'x') {
				// Address is a hexadecimal value
				rc(line, 1); // Remove "x"

				sscanf(line, "%x", &origin);
			}
			else {
				// Address is a decimal value
				sscanf(line, "%d", &origin);
			}

			if (origin > 0xFFFF) {
				printf("ERROR 2: Bad origin address. Address too big for 16 bits.\n");
				return -1;
			}

			// Set the line to empty string for the next pass.
			printf("%4X\n", origin);
			return origin;
		}
		
		// Set the line to empty string for the next pass.
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
	}

	// Set the line to empty string for the next pass.
	line[0] = 0;

	// If the end of file is reached before .end is found print the error and return -1.
	if (!done) {
		printf("ERROR 4: Missing end directive.\n");
		return 0;
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

		char linecpy[LINE_SIZE];
		strcpy(linecpy, line);

		trim(linecpy);
		strlower(linecpy);

		int lineIsEmpty = strlen(linecpy) == 0;
		int lineIsComment = linecpy[0] == ';';
		int lineIsOrigin = strequals(linecpy, ".orig");
		int lineIsEnd = strequals(linecpy, ".end");
		int lineIsLabel = linecpy[0] == 'l' && linecpy[1] >= '0' && linecpy[1] <= '9' && strlen(linecpy) == 2;
		int lineIsFill = strstr(linecpy, ".fill") != NULL;

		int lineIsAdd = strequals(linecpy, "add");
		int lineIsAnd = strequals(linecpy, "and");
		int lineIsNot = strequals(linecpy, "not");
		int lineIsLd = strequals(linecpy, "ld") && linecpy[2] != 'r';
		int lineIsLdr = strequals(linecpy, "ldr");
		int lineIsSt = strequals(linecpy, "st") && linecpy[2] != 'r';
		int lineIsStr = strequals(linecpy, "str");
		int lineIsBr = strequals(linecpy, "br");
		int lineIsTrap = strequals(linecpy, "trap");
		int lineIsInstruction = lineIsAdd || lineIsAnd || lineIsNot || lineIsLd || lineIsLdr || lineIsSt || lineIsStr || lineIsBr || lineIsTrap;


		// printf("%d %d %d %d %d %d %d %d %s\n", lc, lineIsEmpty, lineIsComment, lineIsOrigin, lineIsEnd, lineIsLabel, lineIsFill, lineIsInstruction, line);

		if (lineIsInstruction) {
			lc++;

			int instruction = 0;
			
			// Call appropriate method to deal with the instruction
			if (lineIsAdd) {
				instruction = getAdd(linecpy);
			} else if (lineIsAnd) {
				instruction = getAnd(linecpy);
			} else if (lineIsNot) {
				instruction = getNot(linecpy);
			} else if (lineIsLd) {
				instruction = getLd(linecpy, labels, lc);
			} else if (lineIsLdr) {
				instruction = getLdr(linecpy);
			} else if (lineIsSt) {
				instruction = getSt(linecpy, labels, lc);
			} else if (lineIsStr) {
				instruction = getStr(linecpy);
			} else if (lineIsBr) {
				instruction = getBr(linecpy, labels, lc);
			} else if (lineIsTrap) {
				instruction = getTrap(linecpy);
			}
			
			printf("%04X\n", instruction);
		}
		else if (lineIsFill) {
			lc++;
			printf("0000\n");
		}
		else if (lineIsEnd) {
			return 0;
		}

		line[0] = 0;
	}
}


/*** Instructions logic ***/

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
		
		int num = strtoint(line);

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
		
		int num = strtoint(line);

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
	
	int num = hextoint(line);
	strcat(output, inttobin(num, 8)); // Append trap vector to output

	return bintoint(output);
}

int getNot(char line[]) {
	/*
		NOT R3, R4
		1001 DR(3) SR(3) 111111
	*/
	char* output = (char*) malloc(16 * sizeof(char));
	strcat(output, "1001");

	rc(line, 3); // Remove "NOT"
	
	char DR = line[1];
	strcat(output, chartobin(DR, 3)); // Append DR to output

	rc(line, 2); // Remove DR
	rc(line, 1); // Remove ","
	
	char SR = line[1];
	strcat(output, chartobin(SR, 3));

	strcat(output, "111111");

	return bintoint(output);
}

int getLd(char line[], int labels[], int lc) {
	/* 
		LD R1, L2
		0010 DR(3) PCoffset9(9)
	*/
	char* output = (char*) malloc(16 * sizeof(char));
	strcat(output, "0010");

	rc(line, 2); // Remove "LD"

	char DR = line[1];
	strcat(output, chartobin(DR, 3));

	rc(line, 2); // Remove DR
	rc(line, 1); // Remove ","

	int labelAddress = labels[chartoint(line[1])];
	int PCoffset = labelAddress - lc;
	
	strcat(output, inttobin(PCoffset, 9));

	return bintoint(output);
}

int getLdr(char line[]) {
	/* 
		LDR R1, R2, #-5
		0110 DR(3) BaseR(3) offset6(6)
	*/
	char* output = (char*) malloc(16 * sizeof(char));
	strcat(output, "0110");

	rc(line, 3); // Remove "LDR"

	char DR = line[1];
	strcat(output, chartobin(DR, 3));

	rc(line, 2); // Remove DR
	rc(line, 1); // Remove ","

	char BaseR = line[1];
	strcat(output, chartobin(BaseR, 3));

	rc(line, 2); // Remove BaseR
	rc(line, 1); // Remove ","
	rc(line, 1); // Remove "#"

	int offset = strtoint(line);
	strcat(output, inttobin(offset, 6));

	return bintoint(output);
}

int getSt(char line[], int labels[], int lc) {
	/* 
		ST R5, L2
		0011 SR(3) PCoffset9(9)
	*/
	char* output = (char*) malloc(16 * sizeof(char));
	strcat(output, "0011");

	rc(line, 2); // Remove "ST"

	char SR = line[1];
	strcat(output, chartobin(SR, 3));

	rc(line, 2); // Remove SR
	rc(line, 1); // Remove ","

	int labelAddress = labels[chartoint(line[1])];
	int PCoffset = labelAddress - lc;
	
	strcat(output, inttobin(PCoffset, 9));

	return bintoint(output);
}

int getStr(char line[]) {
	/* 
		STR R6, R1, #-5
		0111 SR(3) BaseR(3) offset6(6)
	*/
	char* output = (char*) malloc(16 * sizeof(char));
	strcat(output, "0111");

	rc(line, 3); // Remove "LDR"

	char SR = line[1];
	strcat(output, chartobin(SR, 3));

	rc(line, 2); // Remove SR
	rc(line, 1); // Remove ","

	char BaseR = line[1];
	strcat(output, chartobin(BaseR, 3));

	rc(line, 2); // Remove BaseR
	rc(line, 1); // Remove ","
	rc(line, 1); // Remove "#"

	int offset = strtoint(line);
	strcat(output, inttobin(offset, 6));

	return bintoint(output);
}

int getBr(char line[], int labels[], int lc) {
	/* 
		BRnzp L1
		0000 n z p PCoffset9(9)
	*/
	char* output = (char*) malloc(16 * sizeof(char));
	strcat(output, "0000");

	int nzp = 0;

	// Logic to determine NZP structure

	if (isspace(line[2])) {
		nzp = 0b111; // blank
		rc(line, 2); // Remove "BR"
	}
	else if (line[2] == 'n') {
		if (isspace(line[3])) {
			nzp = 0b100; // n
			rc(line, 3); // Remove "BRn"
		}
		else if (line[3] == 'z') {
			if (isspace(line[4])) {
				nzp = 0b110; // nz
				rc(line, 4); // Remove "BRnz"
			}
			else if (line[4] == 'p') {
				nzp = 0b111; // nzp
				rc(line, 5); // Remove "BRnzp"
			}
		}
		else if (line[3] == 'p' && isspace(line[4])) {
			nzp = 0b101; // np
			rc(line, 4); // Remove "BRnp"
		}
	}
	else if (line[2] == 'z') {
		if (isspace(line[3])) {
			nzp = 0b010; // z
			rc(line, 3); // Remove "BRz"
		}
		else if (line[3] == 'p' && isspace(line[4])) {
			nzp = 0b011; // zp
			rc(line, 4); // Remove "BRzp"
		}
	}
	else if (line[2] == 'p' && isspace(line[3])) {
		nzp = 0b001; // p
		rc(line, 3); // Remove "BRp"
	}

	strcat(output, inttobin(nzp, 3));

	int labelAddress = labels[chartoint(line[1])];
	int PCoffset = labelAddress - lc;
	
	strcat(output, inttobin(PCoffset, 9));

	return bintoint(output);
}


/*** Helper functions ***/

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

/*** Conversions ***/

// Convert a character '0' - '9' to integer
int chartoint(char num) {
	return (int) num - '0'; 
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
char* chartobin(char num, int length) {
	int intnum = chartoint(num);
	return inttobin(intnum, length);
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

// Convert string number to integer
int strtoint(char *str) {
	int num = 0;
	sscanf(str, "%d", &num); // Scan number into int
	return num;
}

// Convert string hex number to int
int hextoint(char *str) {
	int num = 0;
	sscanf(str, "%x", &num);
	return num;
}