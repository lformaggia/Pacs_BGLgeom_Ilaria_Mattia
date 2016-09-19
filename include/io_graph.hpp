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
* \date Sep 14, 2016
* \brief Declaration of functions related to input and output of the graph 
*/

#ifndef HH_IO_GRAPH_HH
#define HH_IO_GRAPH_HH

template <typename Graph, typename Point>
void read_zunino_old_format(Graph & G, std::string file_name);

template <typename Graph>
void read_Formaggia_format(Graph & G, std::string file_name);

template <typename Graph, typename Point>
typename boost::graph_traits<Graph>::vertex_descriptor  // return type
vertex_insertion_or_identification(Graph const& G, Point const& P);

template <typename G, typename Point>
void check_for_intersections(std::vector<std::pair<Point, typename boost::graph_traits<Graph>::edge_descriptor> > & v,
							 typename Point const & SRC,
							 typename Point const & TGT, 
							 typename Graph const & G);

template <typename Point>
std::pair<bool, Point> are_intersected (std::pair<Point, Point> line1, std::pair<Point, Point> line2){
	Point p;
	return std::make_pair(true,p);
}

template<typename Graph, typename Point>
void refine_graph(Graph & G, std::vector<std::pair<Point, typename boost::graph_traits<Graph>::edge_descriptor> > const & vect);

#include "io_graph_imp.hpp"	

#endif 
