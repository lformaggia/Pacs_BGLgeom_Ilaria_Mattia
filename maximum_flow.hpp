/*!
	\file maximum_flow.hpp
	\author Ilaria Speranza & Mattia Tantardini
	\date Sep 14, 2016
	\brief Header file for managing maximum_flow algorithm from BGL

	\detail	
*/

#ifndef HH_MAXIMUM_FLOW_HH
#define HH_MAXIMUM_FLOW_HH

#include <map>
#include <vector>

/*!
	\brief It runs push_relabel_max_flow algorithm on graph G
	
	\detail This function find the maximum flow that can flow from node s to node t.
*/
template<typename Graph>
double maximum_flow	(Graph const & G, 
					typename boost::graph_traits<Graph>::vertex_descriptor s,
					typename boost::graph_traits<Graph>::vertex_descriptor t,
					std::vector<double> & out_residual_capacity);

/*!
	\brief Helper function for maximum_flow
	
	\detail This function build the flow graph associated to the input graph.
			This is because we want not to modify the original Graph passed as input in maximum_flow,
			and because the push_relabelmax_flow algorithm requires such a Graph.
*/
template<typename Graph, typename Flow_Graph, typename Edge_fg>
void build_flow_graph(Graph const& G, Flow_Graph & FG, std::map<Edge_fg, Edge_fg>& rev_map);

#include "maximum_flow_imp.hpp"

#endif //HH_MAXIMUM_FLOW_HH
