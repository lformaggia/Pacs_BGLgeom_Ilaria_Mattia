/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*! 
	@file   disjoint_components.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date   Sep 2016.
	@brief  Identifies if there are fully disconnected subgraphs..
	@detail
*/

#ifndef HH_DISJOINT_COMPONENTS_HH
#define HH_DISJOINT_COMPONENTS_HH

#include <map>
#include <tuple>
#include <iostream>

#include <boost/graph/graph_traits.hpp>
//#include <boost/property_map/property_map.hpp>
//#include <boost/pending/disjoint_sets.hpp>

#include "our_disjoint_sets.hpp"

//typedef typename our_disjoint_sets<Graph>::Components_map_t Component_map_t;

/*! 
	@detail Given a graph, this function checks whether there are fully disconnected subgraphs, i.e. subgraphs with no edge connecting each other.
		It returns a map which associates each vertex with an integer identifying the subgraph it belongs to.
*/
template<typename Graph>
void disjoint_components(Graph & G); 

#include "disjoint_components_imp.hpp"

#endif
