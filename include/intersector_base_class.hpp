/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file intersector_base_class.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Abstract class to handle intersections of edges in a graph with geometrical properties
	@detail It contains also some utilities needed to compute the intersection between two (linear) edges
*/

/// Ecco il trucco forse!!! Una volta stabilita tutta la geometria, questa classe dovrebbe funzionare sempre allo stesso modo. Cioè, tipo: le intersezioni si troveranno sempre nella stessa maniera, i punti e gli edge saranno tutti descritti alla stessa maniera, ecc.

#ifndef HH_INTERSECTOR_BASE_CLASS_HH
#define HH_INTERSECTOR_BASE_CLASS_HH

#include <vector>
#include <array>
#include <tuple>
#include <cmath>
#include <limits>
#include <boost/graph/graph_traits.hpp>

#include "generic_point.hpp"
#include "point2D.hpp"

typedef std::array<double,2> Vector;

//! Helper functions for operations with points and vectors. Using an anonimous namespace.
namespace{
	//! Scalar product between two vectors
	inline double dot(Vector const& a, Vector const& b){
		return a[0]*b[0] + a[1]*b[1];
	}
	
	//! Norm of a vector
	inline double norm(Vector const& a){
		return std::sqrt(dot(a,a));
	}
	
	//! Overloading of operator+ for std::array<double,2>
	inline Vector operator+ (point2D const& P, point2D const& Q){
		return Vector{P[0]+Q[0], P[1]+Q[1]};
	}
	
	//! Overloading of operator- for std::array<double,2>
	inline Vector operator- (point2D const& P, point2D const& Q){
		return Vector{P[0]-Q[0], P[1]-Q[1]};
	}
	
	//Both the overloading needed!
	//! Overloading of operator* to represent the multiplication of a vector for a scalar
	inline Vector operator* (double const& k, Vector const& a){
		return Vector{k*a[0], k*a[1]};
	}
	
	//! Overloading of operator* to represent the multiplication of the coordinates of a point for a scalar
	//Se qualcosa non funziona, provo a mettere Vector come tipo del valore restituito
	inline point2D operator* (double const& k, point2D const& P){
		return point2D(k*P[0], k*P[1]);
	}
	
	//! Solves Ax=b
	/*!
		@param tol Tolerance on the determinant
		@return std::pair<bool,std::array<double,2>> The bool indicates if there is an intersection. In the array there are the parametric coordinates of the intersection (defaulted to (0,0) if there is no intersection)
	*/
	std::pair<bool, Vector>
	solve (std::array<Vector,2> const& A,
	        Vector b,
	        double const& tol)
	{
	auto D = A[0][0]*A[1][1] - A[1][0]*A[0][1];
	if (std::abs(D) <= tol)
	    return std::make_pair(false, Vector{0,0});
	D=1./D;
	Vector res;
	res[0] = D * (A[1][1]*b[0] - A[0][1]*b[1]);
	res[1] = D * (A[0][0]*b[1] - A[1][0]*b[0]);
	return std::make_pair(true, res);
	}	//solve
}	//namespace


namespace BGLgeom{
	/*! 
		@brief A simple class that hanlde a linear edge
		@detail This class is thought to manage the description of the geometry
				of a linear edge, in order to compute intersections.
		@remarks The class must have an overload of operator[] in order to run in the 
					function that computes intersections
	*/
	class Linear_edge{
		public:
			//! Default constructor
			Linear_edge() : extremes{point2D(), point2D()}, extremes_are_set(false) {};
		
			//! Constructor
			Linear_edge(point2D const& SRC, point2D const& TGT) : extremes{SRC, TGT}, extremes_are_set(true) {};
			
			//! Copy constructor
			Linear_edge(Linear_edge const&) = default;
			
			//! Assignment operator
			Linear_edge & operator=(Linear_edge const&) = default;
			
			//! Setting the two end points (extremes) of the edge
			void set(point2D const& SRC, point2D const& TGT){
				extremes[0] = SRC;
				extremes[1] = TGT;
				extremes_are_set = true;
			}
			
			//! Overloading of operator[] to access each of the two end points. Usefull in algorithms
			point2D operator[](std::size_t i){ return extremes[i]; }
			point2D operator[](std::size_t i) const { return extremes[i]; }
					
