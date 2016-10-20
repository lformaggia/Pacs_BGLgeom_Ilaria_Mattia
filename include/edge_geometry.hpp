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
namespace BGLgeom{

template<unsigned int dim>
class
edge_geometry{
	public:
	virtual BGLgeom::point<dim>
	value (const double parameter) = 0;
	
	virtual std::vector<double>
	first_derivatives (const double x) = 0;
	
	virtual std::vector<double>
	second_derivatives (const double x) = 0;
	
	/*virtual double
	curvilinear_abscissa (const double parameter) = 0;*/
	
	
}; //class

} //namespace

#endif
