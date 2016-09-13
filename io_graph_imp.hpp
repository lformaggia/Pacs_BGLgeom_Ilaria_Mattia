#ifndef HH_IO_GRAPH_IMP_HH
#define HH_IO_GRAPH_IMP_HH

#include<tuple>
#include<boost/graph/adjacency_list.hpp>
// #include<boost/graph/graph_traits.hpp>
#include"edge_property.hpp"

	template<typename Graph, typename Point>
	void initialize_graph(const int src, const int tgt, Graph & G, double diam, double length, Point const & SRC, Point const & TGT){
		
		typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
		
		std::set<int> vertex_set; // it's a set in order to avoid multiple additions of the same vertex
  		bool inserted; //if FALSE the vertex I'm trying to add was already in the set 
  		std::pair<std::set<int>::iterator, bool> set_inserter;
  		std::set<int>::iterator it;
		
		edge_descriptor e;
  		bool edge_inserted;
		
		
    		//! create edge (src, tgt)
    		std::tie(e, edge_inserted) = boost::add_edge(src, tgt, G);
    		G[e].diam = diam;
    		G[e].length = length;
	
		set_inserter = vertex_set.insert(src);

		if(set_inserter.second)
			{
		 		G[src]=SRC;
			}
		set_inserter = vertex_set.insert(tgt);
		if(set_inserter.second)
			{
				G[tgt]=TGT;
			}
	}
#endif
