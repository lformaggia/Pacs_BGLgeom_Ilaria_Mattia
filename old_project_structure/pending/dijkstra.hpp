/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*! 
	@file   dijkstra.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date   Oct 2016.
	@brief  Solves the single-source shortest-paths problem on a weighted, directed graph with non-negative edge weights.
	@detail This function takes in input the graph, the source vertex and two vectors, one for the distance map and the other for the predecessor map, which will be filled with the results of the algorithm.
*/

#ifndef HH_DIJKSTRA_HH
#define HH_DIJKSTRA_HH

#include "boost/graph/dijkstra_shortest_paths.hpp"
#include "Zunino_edge_property.hpp"
#include "dijkstra_imp.hpp"

template<typename Graph>
void dijkstra(Graph const &G, 
			  typename boost::graph_traits<Graph>::vertex_descriptor const & v, 
			  std::vector<int> & distances, 
			  std::vector<typename boost::graph_traits<Graph>::vertex_descriptor> & predecessors); 

#endif
