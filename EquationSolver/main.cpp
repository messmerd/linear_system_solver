#include "eqsolve.h"
#include <iostream>

int main()
{
	//std::cout.precision(10);
	//std::cout.showpoint;
	//std::cout << (strtodouble("-12.10101")) << std::endl;

	EqSolve solve1("-A +2B    +3C = 4 ; 5A + 6.68B-7C=10;-11A+0.5B-3.79C=-0.234;");

	

	solve1.displayMatrix();

	std::cout << "\n\n";

	return 0;
}