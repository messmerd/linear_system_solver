#ifndef EQSOLVE_H_
#define EQSOLVE_H_


#include <string>
#include <map>
#include <set>

class EqSolve
{
public:
	EqSolve();
	EqSolve(std::string e);
	std::string solveEquation(std::string equation);
	void displayMatrix();

	void Solve();  // Solve using reduction

private:
	std::string* eq;
	//int vars;
	int num_eqs;
	std::map<char, double>* coefs;  // An array of maps that map each variable to its coefficient 
	std::set<char> vars;            // Stores all the names of all the variables used 

	double** m;         // The matrix
	double* mresults;   // Stores final results
	std::string mvar;   // Stores variable names 
	double* b;          // Right side of the equations 

	void parseEquation(std::string e, int eq_id); 

	double** reflect(double**& matrix);
	double** coefficientMatrix(double**& matrix);
	double** matrixMult(double**& matrix);
	double** createMatrix(std::string equation);
	double getDeterminate(const double**& matrix);

};

double strtodouble(std::string s);

#endif