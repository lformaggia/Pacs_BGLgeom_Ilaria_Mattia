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
		
		//enum {src_greater_than_tgt, src_less_than_tgt};
		
		//! Default constructor (initialization of the reference to the graph needed)
		intersector_base_class(Graph & _G):	G(_G),
											line1(),
											line2(),
											intersections(),
											src_int_edge(), 
											tgt_int_edge(),
											intersection_point(),
											current_frac_number(0),
											Edge2_descriptor() {};
		
		//! Constructor with single points for line1
		intersector_base_class	(Graph & _G,
								point<2> const& _SRC,
								point<2> const& _TGT) :	G(_G), 
														Edge1(_SRC, _TGT),
														Edge2(),
														intersections(),
														src_int_edge(),
														tgt_int_edge(),
														intersection_point(),
														current_frac_number(0),
														Edge2_descriptor() {};
		
		//! Constructor with already a line
		intersector_base_class(Graph & _G, Line _Edge1) : 	G(_G),
																	Edge1(_Edge1),
																	Edge2(),
																	intersections(),
																	src_int_edge(),
																	tgt_int_edge(),
																	intersection_point()
																	current_frac_number(0),
																	Edge2_descriptor() {};
		
		//! Copy constructor
		intersector_base_class(intersector_base_class const&) = default;
		
		//! Destructor
		virtual ~intersector_base_class(){};
		
		//! Assignment operator
		intersector_base_class & operator=(intersector_base_class const&) = default;
		
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
		
		//! It allows to set the value of current_frac_number
		virtual void set_current_frac_number(unsigned int const& _frac_number){
			current_frac_number = _frac_number;
		};
		
		//! It allows to set Edge2_descriptor
		virtual void set_Edge2_descriptor(Edge_desc _Edge2_desc){
			Edge2_descriptor = _Edge2_desc;
		};
		
		/*! 
			@brief It checks if Edge1 and Edge2 are actually intersected
			@detail If the two edge are intersecating, this method must store in the class variable
					intersection_point the coordinates of the intersection found
		*/
		virtual bool are_intersected() = 0;		
		
		//! It pushes back a new intersection point in the vector intersections
		virtual void store_intersection(){
			intersections.push_back(std::make_pair(intersection_point, Edge2_descriptor);	
		};	//compute_intersections
		
		virtual void clear_intersections(){
			
		};
		
		/*!
			@brief It explains how to rebuilt graph after the intersections were computed
			@detail It has to inteface with private attributes of the derived class in order
					to set edge and vertex properties in the right way.
		*/
		void refine_graph() = 0;
		
		//===================== SORTING UTILITIES =============================//
		
		/*!
			@brief This reorders the vector intersections according to some order, defined by the user
			@detail It will consist of a call to "sort" algorithm, in which the compare function
					must be user defined, choosing a possible ordering in the 2D space
		*/
		virtual void order_intersections() = 0;
		
		/*!
			@brief This is one possible method to compare teo element in the vector intersections
			@detail It is thougth to be used when source of Edge1 is "less than" (according to the
					user defined ordering) the target of Edge1
		*/
		virtual bool src_less_than_tgt	(std::pair<point<2>, Edge_desc> intersection_vector_elem1,
								 		 std::pair<point<2>, Edge_desc> intersection_vector_elem2) = 0;
		
		/*!
			@brief This is one possible method to compare teo element in the vector intersections
			@detail It is thougth to be used when source of Edge2 is "less than" (according to the
					user defined ordering) the target of Edge2
		*/
		virtual bool src_greater_than_tgt	(std::pair<point<2>, Edge_desc> intersection_vector_elem1,
					 		 	 	 		 std::pair<point<2>, Edge_desc> intersection_vector_elem2) = 0;
		
		
		/*
		//! Used to sort in the right way two elements of the vector intersections
		template<bool src_less_than_tgt>
		bool sort_intersection_points	(std::pair<point<2>, Edge_desc> intersection_vector_elem1,
					 		 			 std::pair<point<2>, Edge_desc> intersection_vector_elem2){
			if(src_less_than_tgt)
				return intersection_vector_elem1.first < intersection_vector_elem2.first;
			else 
				return intersection_vector_elem1.first > intersection_vector_elem2.first;
		};	//sort_intersection_points
		*/
		

		
	
	protected:
		//! Graph of which we want to calculate intersections. By reference to save memory.
		Graph & G;
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
		//! Vertex_descriptor of the Edge2
		Vertex_desc src_int_edge, tgt_int_edge;
		//! The intersection point between Edge1 and Edge2 (if present) 
		point<2> intersection_point;
		//! Number of the fracture which the current edge belongs to
		unsigned int current_frac_number;
		/*!
			@brief Edge descriptor of Edge2.
			@detail If the user has to perform multiple intersections between Edge1 (fixed) and 
					Edge2 (variable), this tracks the edge descriptor of the edges in the graph
					that are intersecating Edge1 (one at a time) 
		*/
		Edge_desc Edge2_descriptor;
		
};	//intersector_base_class

#endif	//HH_INTERSECTOR_BASE_CLASS_HH
