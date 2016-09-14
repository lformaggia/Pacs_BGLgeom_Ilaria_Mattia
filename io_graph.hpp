/*!
* \file io_graph.hpp
* \author Ilaria Speranza & Mattia Tantardini
* \date Sep 14, 2016
* \brief Declaration of functions related to input and output of the graph 
*/

#ifndef HH_IO_GRAPH_HH
#define HH_IO_GRAPH_HH

template<typename Graph, typename Point>
void fill_graph(const int src, const int tgt, Graph & G, double diam, double length, Point const & SRC, Point const & TGT);
	
template<typename Graph, typename Point>
void read_zunino_old_format(Graph & G, std::string file_name);

#include "io_graph_imp.hpp"	

#endif 
