#include "eqsolve.h"
#include <iostream>
#include <map>
#include <set>

EqSolve::EqSolve()
{
	vars.clear();
}

EqSolve::EqSolve(std::string e)
{
	// CONSTRUCTOR FOR N-EQUATION / N-VARIABLE PROBLEMS  
	//vars = 0;
	
	vars.clear(); 
	num_eqs = 0; 

	for (int i=0; i<e.length(); i++)
	{
		if (e[i]=='=')  // The number of equations 
		{
			num_eqs++;
		}

		if (isalpha(e[i]))
		{
			vars.insert(e[i]);
		}
	}

	b = new double[num_eqs];
	mresults = new double[num_eqs];
	eq = new std::string[num_eqs];
	coefs = new std::map<char, double>[num_eqs];

	for (int i=0; i<num_eqs; i++)
	{
		b[i] = 0.0;
		mresults[i] = 0.0;
		eq[i] = "";
		coefs[i].clear(); // ?? 
	}


	int eqon = 0;
	int start = 0; 
	for (int i=0; i<e.length(); i++)
	{	
		if (e[i]==';') 
		{
			if (eqon >= num_eqs) {break; }
			parseEquation(e.substr(start, i - start), eqon);
			start = i + 1; 
			eqon++;
		}

	}

	// For testing: Output the variables used: 
	
	/*for (auto it : vars)
	{
		std::cout << it << "\n";
	}*/

	// CREATING MATRIX

	//vars = mvar.length();
	if (vars.size() > num_eqs)
	{
		std::cout << "There are more variables then equations, so this is unsolvable. \n";
	}

	mresults = new double[vars.size()];
	for (int i=0; i<vars.size(); i++) 
	{
		mresults[i] = 0;
	}



	//std::cout << "ok. vars.size() = " << vars.size() << "\n\n";

	int x = 0; 
	m = new double*[vars.size()];  
	for (int y=0; y<vars.size(); y++)
	{
		m[y] = new double[vars.size() + 1];  // One extra because of the constants
		x = 0; 
		for (auto var : vars)
		{
			m[y][x] = coefs[y][var];
			//std::cout << "y="<<y<<"; x="<<x<<"\n";
			x++; 
		}
		m[y][x] = coefs[y]['~'];
		
	}
	
	//std::cout << "Created matrix. \nEqs = " << num_eqs << "\n";



}


void EqSolve::parseEquation(std::string e, int eq_id)
{

	//std::cout << "Parsing. e = '" << e << "' \neq_id = " << eq_id << "\n\n";

	int i, i2 = 0;
	int on_left_side = 1; 

	for (auto it : vars)
	{
		coefs[eq_id][it] = 0.0;  // Initialize all coefficients to 0 
	}
	coefs[eq_id]['~'] = 0.0;     // Initialize the constant to 0 
	
	for (i = 0; i < e.length(); i++)
	{
		if (isdigit(e[i]) || e[i]=='+' || e[i]=='-') // Start of a coefficient 
		{
			i2 = i+1;
			bool coef_exists = (isdigit(e[i])) ? true : false; 
			bool var_exists = false; 
			while (i2 < e.length() && e[i2] != '=' && e[i2] != '+' && e[i2] != '-') // End when at end, when at end of left side of equation, or if it was a constant and it is past it
			{
				if (isdigit(e[i2]))
				{
					coef_exists = true; 
				}
				if (isalpha(e[i2]))
				{
					if (coef_exists) 
					{
						coefs[eq_id][e[i2]] += strtodouble(e.substr(i, i2-i))*on_left_side; 
						//std::cout << "Coef exists: '" << e.substr(i, i2-i) << "'\n";
					}
					else
					{
						coefs[eq_id][e[i2]] += ((e[i]=='-') ? -1.0 : 1.0)*on_left_side;  // The coefficient is an implicit 1. I.e. "+C" or "-B"
						//std::cout << "Implicit Coef exists: '" << ((e[i]=='-') ? -1.0 : 1.0) << "'\n";
					}
					var_exists = true; 
					i = i2; // ??
				}
				i2++;
			}

			if (coef_exists && !var_exists)  // Found a constant 
			{
				coefs[eq_id]['~'] += strtodouble(e.substr(i, i2-i))*on_left_side;  // ~ means constant 
				i = i2; // ??
				//std::cout << "Const. exists: '" << e.substr(i, i2-i) << "'\n";
			}
		}
		else if (isalpha(e[i])) // The first term might start with the variable (implicit 1 and no + or -)
		{
			coefs[eq_id][e[i]] += 1.0*on_left_side; 
		}

		if (e[i] == '=')
		{
			on_left_side = -1; // Will move anything it finds to the left side of the equation so the form is: aX + ... + cZ + n = 0  
		}

	}

}


	
double** EqSolve::reflect(double**& matrix)
{
	return matrix;

}



double** EqSolve::coefficientMatrix(double**& matrix)
{
	return matrix;
}



double** EqSolve::matrixMult(double**& matrix)
{
	return matrix;
}



double** EqSolve::createMatrix(std::string equation)
{
	return m;
}



double EqSolve::getDeterminate(const double**& matrix)
{
	return 0.0;
}


void EqSolve::displayMatrix()
{
	for (int y=0; y<vars.size(); y++)
	{
		for(int x=0; x<vars.size() + 1; x++)  // One extra because of the constants
		{
			std::cout << m[y][x];
			std::cout << "\t";
		}
	std::cout << "\n";
	}
}




double strtodouble(std::string s)
{
	double result = 0.0;
	std::string fixed = "";
	for(int i=0; i<s.length(); i++)
	{
		if (s[i]=='+' || s[i]=='-')
		{
			fixed = s[i] + fixed;  
		}
		else if (s[i] != ' ')
		{
			fixed += s[i]; 
		}
	}
	if (fixed.length() != 0 && fixed[0] != '+' && fixed[0] != '-')
	{
		fixed = '+' + fixed; 
	}

	//std::cout << "Fixed: '" << fixed << "'\n";

	int decpos = fixed.find('.', 0);
	if (decpos == std::string::npos)
		decpos = fixed.length();

	for(int i=1; i<fixed.length(); i++)
	{
		if (i<decpos)
		{
			result += pow(10,decpos-i-1)*((char)fixed[i]-48); 
		}
		if (i>decpos)
		{
			result += pow(10,decpos-i)*((char)fixed[i]-48); 
		}
	}
	if (fixed[0]=='-')
	{
		result *= -1;
	}
	return result; 
}

