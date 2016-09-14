#ifndef HH_MAXIMUM_FLOW_IMP_HH
#define HH_MAXIMUM_FLOW_IMP_HH

#include <map>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>		//per push_relabel
#include <boost/property_map/property_map.hpp>
#include <boost/graph/properties.hpp>					//per la definizione di vertex_index richiesta da push_relabel

#include "generic_point.hpp"
#include "edge_property_max_flow.hpp"


template<typename Graph>
double maximum_flow	(Graph const &G, 
					typename boost::graph_traits<Graph>::vertex_descriptor s,
					typename boost::graph_traits<Graph>::vertex_descriptor t){
	
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
					boost::get(boost::vertex_index, FG)
					);		//manca l'ultimo parametro con la map dei vertex_index, che è default
	
	return out_max_flow;
				
} //max_flow;


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
		std::tie(rev_e, inserted) = boost::add_edge(v, u, FG);
		FG[rev_e].capacity = FG[rev_e].residual_capacity = 0.0;		//per specifiche dell'algoritmo, vanno messe a zero
		rev_map[e] = rev_e;
		rev_map[rev_e] = e;	
	}

}	//build_flow_graph


#endif //HH_MAXIMUM_FLOW_IMP_HH