		private:
			//! This container contains coordinates of source (first row), coordinates of target (second row)
			std::array<point2D,2> extremes;
			//! Bool to track if the coordinates in the matrix is set
			bool extremes_are_set;
	};	//Linear_edge
	
	
	//! A simple struct that contains the result of the intersection test
	/*!
		To be able to treat the most general case each segment is allowed
		to have up to two intersections. It happens if the segments overlaps

		@todo It can be bettered by adding another attribute that indicates,
				in the case of two edges end which coincides the relative position on the 
				edge. It requires a simple modification of the function segmentIntersect
		@note Piece of code provided by prof. Formaggia
	*/
	struct
	Intersection
	{
	//! Segments intersects
	/*! 
	    True is there is any intersection
	    */
	bool intersect = false;
	//! Number of intersections (max 2)
	unsigned int numberOfIntersections = 0u;
	//! Intersection points coordinates
	std::array<point2D,2> intersectionPoint = std::array<point2D,2>{point2D(), point2D()};
	/*! Intersection may be end point
	    
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
		@brief Function to compute intersection between two linear edges
		@note We can make it template on the class of edge that we pass to the function.
				In this case we implemented only Linear_edge to describe segments, but what if
				the edges are curves in the plane (De Falco application)? In this way,
				with another class of "type_of_edge" that fits to the functions, there is 
				no need to rewrite it.
		@note Piece of code provided by prof. Formaggia
	*/
	template <typename Edge_t = Linear_edge>
	Intersection compute_intersection	(Edge_t const& Edge1,
										Edge_t const& Edge2,
										double const tol = 20*std::numeric_limits<double>::epsilon()){
								
		Intersection out;
    //[0][0] ->1
    //[0][1] ->2
    //[1][0] ->3
    //[1][1] ->4
    auto v1= Edge1[1]-Edge1[0];
    auto v2= Edge2[1]-Edge2[0];
    auto len1 = norm(v1);
    auto len2 = norm(v2);
    // Tolerance for distances
    double tol_dist=tol*std::max(len1,len2);
    // I need to scale the tolerance to check
    // parallelism correctly
    auto tol_sys=2.0*tol*len1*len1*len2*len2;
    // First check if segments meets at the end
    for (unsigned int i=0;i<2;++i)
      {
        auto const & P1=Edge1[i];
        for (unsigned int j=0;j<2;++j)
          {
            auto const & P2=Edge2[j];
            auto dist=norm(P1-P2);
            if (dist<=tol_dist)
              {
                if(out.numberOfIntersections>=2)
                  {
                    std::cerr<<
                      "Something wrong, cannot have more that 2 intersections"
                             <<std::endl;
                    out.good=false;
                    return out;
                  }
                out.intersect=true;
                out.intersectionPoint[out.numberOfIntersections++]=P1;
                out.endPointIsIntersection[0][i]=true;
                out.endPointIsIntersection[1][j]=true;
                out.otherEdgePoint[0][i]=j;
                out.otherEdgePoint[1][j]=i;
              }
          }
      }
    // Handle the case where the two edges are identical!
    if (out.numberOfIntersections==2u)
      {
        out.identical=true;
        out.parallel=true;
        out.collinear=true;
        return out;
      }
    // Now solve the linear system that returns the
    // parametric coordinates of the intersection
    std::array<std::array<double,2>,2> A;
    // to make life easier I call A and B the
    // ends of the two segmensts
    auto const & A1=Edge1[0];
    auto const & B1=Edge1[1];
    auto const & A2=Edge2[0];
    auto const & B2=Edge2[1];
    auto  V1 = B1-A1;
    auto  V2 = B2-A2;
    A[0][0]= dot(V1,V1);
    A[0][1]=-dot(V1,V2);
    A[1][0]= A[0][1];
    A[1][1]= dot(V2,V2);
    std::array<double,2> b;
    b[0]=    dot(A2-A1,V1);
    b[1]=    dot(A1-A2,V2);
    // find parametric coordinate of the intersection
    auto result=solve(A,b,tol_sys);
    // Standard case, the two lines are not parallel
    if(result.first==true)
      {
        auto const & t = result.second;
        // Make a stupid check (only in debugging phase)
#ifndef NDEBUG
        auto P1 = A1+ t[0]*(B1-A1);
        auto P2 = A2+ t[1]*(B2-A2);
        if(norm(P1-P2)>tol_dist)
          std::cerr<<" Something strange, intersection points not coincident. Distance= "<<norm(P1-P2);
#endif        
        // The two lines intersect.
        // Check whether we are inside the segments
        double const & t1=result.second[0];
        double const & t2=result.second[1];
        //double tol1=tol/len1;
        //double tol2=tol/len2;
        bool inside =
          (t1>=-0.5*tol) && (t1<= 1.0+0.5*tol) && 
          (t2>=-0.5*tol) && (t2<= 1.0+0.5*tol);
        if (!inside)
          {
            // No intersecion, end here
            return out;
          }
        else
          {
            out.intersect=true;
            // I coud have used the mean
            if (std::abs(t1    )<= tol)
              {
                if(out.endPointIsIntersection[0][0])
                  {
                    // already found. End here
                    return out;
                  }
                else
                  {
                    out.endPointIsIntersection[0][0]=true;
                  }
              }
            if (std::abs(t1-1.0)<= tol)
              {
                if(out.endPointIsIntersection[0][1])
                  {
                    // already found. End here
                    return out;
                  }
                else
                  {
                    out.endPointIsIntersection[0][1]=true;
                  }
              }
            if (std::abs(t2    )<= tol)
              {
                if(out.endPointIsIntersection[1][0])
                  {
                    // already found. End here
                    return out;
                  }
                else
                  {
                    out.endPointIsIntersection[1][0]=true;
                  }
              }
            if (std::abs(t2-1.0)<= tol)
              {
                if(out.endPointIsIntersection[1][1])
                  {
                    // already found. End here
                    return out;
                }
                else
                  {
                    out.endPointIsIntersection[1][1]=true;
                  }
              }
            out.intersectionPoint[out.numberOfIntersections++]=A1+ t1*(B1-A1);
            return out;
          }
      }  
    else
      {
        out.parallel=true;
      // Compute distance between the two lines
      /*
        (x-Edge1(0,0))(Edge1(1,1)-Edge1(0,1))-(Edge1(1,0)*Edge1(0,0))*(y-Edge1(0,1))=0
        (Edge1(1,1)-Edge1(0,1))x+(Edge1(0,0)*Edge1(1,0))y+Edge1(0,1)(Edge1(1,0)*Edge1(0,0))-
        (Edge1(1,1)-Edge1(0,1))Edge1(0,0)=0
      */
        double factor=dot(B1-A1,A2-A1)/(len1*len1);
        auto distVect=
          (A1-A2) + factor*(B1-A1);
        double distance = norm(distVect);
        out.distance=distance;
        // Maybe here you want to use a different tolerance?
        if (distance>tol_dist)
          {
            // The two segments are separated
            // No intersection.
            return out;
          }
        else
          {
            out.collinear=true;
            double t;
            // Segments are collinear!
            // We need to consider all the cases!
            // IS A2 on Edge1?
            // Maybe I have already considered it earlier
            if(!out.endPointIsIntersection[1][0])
              {
                t=dot((A2-A1),(B1-A1))/(len1*len1);
                if(t>=-0.5*tol && t<=1+0.5*tol)
                  {
                    out.intersect=true;
                    out.intersectionPoint[out.numberOfIntersections++]=A2;
                    out.endPointIsIntersection[1][0]=true;
                    if(out.numberOfIntersections==2)return out;
                  }
              }
            // IS B2 on Edge1?
            // Maybe I have already considered it earlier
            if(!out.endPointIsIntersection[1][1])
              {
                t=dot((B2-A1),(B1-A1))/(len1*len1);
                if(t>=-0.5*tol && t<=1+0.5*tol)
                  {
                    out.intersect=true;
                    out.intersectionPoint[out.numberOfIntersections++]=B2;
                    out.endPointIsIntersection[1][1]=true;
                    if(out.numberOfIntersections==2)return out;
                  }
              }
            // IS A1 on Edge2?
            // Maybe I have already considered it earlier
            if(!out.endPointIsIntersection[0][0])
              {
                t=dot((A1-A2),(B2-A2))/(len2*len2);
                if(t>=-0.5*tol && t<=1+0.5*tol)
                  {
                    out.intersect=true;
                    out.intersectionPoint[out.numberOfIntersections++]=A1;
                    out.endPointIsIntersection[0][0]=true;
                    if(out.numberOfIntersections==2)return out;
                  }
              }
            // IS B1 on Edge2?
            // Maybe I have already considered it earlier
            if(!out.endPointIsIntersection[0][1])
              {
                t=dot((B1-A2),(B2-A2))/(len2*len2);
                if(t>=-0.5*tol && t<=1+0.5*tol)
                  {
                    out.intersect=true;
                    out.intersectionPoint[out.numberOfIntersections++]=B1;
                    out.endPointIsIntersection[0][1]=true;
                    if(out.numberOfIntersections==2)return out;
                  }
              }
          }
      }
    return out;						
	
	
	
	
	
	}
	
	
}	//BGLgeom





template <typename Graph>
class intersector_base_class {
	public:
	
