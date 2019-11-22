#include "file.h"

int fCountLines(FILE **finput) { // Count lines of a file and put pointer back to start
	int counter = 0;

	int ch = getc(*finput); // Read first character of file
	if (ch == EOF) { // If file is empty

		return counter;
	}

	while (ch != EOF) { // Read file character by character
		if (ch == '\n') { // If newline was found

			counter++;
		}

		ch = getc(*finput);
	}

	if (counter == 0) { // If no newlines found

		counter = 1; // Then there is one line with no newline
	}

	return counter;
}