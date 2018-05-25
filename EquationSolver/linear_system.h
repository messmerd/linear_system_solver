#ifndef LINEARSYSTEM_H_
#define LINEARSYSTEM_H_

#include <iostream>
#include <string>
#include <map>
#include <set>

class LinearSystem
{
public:
	LinearSystem();                     // Default constructor 
	LinearSystem(std::string e);        // Constructor that takes in a string of linear equations (semicolon separated)
	~LinearSystem();                    // Destructor 

	void Solve();                       // Solve linear system of equations using reduction
	void Solve(std::string equations);  // Solve linear system of equations using reduction

	void displayEquations();            // Prints the equations 
	void displayMatrix();               // Prints the augmented matrix (the coefficients of each variable and the constant terms)
	void displayResults();              // Prints the values of each variable 

private:

	double** m;                     // The N by N+1 augmented matrix

	std::string* eq;
	int num_eqs;                    // The number of equations in the string given by the user
	std::map<char, double>* coefs;  // An array of maps that map each variable to its coefficient. Each array index represents a different equation.  
	std::map<char, double> values;  // A map that maps each variable name to its value. This is set once the Solve method is executed 
	std::set<char> vars;            // Stores the names of all the variables used 
	bool valid_equations;            // Whether or not the equations given by the user can be used. Must be properly formatted and # of variables must equal the # of equations

	void parseEquations(std::string e);              // Parses the equations from a string of equations 
	void parseEquation(std::string e, int eq_id);    // Parses an equation 
	
	int reduceMatrix();             // Reduces the augmented matrix 

	void swapRows(int row1, int row2);               // Swaps two rows within the augmented matrix 
	void leadingOneForm(int leading_one_row);        // Gets a row of the matrix into leading one form using elementary row operations

};

double strtodouble(std::string s);  // Converts a string into a double

#endif
