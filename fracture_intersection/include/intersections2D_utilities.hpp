/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*!
	@file	intersections2D_utilities.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	More classes and function to handle intersections
	
	In this header some more specific utilities to handle
	intersections between edges can be found. In particular, we provide a function
	which computes the "type" of the intersection and a struct
	that can be useful when one needs to refine the edges of the graph
	according to the intersections he found
*/

#ifndef HH_INTERSECTIONS2D_UTILITIES_HH
#define HH_INTERSECTIONS2D_UTILITIES_HH

#include "graph_access.hpp"
#include "intersections2D.hpp"

#include <vector>
#include <iostream>

namespace Fracture{

/*!
	@brief	Class to store specific information about the intersection
	
	This class contains all information needed to build in the proper 
	way the graph by successive addition of edges. It stores specific 
	information that will be useful in the algorithm.
*/
template <typename Graph>
struct Int_layer{
	//! The type of the intersection
	BGLgeom::intersection_type how;
	//! The edge descriptor for the old edge that intersects the new one
	BGLgeom::Edge_desc<Graph> int_edge;
	//! The container for the intersection points
	std::vector<BGLgeom::point<2>> int_pts;
	//! Bool value to monitor if the components of int_pts will be swapped
	bool swapped_comp;
	// It's useful only in those cases where the intersection happens in one of the two extremes of the old edge. =0 if 1st extreme, =1 if second extreme
	unsigned int intersected_extreme_old; 
	// It's useful only in those cases where the intersection happens in one of the two extremes of the new edge. =0 if 1st extreme, =1 if second extreme
	unsigned int intersected_extreme_new; 
		
	//! Constructor
	Int_layer	(BGLgeom::Intersection const& _I,
				 BGLgeom::Edge_desc<Graph> const& _e = BGLgeom::Edge_desc<Graph>()) : how(_I.how), 
				 																	  int_edge(_e),
																					  swapped_comp(false) {
		int_pts.resize(_I.numberOfIntersections);
		for(std::size_t i = 0; i < _I.numberOfIntersections; ++i)
			int_pts[i] = _I.intersectionPoint[i];
		
		if(_I.endPointIsIntersection[0][0] == 1) 
			intersected_extreme_old = 0;
		else
			intersected_extreme_old = 1;
			
		if(_I.endPointIsIntersection[1][0] == 1) 
			intersected_extreme_new = 0;
		else
			intersected_extreme_new = 1;
	}
	
	//! Destructor
	virtual ~Int_layer() = default;
};	//Int_layer

//! Overload of output operator to show infos contained in Int_layer
template <typename Graph>
std::ostream &
operator<< (std::ostream & out, Int_layer<Graph> const& I){
	out << "Intersection points: " << std::endl;
	for(const BGLgeom::point<2> & P: I.int_pts)
		out << "\t" << P << std::endl;
	out << "Type: ";
	if(I.how == BGLgeom::intersection_type::X)
		out << "X";
	else if(I.how == BGLgeom::intersection_type::T_new)
		out << "T_new";
	else if(I.how == BGLgeom::intersection_type::T_old)
		out << "T_old";
	else if(I.how == BGLgeom::intersection_type::Overlap_outside)
		out << "Overlap_outside";
	else if(I.how == BGLgeom::intersection_type::Overlap_inside)
		out << "Overlap_inside";
	else if(I.how == BGLgeom::intersection_type::Overlap)
		out << "Overlap";
	else if(I.how == BGLgeom::intersection_type::Overlap_extreme_inside)
		out << "Overlap_extreme_inside";
	else if(I.how == BGLgeom::intersection_type::Overlap_extreme_outside)
		out << "Overlap_extreme_outside";
	else if(I.how == BGLgeom::intersection_type::Identical)
		out << "Identical";
	else if(I.how == BGLgeom::intersection_type::Common_extreme)
		out << "Common_extreme";
	else if(I.how == BGLgeom::intersection_type::Something_went_wrong)
		out << "Something went wrong";	
	out << std::endl;
	return out;
}	//operator<<

}	//Fracture

#endif	//HH_INTERSECTIONS2D_UTILITIES_HH
