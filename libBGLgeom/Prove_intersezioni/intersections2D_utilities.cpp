#include "intersections2D_utilities.hpp"
#include "intersections2D.hpp"
#include <iostream>

using namespace BGLgeom;

namespace BGLgeom{

intersection_type_new
compute_intersection_type(BGLgeom::Intersection const& I){
	// Identical:
	if(I.identical)
		return intersection_type_new::Identical;
	
	// Something_went_wrong
	if(!I.good)
		return intersection_type_new::Something_went_wrong;
		
	// No_intersection
	if(!I.intersect)
		return intersection_type_new::No_intersection;
	
	// Now more complicated cases
	// computing the number of endPointIntersection
	unsigned int numEndPointIntersections = 0u;
	for(std::size_t i=0; i<2; ++i){
		for(std::size_t j=0; j<2; ++j){
			if(I.endPointIsIntersection[i][j])
				numEndPointIntersections++;
		}
	}
	std::cout << I.numberOfIntersections << std::endl;
	//numIntersection == 0 already treated
	if(I.numberOfIntersections == 1){	//collinear == false, i.e. only 1 intersection, but the case of collinear with common extreme
		//with only one intersection, of course edge are not collinear
		// X intersection
		if(numEndPointIntersections == 0){
			return intersection_type_new::X;
		}
		//T_new, T_old
		if(numEndPointIntersections == 1){
			//edge 1 interseca con uno dei suoi due estremi (0 o 1)
			if(I.endPointIsIntersection[1][0] || I.endPointIsIntersection[1][1]){
				return intersection_type_new::T_new;
			}
			//edge 0 interseca con uno dei suoi due estremi (0 o 1)
			if(I.endPointIsIntersection[0][0] || I.endPointIsIntersection[0][1]){
				return intersection_type_new::T_old;
			}
		}
		//Common_extreme; There are two end points (one for each edge) that meets
		if(numEndPointIntersections == 2){
			return intersection_type_new::Common_extreme;
		}
	} else {		// 2 intersections, i.e. collinear == true
		//Overlap_Outside; edge 0 (old) intersects in both its extremes (0 and 1) edge 1 (new)		
		if(numEndPointIntersections == 2 && I.endPointIsIntersection[0][0] && I.endPointIsIntersection[0][1]){
			return intersection_type_new::Overlap_outside;
		}
		//Overlap_inside; edge 1 (new) intersects in both its extremes (0 and 1) edge 0 (old)
		if(numEndPointIntersections == 2 && I.endPointIsIntersection[1][0] && I.endPointIsIntersection[1][1]){
			return intersection_type_new::Overlap_inside;
		}
		//Overlap; with only two end point intersections I'm sure to be in this case
		if(	numEndPointIntersections == 2 && (
			(I.endPointIsIntersection[0][0] && I.endPointIsIntersection[1][0]) ||
			(I.endPointIsIntersection[0][0] && I.endPointIsIntersection[1][1]) ||
			(I.endPointIsIntersection[0][1] && I.endPointIsIntersection[1][0]) ||
			(I.endPointIsIntersection[0][1] && I.endPointIsIntersection[1][1])	 )
			){
				return intersection_type_new::Overlap;
		}
		//Overlap_extreme; if one extreme overlaps, there are 3 end point intersections
		if(numEndPointIntersections == 3){
			return intersection_type_new::Overlap_extreme;
		}			
	}
	// If I arrive here:
	//return intersection_type_new::Something_went_wrong;
}		//compute_intersection_type_new

/*
std::ostream &
operator<< (std::ostream & out, Int_layer const& I){
	out << "Number of intersections: " << I.num_intersections << std::endl;
	out << "Intersecting edge: " << I.e << std::endl;
	out << "Intersection points: " << std::endl;
	for(std::size_t i=0; i<num_intersection; ++i)
		out << "\t" << I.int_points[i] << std::endl;
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
	else if(I.how == BGLgeom::intersection_type_new::Overlap_extreme_inside)
		out << "Overlap_extreme_inside";
	else if(I.how == BGLgeom::intersection_type_new::Overlap_extreme_outside)
		out << "Overlap_extreme_outside";
	else if(I.how == BGLgeom::intersection_type_new::Identical)
		out << "Identical";
	else if(I.how == BGLgeom::intersection_type_new::Common_extreme)
		out << "Common_extreme";
	else if(I.how == BGLgeom::intersection_type_new::Something_went_wrong)
		out << "Something went wrong";	
	out << std::endl;
	return out;
}	//operator<<
*/

}		//BGLgeom
