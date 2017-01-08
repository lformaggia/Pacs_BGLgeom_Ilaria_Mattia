/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	intersections2D.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Nov, 2016
	@brief	Classes and functions to compute intersections between two linear edges
	
	@detail We need to define some additional functionality for std::array just because
			the storage type for the points in the library is an Eigen std::array<double,2>, while
			this code (provided by professor Formaggia) is suitable for std::array.
			That's why we provide also some interface classes to transform data in a
			format that can be processed by this code.
*/

#ifndef HH_INTERSECTIONS_2D_HH
#define HH_INTERSECTIONS_2D_HH

#include <array>
#include <limits>
#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>

#include "linear_geometry.hpp"
#include "point.hpp"

#ifndef TOL
#define TOL 20*std::numeric_limits<double>::epsilon()
#endif

//! Helper functions for the algorithms. Using Unnamed namespace
namespace {

//! Scalar product between two std::array<double,2>s
inline double dot(std::array<double,2> const& a, std::array<double,2> const& b){
	return a[0]*b[0] + a[1]*b[1];
}

//! Norm of a std::array<double,2>
inline double norm(std::array<double,2> const& a){
	return std::sqrt(dot(a,a));
}

//! Overload of operator+ for std::array<double,2>
inline std::array<double,2> operator+ (std::array<double,2> const& a, std::array<double,2> const& b){
	return std::array<double,2>{a[0]+b[0], a[1]+b[1]};
}

//! Overload of operator- for std::array<double,2>
inline std::array<double,2> operator- (std::array<double,2> const& a, std::array<double,2> const& b){
	return std::array<double,2>{a[0]-b[0], a[1]-b[1]};
}	

//Both the overloading needed!
//! Overloading of operator* to represent the multiplication of a std::array<double,2> for a scalar
inline std::array<double,2> operator* (double const& k, std::array<double,2> const& a){
	return std::array<double,2>{k*a[0], k*a[1]};
}

/*!
	@brief Solves Ax=b	
	
	@param A The matrix of the system
	@param b Vector of the known terms
	@param tol Tolerance on the determinant
	
	@return std::pair<bool,std::array<double,2>> The bool indicates if there is an intersection.
					In the array there are the parametric coordinates of the intersection
					(defaulted to (0,0)) if there is no intersection)
*/
std::pair<bool, std::array<double,2>> solve 	(std::array<std::array<double,2>,2> const& A,
        										std::array<double,2> const& b,	//qui prima il const& non c'era
        										const double tol){
        										
	auto D = A[0][0]*A[1][1] - A[1][0]*A[0][1];	//determinant
	if (std::abs(D) <= tol)
	    return std::make_pair(false, std::array<double,2>{0,0});
	D=1./D;
	std::array<double,2> res;
	res[0] = D * (A[1][1]*b[0] - A[0][1]*b[1]);
	res[1] = D * (A[0][0]*b[1] - A[1][0]*b[0]);
	return std::make_pair(true, res);
}	//solve

}	//namespace

namespace BGLgeom{

/*! 
	@brief An interface for a linear edge
	@detail This class is thought to manage the description of the geometry
			of a linear edge, in order to compute intersections.
	@remarks The class must have an overload of operator[] in order to run in the 
				function that computes intersections
*/
class linear_edge_interface{
	public:
		//! Default constructor
		linear_edge_interface	(BGLgeom::linear_geometry<2> edge){
			extremes[0][0] = edge.get_source()(0,0);
			extremes[0][1] = edge.get_source()(0,1);
			extremes[1][0] = edge.get_target()(0,0);
			extremes[1][1] = edge.get_target()(0,1);
		
		}
	
		//! Constructor
		//linear_edge_interface(BGLgeom::point<2> const& SRC, BGLgeom::point<2> const& TGT) : extremes{SRC, TGT}, extremes_are_set(true) {};
		
		//! Copy constructor
		linear_edge_interface(linear_edge_interface const&) = default;
		
		//! Assignment operator
		linear_edge_interface & operator=(linear_edge_interface const&) = default;
		
		/*
		//! Setting the two end points (extremes) of the edge
		void set(point<2> const& SRC, point<2> const& TGT){
			extremes[0] = SRC;
			extremes[1] = TGT;
			extremes_are_set = true;
		}
		*/
		/*! 
			@brief Overloading of operator[] to access each of the two end points. Usefull in algorithms
			@detail extremes[0] = source, extremes[1] = target of the edge
		*/
		std::array<double,2> operator[](std::size_t i){ return extremes[i]; }
		std::array<double,2> operator[](std::size_t i) const { return extremes[i]; }
				
