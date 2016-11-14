/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file test_muparser.hpp
	@author Ilaria Speranza and Mattia Tantardini
	@date Sept, 2016
	@brief Testing MuParser_interface class
*/

#include <iostream>
#include <string>
#include "my_muparser_interface.hpp"

using namespace My_muparser;
using namespace std;

int main(){
	string expr = "2*s+1";
	MuParser_interface M1(expr);
	
	std::cout << M1 << endl;
	
	MuParser_interface M2;
	std::cout << "Parser 2: " << M2 << endl;
	M2.set_expr(expr);
	std::cout << "Parser 2: " << M2 << endl;
	
	std::cout << "Evaluation at 3: " << M2(3) << endl;
	std::cout << "Evaluation at 1.5: " << M2(1.5) << endl;
	
	//Copy constructor
	MuParser_interface M3(M2);
	cout << "M3: " << M3 << endl;
	std::cout << "Evaluation at 3: " << M3(3) << endl;
	
	//assignment operator
	MuParser_interface M4;
	M4 = M3;
	cout << "M4: " << M4 << endl;
	std::cout << "Evaluation at 3: " << M4(3) << endl;
	
	//Istream
	std::string typed_expr;
	double typed_value;
	cout << "Type an expression to be evaluated (depending on the variable s): ";	
	cin >> typed_expr;
	MuParser_interface M5(typed_expr);	
	cout << "Type a value in which evaluate the previous expression: ";
	cin >> typed_value;
	cout << "Evaluation: " << M5(typed_value) << endl;
	
	return 0;
}
