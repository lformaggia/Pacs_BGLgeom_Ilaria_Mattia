/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file edge_geometry.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Virtual base class for the geometry of an edge
	@detail 
*/

#ifndef HH_EDGE_GEOMETRY_HH
#define HH_EDGE_GEOMETRY_HH

class
edge_geometry{
	public:
	virtual point
	value (double parameter) = 0;
	
	virtual std::vector<point>
	first_derivatives (double parameter) = 0;
	
	virtual std::vector<point>
	second_derivatives (double parameter) = 0;
	
	virtual double
	curvilinear_abscissa (double parameter) = 0;
	
	
}

#endif
