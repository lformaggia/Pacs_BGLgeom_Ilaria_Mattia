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
	@detail 
*/

/// Ecco il trucco forse!!! Una volta stabilita tutta la geometria, questa classe dovrebbe funzionare sempre allo stesso modo. Cioè, tipo: le intersezioni si troveranno sempre nella stessa maniera, i punti e gli edge saranno tutti descritti alla stessa maniera, ecc.

#ifndef HH_INTERSECTOR_BASE_CLASS_HH
#define HH_INTERSECTOR_BASE_CLASS_HH

#include <vector>
#include <tuple>
#include <boost/graph/graph_traits.hpp>

#include "generic_point.hpp"

template <typename Graph>
class intersector_base_class {
	public:
	
		typedef typename boost::graph_traits<Graph>::edge_descriptor Edge_desc;
		typedef typename boost::graph_traits<Graph>::edge_iterator Edge_iter;
		typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
		typedef std::pair<point<2>, point<2> > Line;
		//typedef typename std::vector<std::pair<point<2>, Edge_desc> > Intersections_container;
		
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
		std::vector<std::pair<point<2>, Edge_desc> > intersections;
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
