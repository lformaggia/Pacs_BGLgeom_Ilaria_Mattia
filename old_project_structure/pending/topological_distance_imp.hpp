/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*! 
  @file   topological_distance_imp.hpp
  @author Ilaria Speranza & Mattia Tantardini
  @date   Sep 2016.
  @brief  Computes topological distance.
  @detail
 */

#ifndef HH_TOPOLOGICAL_DISTANCE_IMP_HH
#define HH_TOPOLOGICAL_DISTANCE_IMP_HH

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include "edge_property.hpp"

template<typename Graph>
double dijkstra(Graph const& G,
	      typename boost::graph_traits<Graph>::vertex_descriptor s,
	      typename boost::graph_traits<Graph>::vertex_descriptor t, 
	      std::vector<typename boost::graph_traits<Graph>::vertex_descriptor> & v){ // il numero di edges nel path si deduceranno dalla lunghezza di v
	    
	    
	 typedef typename boost::graph_traits<Graph> Traits;
	 typedef typename Traits::vertex_descriptor vertex_descriptor;
	 
	 std::vector<vertex_descriptor> predecessors(boost::num_vertices(G));
	 std::vector<double> distances(boost::num_vertices(G));
	 
	 boost::dijkstra_shortest_paths(G, s, 
	 							   predecessor_map(boost::make_iterator_property_map(predecessors.begin(), boost::get(boost::vertex_index,G))),
	 							   distance_map(boost::make_iterator_property_map(distances.begin(), boost::get(boost::vertex_index,G))),
	 							   boost::weight_map(boost::get(&edge_prop_t::length, G)));
	  
}
#endif
