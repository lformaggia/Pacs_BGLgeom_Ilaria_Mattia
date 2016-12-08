/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file intersections2D_utilities.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief More classes and function to handle intersections
	
	@detail In this header can ben found some more specific utilities to handle
			intersections between edges. In particular, we provide a function
			which computes the "situation", "type" of the intersection, and a struct
			that can be useful when one need to refine the edges of the graph
			according to the intersections he found
*/

#ifndef HH_INTERSECTIONS2D_UTILITIES_HH
#define HH_INTERSECTIONS2D_UTILITIES_HH

#include "data_structure.hpp"
#include "linear_edge.hpp"
#include "intersections2D.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <iostream>

namespace BGLgeom{
/*!
	@brief An enum class to identify the intersection situation
	@detail We need to identify how the edges are located in the plane and
			how they intersect. In this way we can perform the correct actions
			at graph level to intersect topologically the edges. \n
			In some cases, or within some algorithms, we already have one edge
			and we want to know if this one intersects with one new edge that is
			coming into the graph. We also use "new" and "old" edge w.r.t the order
			in which the edges are passed into the function compute_intersection:
			"old" is the already present one, "new" the incoming edge.
	
	@param X Simple cross intersection. We are happy
	@param T_new The new edge intersects in one of its extremes the old edge
	@param T_old The old edge intersects in one of its extremes the new edge
	@param Overlap_outside The new edge is overlapped to the old one and it is larger
	@param Overlap_inside The new edge is overlapped to the old one and it is smaller
	@param Overlap The new edge overlaps in some part (not all) the old edge
	@param Overlap_extreme The edges are overlapped and one extreme coincides.
						   No way to distinguish if new edge is the larger or the smaller
	@param Identical The two edges are the same
	@param Common_extreme The two edges have only one extreme in common
	@param Something_went_wrong Error in computing the intersection
	@param No_intersection There are no intersection between the edges
*/
enum class intersection_type_new	{X, T_new, T_old, Overlap_outside, Overlap_inside, Overlap,
								Overlap_extreme, Identical, Common_extreme, Something_went_wrong,
								No_intersection};

/*!
	@brief Function to compute the type of intersection
	@detail It returns one of the situation contained in the enum intersection_type_new
*/
intersection_type_new
compute_intersection_type(BGLgeom::Intersection const& I);

template <typename Graph>
struct Int_layer{
	// The alias for the graph. Dimension = 2
	/*
	using Graph = boost::adjacency_list	<boost::vecS, boost::vecS, boost::directedS,
										 BGLgeom::Vertec_base_property<2>,
										 BGlgeom::Edge_base_property_static<BGlgeom::linear_edge<2>,2>
										 >;
	*/
	//! The type of the intersection
	intersection_type_new how;
	//! The edge descriptor fo the old edge that intersects the new one
	BGLgeom::Edge_desc<Graph> int_edge;
	//! The container for the intersection points
	std::vector<BGLgeom::point<2>> int_pts;
	//! Bool value to monitor if the components of int_pts will be swapped
	bool swapped_comp;

	unsigned int intersected_extreme; // it's useful only in those cases where the intersection happens in one of the two extremes. =0 if 1st extreme, =1 if second extreme
	
	
	//! Constructor
	Int_layer	(BGLgeom::Intersection const& _I,
						 BGLgeom::Edge_desc<Graph> const& _e = BGLgeom::Edge_desc<Graph>()) : int_edge(_e),
																							  int_pts(),
																							  swapped_comp(false){
		int_pts.resize(_I.numberOfIntersections);
		for(std::size_t i = 0; i < _I.numberOfIntersections; ++i)
			int_pts[i] = _I.intersectionPoint[i];
		how = compute_intersection_type(_I);
		
		if(_I.endPointIsIntersection[1][0] == 1) 
			intersected_extreme = 0;
		else
			intersected_extreme = 1;
	}
	
};	//Int_layer

template <typename Graph>
std::ostream &
operator<< (std::ostream & out, Int_layer<Graph> const& I){
//	out << "Number of intersections: " << I.num_intersections << std::endl;
//	out << "Intersecting edge: " << I.int_edge << std::endl;
	out << "Intersection points: " << std::endl;
	for(const BGLgeom::point<2> & P: I.int_pts)
		out << "\t" << P << std::endl;
	out << "Type: ";
	if(I.how == BGLgeom::intersection_type_new::X)
		out << "X";
	else if(I.how == BGLgeom::intersection_type_new::T_new)
		out << "T_new";
	else if(I.how == BGLgeom::intersection_type_new::T_old)
		out << "T_old";
	else if(I.how == BGLgeom::intersection_type_new::Overlap_outside)
		out << "Overlap_outside";
	else if(I.how == BGLgeom::intersection_type_new::Overlap_inside)
		out << "Overlap_inside";
	else if(I.how == BGLgeom::intersection_type_new::Overlap)
		out << "Overlap";
	else if(I.how == BGLgeom::intersection_type_new::Overlap_extreme)
		out << "Overlap_extreme";
	else if(I.how == BGLgeom::intersection_type_new::Identical)
		out << "Identical";
	else if(I.how == BGLgeom::intersection_type_new::Common_extreme)
		out << "Common_extreme";
	else if(I.how == BGLgeom::intersection_type_new::Something_went_wrong)
		out << "Something went wrong";	
	out << std::endl;
	return out;
}	//operator<<

}	//BGLgeom

#endif	//HH_INTERSECTIONS2D_UTILITIES_HH
