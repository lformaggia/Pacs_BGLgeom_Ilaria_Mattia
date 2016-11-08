/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*! 
  @file   compute_euclidean_distance.hpp
  @author Ilaria Speranza & Mattia Tantardini
  @date   Sep 2016.
  @brief  Computes the euclidean distance between two given vertices.
 */

#ifndef HH_COMPUTE_EUCLIDEAN_DISTANCE_HH
#define HH_COMPUTE_EUCLIDEAN_DISTANCE_HH

template<typename Graph>
double compute_euclidean_distance(typename boost::graph_traits<Graph>::vertex_descriptor a, 
				  typename boost::graph_traits<Graph>::vertex_descriptor b,
				  Graph const& G);

#include "compute_euclidean_distance_imp.hpp"

# endif
