/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file my_muparser_interface.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Class to create an interface to muparser and make it a callable object
	
	@detail It uses as variable for the parametrization the letter "s".
*/

#ifndef HH_MY_MUPARSER_INTERFACE_HH
#define HH_MY_MUPARSER_INTERFACE_HH

#include <iostream>
#include <string>
#include "muParser.h"

namespace My_muparser{

std::string const default_expr = "0*s";

//template <unsigned int dim>
class MuParser_interface{
	public:
		//! Default constructor
		MuParser_interface() : value(0), expr(default_expr), expr_is_set(false){
			M.DefineVar("s", &value);
			M.SetExpr(expr);
		}
		
		//! Constructor
		MuParser_interface(std::string const& _expr) : value(0), expr(_expr), expr_is_set(true){
			M.DefineVar("s", &value);
			M.SetExpr(expr);
		}
		
		//! Copy constructor
		MuParser_interface(MuParser_interface const& MPi) : value(MPi.value), expr(MPi.expr), M(){
			M.DefineVar("s", &value);
			M.SetExpr(expr);
			expr == default_expr ? expr_is_set = false : expr_is_set = true;
		}
		
		//! Move constructor
		MuParser_interface(MuParser_interface && MPi) : value(MPi.value), expr(MPi.expr), M(){
			M.DefineVar("s", &value);
			M.SetExpr(expr);
			expr == default_expr ? expr_is_set = false : expr_is_set = true;
		}
		
		//! Assignment operator
		MuParser_interface & operator=(MuParser_interface const& MPi){
			if(this != &MPi){
				M.ClearVar();
				expr = MPi.expr;
				value = MPi.value;
				M.DefineVar("s", &value);
				M.SetExpr(expr);
				expr == default_expr ? expr_is_set = false : expr_is_set = true;
			}
			return *this;
		}
		
		//! Move assignment
		MuParser_interface & operator=(MuParser_interface && MPi){
			if(this != &MPi){
				M.ClearVar();
				expr = MPi.expr;
				value = MPi.value;
				M.DefineVar("s", &value);
				M.SetExpr(expr);
				expr == default_expr ? expr_is_set = false : expr_is_set = true;
			}
			return *this;
		}
		
		//! Destructor
		~MuParser_interface(){
			M.ClearVar();
		}
		
		//! It allows to set the expression
		void set_expr(std::string const& _expr){
			expr = _expr;
			M.SetExpr(expr);
			expr_is_set = true;
		}
		
		/*!
			@brief Overload of the calling operator
			@detail It allows to evaluate the expression set in the parser in the
					value given as parameter. This overload alse make the class
					MuParser_interface a callable object, that can be used inside
					the geometric property of the edge as std::functional
		*/
		double operator()(double const& _value){
			if(!expr_is_set){
				std::cerr << "Attempt to evaluating a meaningless expression" << std::endl;
				return .0;	//or exit()?
			}
			value = _value;
			double result;
			try{
				result = M.Eval();
			} catch(mu::Parser::exception_type &e){
				std::cerr << e.GetMsg() << std::endl;
			}
			return result;
		}
		
		
		//! Overload of output operator. It shows infos on the state of the class
		friend std::ostream & operator<<(std::ostream & out, MuParser_interface const& MPi){
			if(MPi.expr_is_set)
				out << "Expression to be evaluated: " << MPi.expr;
			else
				out << "Warning: any valid expression set";
		}
		
		//! Overload of input operator
		friend std::istream & operator>>(std::istream & in, MuParser_interface & MPi){
			in >> MPi.expr;		//it reads only a string!
			MPi.M.SetExpr(MPi.expr);
			MPi.expr_is_set = true;
		}
		
		
	private:
		//! The value of the variable in which the expression will be evaluated
		double value;
		//! The string containing the expression to be evaluated (needed to implement copy constructor and similar)
		std::string expr;
		//! The parser
		mu::Parser M;
		//! A flag to be sure the expression was set properly
		bool expr_is_set;
};	//MuParser_interface

}	//My_muparser

#endif	//HH_MY_MUPARSER_INTERFACE_HH
