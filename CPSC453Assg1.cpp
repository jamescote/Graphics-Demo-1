// CPSC453Assg1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

double f(double w)
{
	double w_halfed_squared = w / 2;
	w_halfed_squared *= w_halfed_squared;

	return sqrt(w_halfed_squared + w_halfed_squared);
}


int _tmain(int argc, _TCHAR* argv[])
{
	int iterations = 0;
	double w = 1000.0;

	while (w >= 1.0)
	{
		w = f(f(w));
		iterations++;
	}

	cout << "Number of steps before side length is less than 1.0: " << iterations << " @ " << w << ".\n";

	return 0;
}