	private:
		//! This container contains coordinates of source (first row), coordinates of target (second row)
		std::array<std::array<double,2>, 2> extremes;
};	//linear_edge_interface


/*!
	@brief An enum class to identify the intersection situation
	@detail We need to identify how the edges are located in the plane and
			how the intersect. In this way we can perform the correct actions
			at the graph level to intersect topologically the edges
	
	@param X Simple cross intersection. We are happy
	@param T_new The new edge intersects in one of its extremes the old edge
	@param T_old The old edge intersects in one of its extremes the new edge
	@param Overlap_outside The new edge is overlapped to the old one and it is larger
	@param Overlap_inside The new edge is overlapped to the old one and it is smaller
	@param Overlap The new edge overlap in some part (not all) the old edge
	@param Overlap_extreme_inside The new edge (larger) is overlappes to the old one and coincides in one extreme
	@param Overlap_extreme_outside The new edge (smaller) is overlappes to the old one and coincides in one extreme
	@param Identical The two edges are the same
	@param Common_extreme The two edges have only one extreme in common
	@param Something_went_wrong Error in computing the intersection
	@param No_intersection There are no intersection between the edges
*/
enum class intersection_type	{X, T_new, T_old, Overlap_outside, Overlap_inside, Overlap,
								Overlap_extreme_inside, Overlap_extreme_outside, Identical,
								Common_extreme, Something_went_wrong, No_intersection};


//! A simple struct that contains the result of the intersection test
/*!
	To be able to treat the most general case each segment is allowed
	to have up to two intersections. It happens if the segments overlaps

	@todo It can be bettered by adding another attribute that indicates,
			in the case of two edges end which coincides the relative position on the 
			edge. It requires a simple modification of the function segmentIntersect
	@note Piece of code provided by prof. Formaggia
*/
struct Intersection {
	//! Segments interects! True if there is intersection
	bool intersect = false;
	//! How the two segments intersect
	BGLgeom::intersection_type how;
	//! Number of intersections (max 2, rapresenting the case in which the segments overlap)
	unsigned int numberOfIntersections = 0u;
	//! Intersection points coordinates
	std::array<BGLgeom::point<2>,2> intersectionPoint = std::array<BGLgeom::point<2>,2>{BGLgeom::point<2>(), BGLgeom::point<2>()};
	/*! Intersection may be end point:	    
	    endPointIsIntersection[i][j]=true
	    then end j of edge i is at the intersection
	*/
	std::array<std::array<bool,2>, 2> endPointIsIntersection =
	    std::array<std::array<bool,2>, 2>{std::array<bool,2>{false,false}, std::array<bool,2>{false,false} };
	/*! EdgeS join at the end.
	    In that case endPointIsIntersection will be true and the corresponding
	    entry will indicate the numering of the end of the other edge.
	    -1 indicates that the end is not joined. So
	    if endPointIsIntersection[i][j]=true we have
	    otherEdgePoint[i][j]=-1 //End point is not joined with the other edge
	    otherEdgePoint[i][j]= k //End point j of edge j is joined with end point k of other edge
	    */
	std::array<std::array<int,2>, 2> otherEdgePoint =
	    std::array<std::array<int,2>, 2>{std::array<int,2>{-1,-1}, std::array<int,2>{-1,-1}};
	//! Edges are parallel
	bool parallel = false;
	//! Edges are identical
	bool identical = false;
	//! Edges are collinear (and thus also parallel)
	bool collinear = false;
	//! Something is not ok
	bool good = true;
	//! Distance, makes sense only if parallel=true
	double distance = 0.0;
};	//Intersection


/*!
@brief Computes intersection betweeen two edges

It handles also the case of intersection at the segment ends
@note  It is not so rubust because it uses the tolerances in a 
different way: the tolerance tol to test the parametric coordinate
along the edge line and a scaled tolerance to check distances.
Another scaled tolerance is used to test ir edges are parallel.

@pre The edges must have non null length
@par S1 First Edge
@par S2 Second Edge
@return Intersection. A data structure containing the info about the intersection
*/
Intersection compute_intersection	(linear_geometry<2> const& edge1,
									 linear_geometry<2> const& edge2);
                           		
/*
	@brief Overload of operator<< to show the infos obtained by the function
*/
std::ostream & operator<< (std::ostream & out, Intersection const& I);

/*
	@brief Helper function to translate array into eigen
	@detail Needed to suite the working structure of comupte_intersection
			with the structure used for points in this library, that is Eigen
*/
std::array<BGLgeom::point<2>,2>
translate_array_to_eigen(std::array<std::array<double,2>,2> const& array, unsigned int const& numberOfIntersection);

void compute_intersection_type(Intersection & out);

} //BGLgeom

#endif	//HH_INTERSECTIONS_2D_HH
