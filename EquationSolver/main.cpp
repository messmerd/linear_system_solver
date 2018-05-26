#include "linear_system.h"
#include <iostream>

int main(int argc, char *argv[])
{
	std::cout.precision(10);
	std::cout.showpoint;

	LinearSystem *equations;

	if (argc > 1)  // If a command-line argument was given
	{
		equations = new LinearSystem(argv[1]);  // Use the command-line argument as the equations 
	}
	else
	{
		//LinearSystem equations1("-A +2B    +3C = 4 ; 5A + 6.68B-7C=10;-11A+0.5B-3.79C=-0.234;");
		equations = new LinearSystem("3x+2y-3z=-13 ; -3y+4x+6z=7; -z+x=-5    ");  // Use a default test case 
	}

	std::cout << "The equations:\n\n";
	equations->displayEquations();
	std::cout << "\nThe augmented matrix:\n\n";
	equations->displayMatrix(); 
	equations->Solve();
	std::cout << "\nThe augmented matrix after reduction: \n\n"; 
	equations->displayMatrix();
	std::cout << "\nThe results:\n\n"; 
	equations->displayResults(); 
	std::cout << std::endl; 
	delete equations; 

	return 0;
}