		typedef typename boost::graph_traits<Graph>::edge_descriptor Edge_desc;
		typedef typename boost::graph_traits<Graph>::edge_iterator Edge_iter;
		typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
		typedef std::pair<point<2>, point<2> > Line;
		typedef typename std::vector<std::pair<point<2>, Edge_desc> > Intersections_type;
		typedef typename std::pair<point<2>, Edge_desc>	Intersections_value_type;
		
		//! Default constructor (initialization of the reference to the graph needed)
		intersector_base_class():	
											Edge1(),
											Edge2(),
											intersections(),
											intersection_point(),
											Edge2_descriptor() {};

		//! Copy constructor
		intersector_base_class(intersector_base_class const&) = default;
		
		//! Destructor
		virtual ~intersector_base_class(){};
		
		//! Assignment operator
		intersector_base_class & operator=(intersector_base_class const&) = default;
		
		//======================= ATTRIBUTES SETTING OPERATIONS ====================
		
		//! It sets Edge1 from two points
		virtual void set_Edge1(point<2> const& P1, point<2> const& P2){
			Edge1 = std::make_pair(P1,P2);
		};
		
		//! It sets Edge1 from another Line
		virtual void set_Edge1(Line _L){
			Edge1 = _L;
		};
		
		//! It sets Edge2 from two points
		virtual void set_Edge2(point<2> const& P1, point<2> const& P2){
			Edge2 = std::make_pair(P1,P2);
		};
		
