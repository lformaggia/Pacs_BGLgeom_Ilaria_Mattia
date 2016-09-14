#ifndef HH_MAXIMUM_FLOW_HH
#define HH_MAXIMUM_FLOW_HH

#include <map>

/*
	create a FlowGraph, made of generic_point as vertex_property and edge_property_max_flow as edge_property
	copy in Flow Graph (which has "bidirectional" edges) the properties of Graph and add residual_capacity and rev_graph
	run the algorithm max_flow on the flowGraph
	print in a file residual capacities (or how can we save this data once we delete FlowGraph????)
	output max flow
*/
template<typename Graph>
double maximum_flow	(Graph const &G, 
					typename boost::graph_traits<Graph>::vertex_descriptor s,
					typename boost::graph_traits<Graph>::vertex_descriptor t);


// Funzione di help per quella sopra:
template<typename Graph, typename Flow_Graph, typename Edge_fg>
void build_flow_graph(Graph const& G, Flow_Graph & FG, std::map<Edge_fg, Edge_fg>& rev_map);

#include "maximum_flow_imp.hpp"

#endif //HH_MAXIMUM_FLOW_HH
