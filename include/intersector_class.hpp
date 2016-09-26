/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file intersector_class.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Class to handle intersections of edges in a graph with geometrical properties
	@detail 
*/

#ifndef HH_INTERSECTOR_CLASS_HH
#define HH_INTERSECTOR_CLASS_HH

#include <vector>
#include <tuple>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>

#include "generic_point.hpp"

template <typename Graph>
class intersector {
	public:
	
		typedef typename boost::graph_traits<Graph>::edge_descriptor Edge_desc;
		typedef typename boost::graph_traits<Graph>::edge_iterator Edge_iter;
		typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
		typedef std::pair<point<2>, point<2> > Line;
		
		enum {src_greater_than_tgt, src_less_than_tgt};
		
		//! Default constructor (initialization of the reference to the graph needed)
		intersector(Graph & _G):	G(_G),
									line1(),
									line2(),
									intersections(),
									src_int_edge(), 
									tgt_int_edge(),
									intersection_point() {};
		
		//! Constructor with single points for line1
		intersector	(Graph & _G,
					point<2> const& _SRC,
					point<2> const& _TGT) :	G(_G), 
											current_edge(_SRC, _TGT),
											intersection_edge(),
											intersections(),
											src_int_edge(),
											tgt_int_edge(),
											intersection_point() {};
		
		//! Constructor with already a line
		intersector(Graph & _G, Line _current_edge) : 	G(_G),
														current_edge(_current_edge),
														intersection_edge(),
														intersections(),
														src_int_edge(),
														tgt_int_edge(),
														intersection_point() {};
		
		//! Copy constructor
		intersector(intersector const&) = default;
		
		//! Destructor
		virtual ~intersector(){};
		
		//! Assignment operator
		intersector & operator=(intersector const&) = default;
		
		//! It sets current_edge from two points
		void set_current_edge(point<2> const& P1, point<2> const& P2){
			current_edge = std::make_pair(P1,P2);
		};
		
		//! It sets current_edge from another Line
		void set_current_edge(Line _L){
			current_edge = _L;
		};
		
		//! It sets intersection_edge from two points
		void set_intersection_edge(point<2> const& P1, point<2> const& P2){
			intersection_edge = std::make_pair(P1,P2);
		};
		
		//! It sets current_edge from another Line
		void set_intersection_edge(Line _L){
			intersection_edge = _L;
		};
		
		//solo con fratture orizzontali e verticali!!! Poi credo sarebbe da mettere fuori, non inline
		//! It checks if current_edge and intersection_edge are actually intersecating. It stores the coordinates of the intersection point, if present, in intersection_point.
		bool are_intersected(){
			bool vertical1 = false;
			bool vertical2 = false;
			double x_intersect, y_intersect;
			
			if(current_edge.first.x() == current_edge.second.x()){
				vertical1 = true;
				x_intersect = current_edge.first.x();
			}
			else
				y_intersect = current_edge.first.y();
			
			if(intersection_edge.first.x() == intersection_edge.second.x()){
				vertical2 = true;
				x_intersect = intersection_edge.first.x();
			}
			else
				y_intersect = intersection_edge.first.y();
			
			if (vertical1 + vertical2 == 1){
				intersection_point.set({x_intersect, y_intersect});
				return true;
			}
			else
				return false;			
		};	//are_intersected
		
		
		//! It fills intersection_vector with all the intersections founded
		void compute_intersections(){
			Edge_iter e_it, e_end;
			for(std::tie(e_it, e_end) = edged(G); e_it != e_end; ++e_it){
				//creting the intersection edge
				src_int_edge = boost::source(*e_it, G);
				tgt_int_edge = boost::target(*e_it, G);
				this->set_intersection_edge(G[src_int_edge].coord, G[tgt_int_edge].coord);
				
				if(this->are_intersected())
					intersections.push_back(std::make_pair(intersection_point, *e_it);				
			}	//for
		};	//compute_intersections
		
		//! It reorders the intersections point founded in order to create the new smaller edge in the right way.
		void order_intersections(){
			if(current_edge.first < current_edge.second)
				std::sort(intersections.begin(), intersections.end(), this->sort_points<Graph, src_less_than_tgt>);
			else
				std::sort(intersections.begin(), intersections.end(), this->sort_points<Graph, src_greater_than_tgt>);
		};
		
		//! Used to sort in the right way two elements of the vector intersections
		template<typename Graph, bool src_less_than_tgt>
		bool sort_points	(std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> intersection_vector_elem1,
					 		 std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> intersection_vector_elem2){
			
			if(src_less_than_tgt)
				return intersection_vector_elem1.first < intersection_vector_elem2.first;
			else 
				return intersection_vector_elem1.first > intersection_vector_elem2.first;
		}	//sort_points
	
	protected:
		//! Graph of which we want to calculate intersections. By reference to save memory.
		Graph & G;
		//! Edge that we are checking if intersecates some other edge in the graph
		Line current_edge;
		//! Edge that loops over all the edges in the graph to check if intersecates current_edge
		Line intersection_edge;
		//!	Vector that will contains the intersection point and the edge descriptor of the edge with which the current edge is intersecating
		std::vector<std::pair<point<2>, Edge_desc> > intersections;
		//! Vertex_descriptor of the intersection_edge
		Vertex_desc src_int_edge, tgt_int_edge;
		//! The intersection point between current_edge and intersection_edge
		point<2> intersection_point;
		
};	//intersector

#endif	//HH_INTERSECTOR_CLASS_HH
