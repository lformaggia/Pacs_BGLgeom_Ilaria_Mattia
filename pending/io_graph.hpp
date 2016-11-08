/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/

/*!
* \file io_graph.hpp
* \author Ilaria Speranza & Mattia Tantardini
* \date Sept, 2016
* \brief Declaration of functions related to input and output of the graph 
*/

#ifndef HH_IO_GRAPH_HH
#define HH_IO_GRAPH_HH

#include <tuple>
#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <set>
#include <utility>
#include <cmath>
#include <algorithm>

#include "Forma_vertex_property.hpp"
#include "Forma_edge_property.hpp"
#include "generic_point.hpp"

template <typename Graph, typename Reader>
void read_input_file(Graph & G, Reader R, std::string file_name);

/*!
* \brief Reads data about the graph from the input file given by professor Zunino
*
* The funcitions reads from a file where data is written as: \n
* line1: description of file \n
* line2: description of file \n
* from line 3: line_number - source - target - diameter - length - source_coord - target_coord 
* 
*/	
template <typename Graph>
void read_zunino_old_format(Graph & G, std::string file_name);

/*
	@brief Reads data from input file with Formaggia's format.
	
	@detail Until now the input file is:\n
	- first three lines are dummy\n
	- then there are three columns: fracture number, coordinates of origin node, coordinates of target node
*/
template <typename Graph>
void read_Formaggia_format(Graph & G, std::string file_name);

/*!
	@brief Helper function for read_Formaggia_format.
	
	@detail The function checks if the vertex we are trying to insert is already present in the graph,
			(in this case it will be ignored) or if it isn't already present (in this case it will be inserted).
	
	@param G Graph we are constructing
	@param P Point we want to check if is present or not
	@return vertex_descriptor
*/
template <typename Graph>
typename boost::graph_traits<Graph>::vertex_descriptor     //return type
vertex_insertion_or_identification(Graph & G, point<2> const& P);


/*!
	@brief Helper function for read_Formaggia_format
	
	@detail This function checks if two lines (fractures) are intersected. If yes, it creates a vector with 
			all the intersection points already ordered with the right direction (from source to target vertex)
			
	@param vect The vector that will be filled with the intersection points of each new edge
	@param SRC Source vertex of the current edge
	@param TGT Target vertex of the current edge
	@param G Graph
	@return void
*/
template <typename Graph>
void check_for_intersections(std::vector<std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> > & v,
							 point<2> const & SRC,
							 point<2> const & TGT, 
							 Graph const & G);

/*!
	@brief Helper function for check_for intersection
	
	@detail Given a couple of points, this function orders the intersection points according
			to the template parameter src_less_than_tgt. The ordering is needed in order to
			craete the new edges in the right way, preserving the direction of the fractures.
	
	@param pair1 It is the intersection point between the current edge and the edge described 
				by the second component of the pair
	@param pair2 It is the intersection point between the current edge and the edge described 
				by the second component of the pair
	@return bool
*/
template<typename Graph, bool src_less_than_tgt>
bool compare(std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> pair1,
			 std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> pair2);

/*!
	@brief helper function for check_for_intersection
	
	@detail we assume the fractures can only be vertical or horizontal.
			It compute the coordinates of the intersection point, if present.
*/			 
std::pair<bool, point<2> > are_intersected 	(std::pair<point<2>, point<2> > line1,
											 std::pair<point<2>, point<2> > line2);
											 
											 
/*!
	@brief Helper function for read_Formaggia_format
	
	@detail This function breaks old edges to create a refined graph according to the intersection
			points found while inserting the current edge. It preserves the fracture number of each old
			edge while creating the new ones.
			
	@param G Graph
	@param vect The vector of the intersection points
	@param frac_number The fracture number of the current edge
	@param src Vertex descriptor of the source of the current edge
	@param tgt Vertex descriptor of the target of the current edge
	@return void
*/
template<typename Graph>
void refine_graph(Graph & G, 
				  typename std::vector<std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> > const & vect, 
                  int frac_number,
                  typename boost::graph_traits<Graph>::vertex_descriptor src,
                  typename boost::graph_traits<Graph>::vertex_descriptor tgt);

#include "io_graph_imp.hpp"	

#endif 
