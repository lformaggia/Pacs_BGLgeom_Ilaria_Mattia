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

template <typename Graph>
typename boost::graph_traits<Graph>::vertex_descriptor  // return type
vertex_insertion_or_identification(Graph const& G, point<2> const& P);

template <typename G>
void check_for_intersections(std::vector<std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> > & v,
							 typename Point const & SRC,
							 typename Point const & TGT, 
							 typename Graph const & G);

template <typename Point>
std::pair<bool, Point> are_intersected (std::pair<Point, Point> line1, std::pair<Point, Point> line2){
	Point p;
	return std::make_pair(true,p);
}

template<typename Graph, bool src_less_than_tgt>
bool compare(std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> pair1,
			 std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> pair2);

template<typename Graph>
void refine_graph(Graph & G, std::vector<std::pair<point<2>, typename boost::graph_traits<Graph>::edge_descriptor> > const & vect, 
                  int frac_number,
                  typename boost::graph_traits<Graph>::vertex_descriptor src,
                  typename boost::graph_traits<Graph>::vertex_descriptor tgt);

#include "io_graph_imp.hpp"	

#endif 
