#include <stdlib.h>
#include <math.h>

#include "primenumber.h"

int isPrime(int n) { // Return 1 if given number is prime, 0 if it is not
	// If n is prime, then a factor of n must be smaller than square root of n
	if (n < 2) {

		return 0;
	}

	else {
		for (int i = 2; i <= sqrt(n); i++) { 
			if ((n % i) == 0) {

				return 0;
			}
		}
	}

	return 1;
}

int nextPrime(int n) { // Return next prime number closest to given number
	int foundFlag = 0; // Flag that indicates when the prime number is found

	if (n < 2) {
		foundFlag = 1;

		return 2; // First prime number is 2
	}

	else {
		int i = n + 1;
		while (foundFlag == 0) { // While we have not found prime number
			if (isPrime(i) == 1) {

				foundFlag = 1; // Found prime number
			}

			else {

				i++; // Check next number
			}
		}
		
		return i;
	}
}