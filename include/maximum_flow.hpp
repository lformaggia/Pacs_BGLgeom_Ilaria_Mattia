/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
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
#include <tuple>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>		//per push_relabel
#include <boost/property_map/property_map.hpp>
#include <boost/graph/properties.hpp>					//per la definizione di vertex_index richiesta da push_relabel

#include "generic_point.hpp"
#include "edge_property_max_flow.hpp"

/*!
	\brief It runs push_relabel_max_flow algorithm on graph G
	
	\detail This function find the maximum flow that can flow from node s to node t.
	
	@param G Graph
	@param s Source vertex chosen for the maximum flow problem
	@param t Target vertex chosen for the maximum flow problem
	@param out_residual_capacity Map that stores the residual capacity left in each edge
*/
template<typename Graph, typename Edge_Descriptor_g>
double maximum_flow	(Graph const & G, 
					typename boost::graph_traits<Graph>::vertex_descriptor s,
					typename boost::graph_traits<Graph>::vertex_descriptor t,
					std::map<Edge_Descriptor_g, double> & out_residual_capacity);

/*!
	\brief Helper function for maximum_flow
	
	\detail This function build the flow graph associated to the input graph.
			This is because we want not to modify the original Graph passed as input in maximum_flow,
			and because the push_relabelmax_flow algorithm requires such a Graph.
	@param G Graph
	@param FG Flow graph that will be built inside the function. We need to build a new graph with a special
				structure in order to accomplish the requirments of boost::push_relabel_max_flow
	@param rev_map Map that stores the reverse edge for each edge in the graph. It is needed to build Flow Graph.
*/
template<typename Graph, typename Flow_Graph, typename Edge_fg>
void build_flow_graph	(Graph const& G,
						Flow_Graph & FG,
						std::map<Edge_fg, Edge_fg>& rev_map);

/*!	
	\brief Helper function that stores residual capacity on edges after computation of max flow
	
	\detail We use a vector. Next step: using a map<Edge_descriptor, residual_capacity_value>
			This function search in the flow graph which edges have the same sources and target
			as the edges in G, so that we can associate the right residual capacity to the
			right original edge of G. This is because FG is a utility in order to run the 
			push_relabel algorithm and it is destroied after exiting this function.
	
	@param G Graph
	@param FG Flow Graph
	@param out_residual_capacity Map that stores the residual capacity left in each edge
*/
template <typename Graph, typename Flow_Graph, typename Edge_Descriptor_g>
void store_residual_capacity	(Graph const& G,
								Flow_Graph const & FG,
								std::map<Edge_Descriptor_g, double> & out_residual_capacity);

#include "maximum_flow_imp.hpp"

#endif //HH_MAXIMUM_FLOW_HH
