/*!	\file maximum_flow_imp.hpp
	\author Ilaria Speranza & Mattia Tantardini
	\date Sep 14, 2016
	\brief Implementations of the functions defined in maximum_flow.hpp
*/

#ifndef HH_MAXIMUM_FLOW_IMP_HH
#define HH_MAXIMUM_FLOW_IMP_HH

#include <map>
#include <vector>
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
*/
template<typename Graph, typename Edge_Descriptor_g>
double maximum_flow	(Graph const &G, 
					typename boost::graph_traits<Graph>::vertex_descriptor s,
					typename boost::graph_traits<Graph>::vertex_descriptor t,
					std::map<Edge_Descriptor_g, double> & out_residual_capacity){		//comunque dovrei restituire una map<Edge_desc, res_cap>
	
	//Desumiamo i vari selettori dal grafo G preesistente:
	typedef boost::adjacency_list	<typename Graph::out_edge_list_selector,
									typename Graph::vertex_list_selector,
									typename Graph::directed_selector,
									boost::no_vertex_bundle,		//vertex_bundle_type<Graph>::type, non ci servono le coordinate per ora!
									edge_prop_max_flow_t>	Flow_Graph;
	
	//copiamo il grafo in Flow_Graph:
	Flow_Graph FG(num_vertices(G));				//crea grafo con stesso numero di vertici di G
	typedef typename boost::graph_traits<Flow_Graph>::edge_descriptor Edge_fg;
	std::map<Edge_fg, Edge_fg> rev_map;
	
	build_flow_graph<Graph, Flow_Graph, Edge_fg> (G, FG, rev_map);
	
	//Ora chiamiamo l'algoritmo:
	double out_max_flow;
	out_max_flow = boost::push_relabel_max_flow(
					FG,
					s,
					t,
					boost::get(&edge_prop_max_flow_t::capacity, FG),	
					boost::get(&edge_prop_max_flow_t::residual_capacity, FG),
					boost::make_assoc_property_map(rev_map),
					boost::get(boost::vertex_index, FG)		//quest'ultimo parametro va messo sempre!!!
					);
					
	
			
	store_residual_capacity<Graph, Flow_Graph, Edge_Descriptor_g> (G, FG, out_residual_capacity);
	
	return out_max_flow;				
} //max_flow;

/*!
	\brief Helper function for maximum_flow
	
	\detail This function build the flow graph associated to the input graph.
			This is because we want not to modify the original Graph passed as input in maximum_flow,
			and because the push_relabelmax_flow algorithm requires such a Graph.
*/
template<typename Graph, typename Flow_Graph, typename Edge_fg>
void build_flow_graph(Graph const& G, Flow_Graph & FG, std::map<Edge_fg, Edge_fg>& rev_map){

	typedef typename boost::graph_traits<Graph>::edge_iterator Edge_g;
	typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_g;
		
	Edge_g e_it, e_end;
	Edge_fg e, rev_e;
	bool inserted;
	Vertex_g u,v;
	for( std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
		u = boost::source(*e_it, G);
		v = boost::target(*e_it, G);
		std::tie(e, inserted) = boost::add_edge(u, v, FG);
		//inizializziamo insieme capacity e residual_capacity allo stesso valore. Residual_cap = cap vuol dire che non c'è flusso lì.
		FG[e].capacity = FG[e].residual_capacity = G[*e_it].capacity;		//questa capacità è il diam che leggiamo dal file di input Zunino.
		FG[e].original_edge = true;		// keeps track of the edges which were in the original graph
		std::tie(rev_e, inserted) = boost::add_edge(v, u, FG);
		FG[rev_e].capacity = FG[rev_e].residual_capacity = 0.0;		//per specifiche dell'algoritmo, vanno messe a zero
		FG[rev_e].original_edge = false;		// this edge is added just for this algorithm
		
		rev_map[e] = rev_e;
		rev_map[rev_e] = e;	
	}

}	//build_flow_graph

/*!	
	\brief Helper function that stores residual capacity on edges after computation of max flow
	
	\detail We use a vector. Next step: using a map<Edge_descriptor, residual_capacity_value>
			This function search in the flow graph which edges have the same sources and target
			as the edges in G, so that we can associate the right residual capacity to the
			right original edge of G. This is because FG is a utility in order to run the 
			push_relabel algorithm and it is destroied after exiting this function.
*/
template <typename Graph, typename Flow_Graph, typename Edge_Descriptor_g>
void store_residual_capacity	(Graph const& G,
								Flow_Graph const & FG,
								std::map<Edge_Descriptor_g, double>& out_residual_capacity){

	typedef typename boost::graph_traits<Flow_Graph>::edge_iterator Edge_iter_fg;
	typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_g;
	typedef typename boost::graph_traits<Flow_Graph>::vertex_descriptor Vertex_fg;
	
	Edge_iter_fg e_it_fg, e_end_fg;
	Edge_Descriptor_g e_g;
	Vertex_fg src_fg, tgt_fg;  // flow_graph vertex descriptor
	Vertex_g src_g, tgt_g;    //graph vertex descriptor
	
	for( std::tie(e_it_fg, e_end_fg) = boost::edges(FG); e_it_fg != e_end_fg; e_it_fg++){
		if(FG[*e_it_fg].original_edge){
			src_fg = source(*e_it_fg, FG);
			tgt_fg = target(*e_it_fg, FG);
			
			src_g = src_fg; // we are sure that they are the same type because we built Flow_graph with the same vec and edge containers as Graph
			tgt_g = tgt_fg;
			
			e_g = boost::edge(src_g, tgt_g, G).first; 
			out_residual_capacity[e_g] = FG[*e_it_fg].residual_capacity;
		}
	}
	
}	//store_residual_capacity


#endif //HH_MAXIMUM_FLOW_IMP_HH