		//! It sets Edge1 from another Line
		virtual void set_Edge2(Line _L){
			Edge2 = _L;
		};
		
		//! It allows to set Edge2_descriptor
		virtual void set_Edge2_descriptor(Edge_desc _Edge2_desc){
			Edge2_descriptor = _Edge2_desc;
		};
		
		//================ INTERSECTION OPERATIONS ======================
		
		/*! 
			@brief It checks if Edge1 and Edge2 are actually intersected
			@detail If the two edge are intersecating, this method must store in the class variable
					intersection_point the coordinates of the intersection found
		*/
		// Qui ci andrà la funzione di formaggia e non sarà più abstract!
		virtual bool are_intersected() = 0;
		
		//! It pushes back a new intersection point between Edge1 and Edge2, remembering the edge descriptor of Edge2
		virtual void store_intersection(){
			intersections.push_back(std::make_pair(intersection_point, Edge2_descriptor));	
		};	//compute_intersections
		
		virtual void clear_intersections(){
			intersections.clear();
		};
		
		/*!
			@brief It explains how to rebuilt graph after the intersections were computed
			@detail It has to inteface with private attributes of the derived class in order
					to set edge and vertex properties in the right way.
		*/
		virtual void refine_graph() = 0;
		
		//===================== SORTING UTILITIES =============================//
		
		/*!
			@brief This reorders the vector intersections according to some order, defined by the user
			@detail It will consist of a call to "sort" algorithm, in which the compare function
					must be user defined, choosing a possible ordering in the 2D space
		*/
		virtual void order_intersections() = 0;
		
		//Not provided methods to give ordering, because it wouldn't work inside a std::sort. Use lambda functions instead

	protected:
		//Non ho bisogno del grafo in realtà, qua si gestiscono solo le operazioni geometriche tra due edge qualsiasi, nn su tt il grafo
		
		/*!
			@brief The first of the two edge that are (maybe) intersecating
			@detail If the user has to perform multiple intersection between a fixed edge
					and all the other edges in the graph, Edge1 is thougth to be the fixed edge.
		*/
		Line Edge1;
		/*!
			@brief The second of the two edge that are (maybe) intersecating
			@detail If the user has to perform multiple intersection between a fixed edge
					and all the other edges in the graph, Edge2 is thougth to be the variable edge.
		*/
		Line Edge2;
		//!	Vector that will contains the intersection point and the edge descriptor of the edge with which the current edge is intersecating
		Intersections_type intersections;
		//! The intersection point between Edge1 and Edge2 (if present) 
		point<2> intersection_point;
		/*!
			@brief Edge descriptor of Edge2.
			@detail If the user has to perform multiple intersections between Edge1 (fixed) and 
					Edge2 (variable), this tracks the edge descriptor of the edges in the graph
					that are intersecating Edge1 (one at a time) 
		*/
		Edge_desc Edge2_descriptor;
		
};	//intersector_base_class

#endif	//HH_INTERSECTOR_BASE_CLASS_HH
