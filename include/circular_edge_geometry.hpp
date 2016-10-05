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
	@brief Class for circular geometry of an edge
	@detail 
*/

#ifndef HH_CIRCULAR_EDGE_GEOMETRY_HH
#define HH_CIRCULAR_EDGE_GEOMETRY_HH

class
circular_edge_geometry : public edge_geometry
{
	private: 
	
	point center;
	double start_angle, end_angle, radius;
	
	public:
	
	circular_edge_geometry
	(point center_, double start_angle_, double end_angle_) :
	center(center_), start_angle(start_angle_), 
	end_angle(end_angle_), radius(radius_)
	{};
	
	point
	value(double parameter)
	{
		point c = center;
		double angle = parameter * (end_angle - start_angle) + start_angle;
		c.x += radius * std::cos(angle);
		c.y += radius * std::sin(angle);
		return c;
	}

}

#endif


