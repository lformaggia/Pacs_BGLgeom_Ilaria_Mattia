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
	\fn template<typename Graph>
		double maximum_flow	(Graph const&,
							boost::graph_traits<Graph>::vertex_descriptor,
							boost::graph_traits<Graph>::vertex_descriptor)
	\param G Graph const&;
	\param s Source vertex;
	\param t Sink Vertex;
*/

template<typename Graph>
double maximum_flow	(Graph const &G, 
					typename boost::graph_traits<Graph>::vertex_descriptor s,
					typename boost::graph_traits<Graph>::vertex_descriptor t,
					std::vector<double> & out_residual_capacity){		//comunque dovrei restituire una map<Edge_desc, res_cap>
	
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
					
	//Salviamo le capacità residuali in un vector:
	typedef typename boost::graph_traits<Flow_Graph>::edge_iterator Edge_iter_fg;
	typedef typename boost::graph_traits<Graph>::edge_iterator Edge_iter_g;
	typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_g;
	typedef typename boost::graph_traits<Flow_Graph>::vertex_descriptor Vertex_fg;
	
	Edge_iter_g e_it_g, e_end_g;
	Edge_iter_fg e_it_fg, e_end_fg;
	Vertex_g u, v;
	Vertex_fg uu, vv;
	std::size_t i = 0;
	for( std::tie(e_it_g, e_end_g) = edges(G); e_it_g != e_end_g; e_it_g++){
		u = boost::source(*e_it_g, G);
		v = boost::target(*e_it_g, G);
		for( std::tie(e_it_fg, e_end_fg) = edges(FG); e_it_fg != e_end_fg; e_it_fg++){
			uu = boost::source(*e_it_fg, FG);
			vv = boost::target(*e_it_fg, FG);
			//se origine e target sono uguali, vuol dire che è lo stesso arco.
			if( u == uu && v == vv){
				out_residual_capacity[i] = FG[*e_it_fg].residual_capacity;
				i++;
				break;
			}
		}	
	}
	
	for(std::size_t j = 0; j < out_residual_capacity.size(); j++)
		std::cout << "Capacità residua: " << out_residual_capacity[j] << std::endl;
	
	
	//Le capacità residue che mi interessano sono quelle che hanno capacità maggiore di zero!
	
	/*
	// Stampo le capacità residuali di ogni arco: (comprende sia edge che rev_edge)
	typename boost::graph_traits<Flow_Graph>::edge_iterator e_it, e_end;
	for( std::tie(e_it, e_end) = boost::edges(FG); e_it != e_end; e_it++){
			std::cout << "La capacità residua dell'arco " << *e_it << " è: " << FG[*e_it].residual_capacity << std::endl;
	}
	*/
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
