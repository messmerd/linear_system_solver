#include "linear_system.h"

// Default constructor 
LinearSystem::LinearSystem()
{
	eq = nullptr;
	num_eqs = 0;
	coefs = nullptr;   // An array of maps that map each variable to its coefficient 
	values.clear();    // An array of maps that map each variable name to its value
	vars.clear();            // Stores all the names of all the variables used 
	valid_equations = false;

	m = nullptr;         // The N by N+1 matrix 
}

// Constructor for n-equation / n-variable problems. e is the string containing the equations (each is separated with a semicolon)  
LinearSystem::LinearSystem(std::string e)
{
	eq = nullptr;
	num_eqs = 0;
	coefs = nullptr;   // An array of maps that map each variable to its coefficient 
	values.clear();    // An array of maps that map each variable name to its value
	vars.clear();            // Stores all the names of all the variables used 
	valid_equations = false;

	m = nullptr;         // The N by N+1 matrix 

	parseEquations(e);
}

// Destructor 
LinearSystem::~LinearSystem()
{
	delete [] eq; 
	delete [] coefs; 
	if (m != nullptr)
	{
		for (int i = 0; i < num_eqs; i++)
		{
			delete [] m[i]; 
		}
		delete [] m; 
	}
}

// Solve linear system of equations using reduction
void LinearSystem::Solve()
{
	if (valid_equations)
	{
		reduceMatrix();

		int i = 0;
		for (char var : vars)
		{
			values[var] = m[i][num_eqs];
			i++;
		}
	}
}

// Solve linear system of equations using reduction
void LinearSystem::Solve(std::string equations)
{
	parseEquations(equations);
	Solve();
}

// Prints the equations 
void LinearSystem::displayEquations()
{
	if (valid_equations)
	{
		for (int i = 0; i < num_eqs; i++)
		{
			std::cout << eq[i] << std::endl; 
		}
	}
	else
	{
		std::cout << "Your equations are invalid.\n";
	}
}

// Prints the augmented matrix (the coefficients of each variable and the constant terms)
void LinearSystem::displayMatrix()
{
	if (valid_equations)
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
	else
	{
		std::cout << "Your equations are invalid or you still need to execute the Solve method.\n";
	}
}

// Prints the values of each variable 
void LinearSystem::displayResults()
{
	if (valid_equations)
	{
		for (auto var : vars)
		{
			std::cout << var << " = " << values[var] << "\n";
		}
	}
	else
	{
		std::cout << "Your equations are invalid or you still need to execute the Solve method.\n";
	}
}

// Parses the equations from a string of equations 
void LinearSystem::parseEquations(std::string e)
{
	num_eqs = 0; 
	for (int i = 0; i < e.length(); i++)
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

	if (num_eqs == 0)
	{
		std::cout << "You did not enter valid equations. \n";
		valid_equations = false;
		return; 
	}

	eq = new std::string[num_eqs];
	coefs = new std::map<char, double>[num_eqs];
	values.clear(); 

	for (int i=0; i<num_eqs; i++)
	{
		eq[i] = "";
		coefs[i].clear();  // ?? 
	}

	int eqon = 0;
	int start = 0; 
	for (int i=0; i<e.length(); i++)
	{	
		if (e[i]==';') 
		{
			parseEquation(e.substr(start, i - start), eqon);
			start = i + 1; 
			if (eqon == num_eqs - 1) {break; }
			eqon++;
		}
		else if (i == e.length() - 1)
		{
			parseEquation(e.substr(start, i - start + 1), eqon);
		}

		if (e[i] != ' ' && e[i] != ';')
		{
			eq[eqon] += e[i]; 
		}

	}

	if (vars.size() > num_eqs)
	{
		std::cout << "There are more variables then equations, so there is an infinite number of answers. \n"; 
		valid_equations = false;
		return; 
	}

	int x = 0; 
	m = new double*[vars.size()];  
	for (int y=0; y<vars.size(); y++)
	{
		m[y] = new double[vars.size() + 1];  // One extra because of the constants
		x = 0; 
		for (auto var : vars)
		{
			m[y][x] = coefs[y][var];
			x++; 
		}
		m[y][x] = coefs[y]['~'];
	}

	valid_equations = true; 
}

// Parses an equation 
void LinearSystem::parseEquation(std::string e, int eq_id)
{
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
					}
					else
					{
						coefs[eq_id][e[i2]] += ((e[i]=='-') ? -1.0 : 1.0)*on_left_side;  // The coefficient is an implicit 1. I.e. "+C" or "-B"
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

	// Move the constant to the right side of the equation: 
	coefs[eq_id]['~'] *= -1.0; 
}

// Reduces the augmented matrix 
int LinearSystem::reduceMatrix()  // n by n+1 matrix
{
	for (int i = 0; i < num_eqs; i++)  // y value (and also x)
	{
		/// Make current row be the one with the greatest absolute value in the current column
		int max_row = i; 
		for (int i2 = i; i2 < num_eqs; i2++) // see what row has greatest term at current column (i)
		{
			if (m[i2][i]*m[i2][i] > m[max_row][i]*m[max_row][i])
			{
				max_row = i2; 
			}
		}

		if (m[max_row][i] == 0)
		{
			// !!!
			continue; // ??
		}
		else
		{
			swapRows(i, max_row);
		}

		/// Set value at current row/column to 1 (leading one):    
		double mult_amount = 1.0/m[i][i]; 
		for(int col = i; col < num_eqs + 1; col++)  // Start at 0 instead, not i? 
		{
			m[i][col] *= mult_amount; 
		}

		/// Set all other rows to 0 using elementary row operations:
		leadingOneForm(i);
	}

	return 1; 
}

// Swaps two rows within the augmented matrix 
void LinearSystem::swapRows(int row1, int row2)
{
	double temp; 
	for (int i = 0; i < num_eqs + 1; i++)
	{
		temp = m[row1][i];
		m[row1][i] = m[row2][i]; 
		m[row2][i] = temp; 
	}
}

// Gets a row of the matrix into leading one form using elementary row operations
void LinearSystem::leadingOneForm(int leading_one_row)
{
	for (int i = 0; i < num_eqs; i++)
	{
		if (i == leading_one_row)
		{
			continue; 
		}
		
		double mult_amount = m[i][leading_one_row]; 
		for (int j = 0; j < num_eqs + 1; j++)
		{
			m[i][j] -= mult_amount*m[leading_one_row][j]; // ?
		}
	}

}

// Converts a string into a double
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

