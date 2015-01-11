#ifndef RULES_H
#define RULES_H

#include <omp.h>
#include <stdio.h>  // stdio functions are used since C++ streams aren't necessarily thread safe
#include "Point.h"

//#include "cuda.h"

using namespace std;

int correctResult(int *data, const int n, const int b);
void cudatest();
unsigned short index(unsigned char dimensions, unsigned char x,
		unsigned char y);
bool willBeTaken(unsigned char dimensions, Point** points,
		const unsigned char x, const unsigned char y, const bool black);

#endif // RULES_H
