/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*! 
	@file   dijkstra_imp.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date   Oct 2016.
	@brief  Solves the single-source shortest-paths problem on a weighted, directed graph with non-negative edge weights.
	@detail
*/

#ifndef HH_DIJKSTRA_IMP_HH
#define HH_DIJKSTRA_IMP_HH

template<typename Graph>
void dijkstra(Graph const &G, 
			  typename boost::graph_traits<Graph>::vertex_descriptor const & v, 
			  std::vector<int> & distances, 
			  std::vector<typename boost::graph_traits<Graph>::vertex_descriptor> & predecessors){
	
	// call Dijkstra algorithm
	boost::dijkstra_shortest_paths(G, v, 
					boost::weight_map(boost::get(&Zunino_edge_property_t::length,G))
					.distance_map(boost::make_iterator_property_map(distances.begin(), boost::get(boost::vertex_index,G)))
					.predecessor_map(boost::make_iterator_property_map(predecessors.begin(), boost::get(boost::vertex_index,G)))
					);
			  
}
#endif
