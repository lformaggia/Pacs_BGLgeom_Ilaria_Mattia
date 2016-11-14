/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*! 
  @file   topological_distance.hpp
  @author Ilaria Speranza & Mattia Tantardini
  @date   Sep 2016.
  @brief  Computes topological distance.
  @detail
 */

#ifndef HH_TOPOLOGICAL_DISTANCE_HH
#define HH_TOPOLOGICAL_DISTANCE_HH

template<typename Graph>
void dijkstra(Graph const& G,
	      typename boost::graph_traits<Graph>::vertex_descriptor s);
	      
#include "topological_distance_imp.hpp"

#endif
