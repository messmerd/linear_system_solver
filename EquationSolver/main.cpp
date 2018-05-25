#include "linear_system.h"
#include <iostream>

int main(int argc, char *argv[])
{
	std::cout.precision(10);
	std::cout.showpoint;

	if (argc > 1)
	{
		LinearSystem *equations = new LinearSystem(argv[1]);
		equations->displayEquations();
		equations->displayMatrix(); 
		equations->Solve();
		equations->displayResults(); 
		equations->displayMatrix();
		std::cout << std::endl; 
		delete equations; 
	}
	else
	{
		//LinearSystem equations1("-A +2B    +3C = 4 ; 5A + 6.68B-7C=10;-11A+0.5B-3.79C=-0.234;");
		LinearSystem *equations1 = new LinearSystem("3x+2y-3z=-13 ; -3y+4x+6z=7; -z+x=-5    ");
		equations1->Solve();
		//equations1.displayEquations();
		//equations1.displayMatrix(); 
		equations1->displayResults(); 
		std::cout << std::endl;
		delete equations1; 
	}

	return 0;
}