#include <stdio.h>
#include <string.h>

#define LIMIT 10000
#define LINE_SIZE 128

#define A12MASK 0XF000
#define A9MASK 0XE00
#define A6MASK 0X1C0
#define NMASK 0X800
#define ZMASK 0X400
#define PMASK 0X200

// For part 1
void assemble(char[]);
int findOrigin(FILE *);
int firstPass(FILE *, int[], int);
void printLabels(int[]);
void toUpperCase(char[]);
void removeSpace(char[]);

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
int secondPass(FILE *, int[], int);

void assemble(char filename[]) {
	// Open the file for reading
	FILE *infile = fopen(filename, "r");

	if (infile != NULL) {
		// Create labels array and set all elements to -1.
		int labels[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

		int lc = findOrigin(infile);

		if (lc > -1) {
			// Read in label values
			if (!firstPass(infile, labels, lc)) {
				// Show the labels.
				// printLabels(labels);

				// The following is for part 2
				secondPass(infile, labels, lc);
			}
		}

		// Close the file
		fclose(infile);
	} else {
		printf("Can't open input file.\n");
	}
}

int findOrigin(FILE *infile) {
	// Each trip through the while loop will read the next line of infile
	// into the line char array as a null terminated string.
	char line[LINE_SIZE];
	line[0] = 0;

	// The variable lineCount keeps track of how many lines have been read.
	// It is used to guard against infinite loops.  Don't remove anything
	// that has to do with linecount.
	int lineCount = 0;

	// For getting out of the while loop.
	int done = 0;

	// For getting rid of the trailing newline
	char c;

	char org[6] = ".ORIG";
	char don[LINE_SIZE];

	// Read lines until EOF reached or LIMIT lines read.  Limit prevent infinite
	// loop.
	while (!done && lineCount < LIMIT &&
		   fscanf(infile, "%[^\n]s", line) != EOF) {
		lineCount++;

		fscanf(infile, "%c", &c);  // Get rid of extra newline.

		// At this point, line contains the next line of the ASM file.
		// Put your code here for seeing if line is an origin.
		// Options:
		//	1. line is an origin (save value, set done = 1).
		//  2. line is a blank line (skip).
		//  3. line is a comment (skip).
		//  4. line is anything else (print error, set done = 1).
		removeSpace(line);
		toUpperCase(line);

		if (!(strncmp(line, org, 5))) {
			strcpy(don, line);
			done = 1;
		} else if (line[0] == 0) {
		} else if (line[0] == ';') {
		} else {
			printf(
				"ERROR 1: Missing origin directive. Origin must be first line "
				"in program.\n");
			done = 1;
		}

		// Set the line to empty string for the next pass.
		line[0] = 0;
	}

	// At this point you must decide if an origin was found or not.
	// How you do this is up to you.
	// If a good origin was found, check the size.
	//		if it is too big, print an error and return -1.
	//      if it is not too big, return the value.
	// If a good origin was NOT found, print the error message and return -1.

	int ret = 0;

	if (don[0] != 0) {
		if (don[5] == 'X') {
			sscanf(&don[6], "%x", &ret);
		} else {
			sscanf(&don[5], "%d", &ret);
		}
		if (ret > 0xffff) {
			printf(
				"ERROR 2: Bad origin address. Address too big for 16 bits.\n");
			ret = -1;
		}
	} else {
		printf(
			"ERROR 1: Missing origin directive. Origin must be first line in "
			"program.\n");
		ret = -1;
	}

	return ret;
}

int firstPass(FILE *infile, int labels[], int lc) {
	char line[LINE_SIZE];
	line[0] = 0;
	int num = 0;
	int doneQ = 0;
	int lineCount = 0;
	char c;

	char fill[6] = ".FILL";
	char end[5] = ".END";

	char add[4];
	strcpy(add, "ADD");

	char andJoel[4];
	strcpy(andJoel, "AND");

	char notJoel[4];
	strcpy(notJoel, "NOT");

	char ld[3];
	strcpy(ld, "LD");

	char ldr[4];
	strcpy(ldr, "LDR");

	char st[3];
	strcpy(st, "ST");

	char str[4];
	strcpy(str, "STR");

	char br[3];
	strcpy(br, "BR");

	char trap[5];
	strcpy(trap, "TRAP");

	char orig[6];
	strcpy(orig, ".ORIG");

	// Represents line[2]
	char *linePtr = line + 2;

	rewind(infile);

	while (!doneQ && lineCount < LIMIT &&
		   fscanf(infile, "%[^\n]s", line) != EOF) {
		removeSpace(line);
		toUpperCase(line);

		lineCount++;
		fscanf(infile, "%c", &c);  // Get rid of extra newline.

		if (line[0] == ';' || line[0] == 0 || !(strncmp(line, orig, 5)) ||
			line[0] == '\n') {
		} else if (!(strncmp(line, end, 4))) {
			return 0;
		} else if ((!(strncmp(line, add, 3))) ||
				   (!(strncmp(line, andJoel, 3))) ||
				   (!(strncmp(line, notJoel, 3))) ||
				   (!(strncmp(line, ld, 2))) || (!(strncmp(line, ldr, 3))) ||
				   (!(strncmp(line, st, 2))) || (!(strncmp(line, str, 3))) ||
				   (!(strncmp(line, br, 2))) || (!(strncmp(line, trap, 4)))) {
			lc++;
		} else if (line[0] == 'L') {
			if (line[2] == 0) {
				num = line[1] - 48;
				labels[num] = lc;
			} else if (!(strncmp(linePtr, fill, 5))) {
				num = line[1] - 48;
				labels[num] = lc;
				lc++;
			}
		}

		else {
			printf("ERROR 3: Unknown instruction.\n");
			return -1;
		}
		line[0] = 0;
	}
	printf("ERROR 4: Missing end directive.\n");
	return -1;
}

void toUpperCase(char line[]) {
	char *ptr = line;
	while (*ptr != 0) {
		if (*ptr < 123 && *ptr > 96) {
			*ptr = *ptr - 32;
		}
		ptr++;
	}
}

void removeSpace(char line[]) {
	char *ptr = line;
	char *here;
	char *there;
	while (*ptr != 0) {
		if (*ptr == 32 || *ptr == 9) {
			here = ptr;
			there = ptr + 1;
			while (*here != 0) {
				*here = *there;
				here++;
				there++;
			}
		}
		if (*ptr != 32 && *ptr != 9) {
			ptr++;
		}
	}
}

void printLabels(int labels[]) {
	printf("labels = {%d, %d, %d, %d, %d, %d, %d, %d, %d, %d}\n", labels[0],
		   labels[1], labels[2], labels[3], labels[4], labels[5], labels[6],
		   labels[7], labels[8], labels[9]);
}

int secondPass(FILE *infile, int labels[], int lc) {
	rewind(infile);
	printf("%X\n", lc);

	char line[LINE_SIZE];
	line[0] = 0;

	int done = 0;
	int lineCount = 0;
	char c;
	int value = 0;
	char *linePtr = line + 2;

	while (!done && lineCount < LIMIT &&
		   fscanf(infile, "%[^\n]s", line) != EOF) {
		removeSpace(line);
		toUpperCase(line);

		lineCount++;
		fscanf(infile, "%c", &c);  // Get rid of extra newline.

		if (line[0] == ';' || line[0] == 0 || !(strncmp(line, ".ORIG", 5)) ||
			line[0] == '\n') {
		} else if (!strncmp(line, "ADD", 3)) {
			lc++;
			value = getAdd(line);
			printf("%04X\n", value);
		} else if (!strncmp(line, "AND", 3)) {
			lc++;
			value = getAnd(line);
			printf("%04X\n", value);
		} else if (!strncmp(line, "NOT", 3)) {
			lc++;
			value = getNot(line);
			printf("%04X\n", value);
		} else if (!strncmp(line, "LDRR", 4)) {
			lc++;
			value = getLdr(line);
			printf("%04X\n", value);
		} else if (!strncmp(line, "LD", 2)) {
			lc++;
			value = getLd(line, labels, lc);
			printf("%04X\n", value);
		}

		else if (!strncmp(line, "STRR", 4)) {
			lc++;
			value = getStr(line);
			printf("%04X\n", value);
		} else if (!strncmp(line, "ST", 2)) {
			lc++;
			value = getSt(line, labels, lc);
			printf("%04X\n", value);
		}

		else if (!strncmp(line, "BR", 2)) {
			lc++;
			value = getBr(line, labels, lc);
			printf("%04X\n", value);
		} else if (!strncmp(line, "TRAP", 4)) {
			lc++;
			value = getTrap(line);
			printf("%04X\n", value);
		} else if (line[0] == 'L') {
			if (line[2] == 0) {
			} else if (!(strncmp(linePtr, ".FILL", 5))) {
				printf("0000\n");
			}
		} else {
			done = 1;
		}

		line[0] = 0;
		value = 0;
	}

	return 0;
}

int getAdd(char line[]) {
	int p1 = 0X1;
	int p2 = 0;
	int p3 = 0;
	int p4 = 0;
	int answer = 0;

	p2 = line[4] - 48;
	p3 = line[7] - 48;
	if (line[9] == 'R') {
		p4 = line[10] - 48;

		p1 = p1 << 12;
		p1 = p1 & A12MASK;
		answer = answer | p1;

		p2 = p2 << 9;
		p2 = p2 & A9MASK;
		answer = answer | p2;

		p3 = p3 << 6;
		p3 = p3 & A6MASK;
		answer = answer | p3;

		answer = answer | p4;
	} else {
		sscanf(&line[10], "%d", &p4);
		p4 = p4 & 0X1F;

		p1 = p1 << 12;
		p1 = p1 & A12MASK;
		answer = answer | p1;

		p2 = p2 << 9;
		p2 = p2 & A9MASK;
		answer = answer | p2;

		p3 = p3 << 6;
		p3 = p3 & A6MASK;
		answer = answer | p3;

		answer = answer | 32;
		answer = answer | p4;
	}

	return answer;
}
int getAnd(char line[]) {
	int p1 = 0X5;
	int p2 = 0;
	int p3 = 0;
	int p4 = 0;
	int answer = 0;

	p2 = line[4] - 48;
	p3 = line[7] - 48;

	if (line[9] == 'R') {
		p4 = line[10] - 48;

		p1 = p1 << 12;
		p1 = p1 & A12MASK;
		answer = answer | p1;

		p2 = p2 << 9;
		p2 = p2 & A9MASK;
		answer = answer | p2;

		p3 = p3 << 6;
		p3 = p3 & A6MASK;
		answer = answer | p3;

		answer = answer | p4;
	} else {
		sscanf(&line[10], "%d", &p4);
		p4 = p4 & 0X1F;

		p1 = p1 << 12;
		p1 = p1 & A12MASK;
		answer = answer | p1;

		p2 = p2 << 9;
		p2 = p2 & A9MASK;

		answer = answer | p2;

		p3 = p3 << 6;
		p3 = p3 & A6MASK;
		answer = answer | p3;

		answer = answer | 32;
		answer = answer | p4;
	}

	return answer;
}
int getNot(char line[]) {
	int p1 = 0X9;
	int p2 = 0;
	int p3 = 0;
	int answer = 0;

	p2 = line[4] - 48;
	p3 = line[7] - 48;

	p1 = p1 << 12;
	p1 = p1 & A12MASK;
	answer = answer | p1;

	p2 = p2 << 9;
	p2 = p2 & A9MASK;
	answer = answer | p2;

	p3 = p3 << 6;
	p3 = p3 & A6MASK;
	answer = answer | p3;
	answer = answer | 0X3F;

	return answer;
}
int getLd(char line[], int labels[], int lc) {
	int p1 = 0X2;
	int p2 = 0;
	int pcoffset = 0;
	int labelpos = 0;
	int answer = 0;

	p2 = line[3] - 48;

	labelpos = line[6] - 48;
	pcoffset = labels[labelpos] - lc;
	pcoffset = pcoffset & 0X1FF;

	p1 = p1 << 12;
	p1 = p1 & A12MASK;
	answer = answer | p1;

	p2 = p2 << 9;
	p2 = p2 & A9MASK;
	answer = answer | p2;

	answer = answer | pcoffset;

	return answer;
}
int getLdr(char line[]) {
	int p1 = 0X6;
	int p2 = 0;
	int p3 = 0;
	int p4 = 0;
	int answer = 0;

	p2 = line[4] - 48;
	p3 = line[7] - 48;

	sscanf(&line[10], "%d", &p4);
	p4 = p4 & 0X3F;

	p1 = p1 << 12;
	p1 = p1 & A12MASK;
	p2 = p2 << 9;
	p2 = p2 & A9MASK;
	p3 = p3 << 6;
	p3 = p3 & A6MASK;

	answer = answer | p1;
	answer = answer | p2;
	answer = answer | p3;
	answer = answer | p4;

	return answer;
}
int getSt(char line[], int labels[], int lc) {
	int p1 = 0X3;
	int p2 = 0;
	int pcoffset = 0;
	int labelpos = 0;
	int answer = 0;

	p2 = line[3] - 48;

	labelpos = line[6] - 48;
	pcoffset = labels[labelpos] - lc;
	pcoffset = pcoffset & 0X1FF;

	p1 = p1 << 12;
	p1 = p1 & A12MASK;
	answer = answer | p1;

	p2 = p2 << 9;
	p2 = p2 & A9MASK;

	answer = answer | p2;
	answer = answer | pcoffset;

	return answer;

	// return 0;
}
int getStr(char line[]) {
	int p1 = 0X7;
	int p2 = 0;
	int p3 = 0;
	int p4 = 0;
	int answer = 0;

	p2 = line[4] - 48;
	p3 = line[7] - 48;

	sscanf(&line[10], "%d", &p4);
	p4 = p4 & 0X3F;

	p1 = p1 << 12;
	p1 = p1 & A12MASK;
	p2 = p2 << 9;
	p2 = p2 & A9MASK;
	p3 = p3 << 6;
	p3 = p3 & A6MASK;

	answer = answer | p1;
	answer = answer | p2;
	answer = answer | p3;
	answer = answer | p4;

	return answer;
}
int getBr(char line[], int labels[], int lc) {
	int n = 0;
	int z = 0;
	int p = 0;
	int pcoffset = 0;
	int labelpos = 0;
	int answer = 0;

	if (line[2] == 'L') {
		n = 1;
		z = 1;
		p = 1;
		labelpos = line[3] - 48;
	} else {
		if (line[2] == 'N') {
			n = 1;
			if (line[3] == 'Z') {
				z = 1;
				if (line[4] == 'P') {
					p = 1;

					labelpos = line[6] - 48;
				} else {
					labelpos = line[5] - 48;
				}
			} else if (line[3] == 'P') {
				p = 1;

				labelpos = line[5] - 48;
			} else {
				labelpos = line[4] - 48;
			}
		} else if (line[2] == 'Z') {
			z = 1;
			if (line[3] == 'P') {
				p = 1;
				labelpos = line[5] - 48;
			} else {
				labelpos = line[4] - 48;
			}
		} else {
			p = 1;
			labelpos = line[4] - 48;
		}
	}

	pcoffset = labels[labelpos] - lc;
	pcoffset = pcoffset & 0X1FF;

	n = n << 11;
	n = n & NMASK;
	z = z << 10;
	z = z & ZMASK;
	p = p << 9;
	p = p & PMASK;

	answer = answer | n;
	answer = answer | z;
	answer = answer | p;
	answer = answer | pcoffset;

	return answer;
}
int getTrap(char line[]) {
	// Do Something
	int p1 = 0XF;
	int p2 = 0X0;
	int p3 = 0;
	int answer = 0;

	sscanf(&line[5], "%X", &p3);

	p1 = p1 << 12;
	p1 = p1 & A12MASK;
	p2 = p2 << 9;
	p2 = p2 & A9MASK;

	answer = answer | p1;
	answer = answer | p2;
	answer = answer | p3;

	return answer;
}
