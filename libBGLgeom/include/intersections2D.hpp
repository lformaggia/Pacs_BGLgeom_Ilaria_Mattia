/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*
   Copyright (C) 2016 Luca Formaggia
   
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
	@file	intersections2D.hpp
	@author	Luca Formaggia, Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Classes and functions to compute intersections between two linear edges
	
	Most part of this code (the unnamed namespace and the function compute 
	intersection) was provided by prof. Formaggia. We refactored some part 
	of it to provide an interface with our library (for instance we have to 
	translate the original std::array structure with our Eigen structure we 
	use to describe points), and we added newer features, such as the 
	identification of the type of intersection.
	
	@remark	This tools are able to handle only intersections between linear
			edges in a 2D space
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

//! Helper functions for the algorithms. 
namespace BGLgeom{
/*!
	@defgroup Helper_functions Helper functions for the function compute_intersection
*/
//! Scalar product between two std::array<double,2>
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
        										std::array<double,2> const& b,
        										double const& tol);
/*! @} */


/*! 
	@brief An interface for a linear edge
	
	This class will manage the description of the geometry of a linear 
	edge in the code which computes intersections
	
	@remark	The class must have an overload of operator[] in order to 
			run in the function that computes intersections
*/
class linear_edge_interface{
	public:
		//! Default constructor
		linear_edge_interface(BGLgeom::linear_geometry<2> edge){
			extremes[0][0] = edge.get_source()(0,0);
			extremes[0][1] = edge.get_source()(0,1);
			extremes[1][0] = edge.get_target()(0,0);
			extremes[1][1] = edge.get_target()(0,1);		
		}
	
		//! Copy constructor
		linear_edge_interface(linear_edge_interface const&) = default;
		
		//! Move constructor
		linear_edge_interface(linear_edge_interface &&) = default;
		
		//! Assignment operator
		linear_edge_interface & operator=(linear_edge_interface const&) = default;
		
		//! Move assignment
		linear_edge_interface & operator=(linear_edge_interface &&) = default;
		
		//! Destructor
		virtual ~linear_edge_interface() = default;		
		
		/*! 
			@brief Overloading of operator[] to access each of the two end points. Usefull in algorithms
			
			extremes[0] = source, extremes[1] = target of the edge
		*/
		std::array<double,2> operator[](std::size_t i){ return extremes[i]; }
		std::array<double,2> operator[](std::size_t i) const { return extremes[i]; }
				
	private:
		//! Container for the coordinates of source (first row) and the coordinates of target (second row)
		std::array<std::array<double,2>, 2> extremes;
};	//linear_edge_interface


/*!
	@brief An enum class to identify the intersection situation
	
	We need to identify how the edges are located in the plane and
	how they intersect. In this way we can perform the correct actions
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
	@param Something_went_wrong Some error happened while computing the intersection
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
	
	@note	It is not so robust because it uses the tolerances in a 
			different way: the tolerance tol to test the parametric coordinate
			along the edge line and a scaled tolerance to check distances.
			Another scaled tolerance is used to test if edges are parallel.

	@pre The edges must have non null length
	@par S1 First Edge
	@par S2 Second Edge
	@return Intersection. A data structure containing the info about the intersection
*/
Intersection compute_intersection(linear_geometry<2> const& edge1,
								  linear_geometry<2> const& edge2);
                           		
//! Overload of operator<< to show the infos obtained by the function
std::ostream & operator<< (std::ostream & out, Intersection const& I);

/*!
	@brief Computing type of intersection
	
	This function is able to analize the output of the function 
	compute_intersection to find which is the intersection situation 
	among those described in the enum intersection_type. This 
	operation allows to treat each case in an easier way when 
	building the graph through successive addition of edges
	
	@param I The object of class Intersection returned by the 
			function compute_intersection. It is passed by reference 
			the intersection type will be stored in the same object
*/
void compute_intersection_type(Intersection & I);

/*
	@brief Helper function to translate array into eigen
	
	Needed to suite the working structure of comupte_intersection
	with the structure used for points in this library, i.e. Eigen
*/
std::array<BGLgeom::point<2>,2>
translate_array_to_eigen(std::array<std::array<double,2>,2> const& array, unsigned int const& numberOfIntersection);

} //BGLgeom

#endif	//HH_INTERSECTIONS_2D_HH
