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
  
	@detail Note: the algorithm is not very well. It can be bettered, in particular
			when changing all the labels of a big component that we have to move
			in another small component. Better to move the smaller into the bigger.
*/

#ifndef HH_DISJOINT_COMPONENTS_IMP_HH
#define HH_DISJOINT_COMPONENTS_IMP_HH

template<typename Graph>
std::map<typename boost::graph_traits<Graph>::vertex_descriptor, int> disjoint_components(Graph const& G){

	typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
	typedef typename boost::graph_traits<Graph>::edge_iterator Edge_iter;
	
	typedef typename our_disjoint_sets<Graph>::Label_mapped_t Label_t;
	typedef typename our_disjoint_sets<Graph>::Components_mapped_t::iterator Comp_iter;
	/*
	typedef std::map<Vertex_desc, int> Rank_map_t;
	typedef std::map<Vertex_desc, Vertex_desc> Parent_map_t;
	
	Rank_map_t rank_map;
	Parent_map_t parent_map;
	*/
	//boost::disjoint_sets<Rank_map_t, Parent_map_t> dsets(rank_map, parent_map);
	
	our_disjoint_sets<Graph> dsets(G);
	
	dsets.make_label_map();
	
	Edge_iter e_it, e_end;
	Vertex_desc src, tgt;
	bool different_labels;
	
	
	Label_t src_label;
	
	for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
		src = boost::source(*e_it, G);
		tgt = boost::target(*e_it, G);
		src_label = dsets.get_label(src);		//da usare per non richiamare ogni volta sempre questo metodo
		//Check if aource and target already belong to the same component
		if( src_label != dsests.get_label(tgt) )
			different_labels = true;		//they are in different components
		else
			different_labels = false;	//they are in the same component	//continue?
			
		if(different_labels){
			if( dsets.is_present_component(src_label) ){	//se quella componente c'è già
				Label_t tgt_label = dsets.get_label(tgt);	//per chiarezza e velocità se il grafo è grande, chiamo get_label 1 sola volta
				if( dsets.is_present_component(tgt_label) ){		//se c'è anche la componente di tgt, aggiungo tutti i nodi della componente di tgt nella componenete di src (compreso tgt, che è già dentro la componente)
					Comp_iter comp_it, comp_end;
					for(std::tie(comp_it, comp_end) = dsets.get_iterator(tgt_label);
						comp_it != comp_end;
						++comp_it){	//scorro la lista
							dsets.insert_vertex_in_component(*comp_it, src_label);
							dsets.set_label(*comp_it, src_label);				
					}	//for
					//cancello la componente di tgt, dopo aver spostato tutto nella componente di src:
					dsets.erase_component(tgt_label);
				} else {	//tgt è per conto suo, lo metto nella componente di src
					dsets.insert_vertex_in_component(tgt, src_label);
					dsets.set_label(tgt, src_label);
				}	//else
			} else {		//se quella componente non c'è ancora
				dsets.new_component(src_label);
				dsets.insert_vertex_in_component(src, src_label);
				dsets.insert_vertex_in_component(tgt, src_label);	//metto tgt nella stessa componente di src, sempre
				dsets.set_label(tgt, src_label);						//aggiorno l'etichetta
			}	//else
		}	//if		
	}	//for
	
	/*
	
	for(std::tie(v_it, v_end) = boost::vertices(G); v_it != v_end; ++v_it){
		dsets.make_set(*v_it); // at the beginning each vertex is a singleton
	}
	
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
