/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*! 
  @file   disjoint_components_imp.hpp
  @author Ilaria Speranza & Mattia Tantardini
  @date   Sep 2016.
  @brief  Identifies if there are fully disconnected subgraphs.
  @detail
*/

#ifndef HH_DISJOINT_COMPONENTS_IMP_HH
#define HH_DISJOINT_COMPONENTS_IMP_HH

template<typename Graph>
std::map<typename boost::graph_traits<Graph>::vertex_descriptor, int> disjoint_components(Graph const& G){

	typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
	typedef typename boost::graph_traits<Graph>::vertex_iterator Vertex_iter;
	typedef typename boost::graph_traits<Graph>::edge_iterator Edge_iter;
	
	typedef std::map<Vertex_desc, int> Rank_map_t;
	typedef std::map<Vertex_desc, Vertex_desc> Parent_map_t;
	
	Rank_map_t rank_map;
	Parent_map_t parent_map;
	
	boost::disjoint_sets<Rank_map_t, Parent_map_t> dsets(rank_map, parent_map);
	
	Vertex_desc u_repr,v_repr; //they contain the respectively the representatives of u and v 
	Vertex_iter v_it, v_end;
	Edge_iter e_it, e_end;
	
	for(std::tie(v_it, v_end) = boost::vertices(G); v_it != v_end; ++v_it){
		dsets.make_set(*v_it); // at the beginning each vertex is a singleton
	}
	
	
	
	
	
	
	
	
	
	
	
	/*
	for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
		u_repr = dsets.find_set(boost::source(*e_it, G));
		v_repr = dsets.find_set(boost::target(*e_it, G));
		
		if(u_repr != v_repr)   // an edge between u and v exists so they have to belong to the same set
			dsets.link(u_repr, v_repr);
	}
	*/
	
	return rank_map;
}

#endif
