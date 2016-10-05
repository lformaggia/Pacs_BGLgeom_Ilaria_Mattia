/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file generic_edge_geometry.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief class for the generic geometry of an edge
	@detail 
*/

#ifndef HH_GENERIC_EDGE_GEOMETRY_HH
#define HH_GENERIC_EDGE_GEOMETRY_HH


class
generic_edge_geometry : public edge_geometry
{
	private:
	
	std::function<point(double)> value_fun;
	std::function<std::vector<point>(double)> first_derivatives_fun;
	std::function<std::vector<point>(double)> second_derivatives_fun;
	std::function<double(double)> curvilinear_abscissa_fun;
	
	public:
	
	generic_edge_geometry
	(std::function<point(double)> value_,
	 std::function<std::vector<point>(double)> first_derivtives_,
	 std::function<std::vector<point>(double)> second_derivatives_,
	 std::function<double(double)> curvilinear_abscissa_) :
	value_fun(value_),
	first_derivatives_fun(first_derivatives_),
	second_derivatives_fun(second_derivatives_),
	curvilinear_abscissa_fun(curvilinear_abscissa_)
	{};
	
	point
	value (double parameter)
	{return value_fun(parameter);};
	
}

#endif
