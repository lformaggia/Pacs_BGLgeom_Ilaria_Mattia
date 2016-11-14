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

#ifndef HH_COMPUTE_EUCLIDEAN_DISTANCE_IMP_HH
#define HH_COMPUTE_EUCLIDEAN_DISTANCE_IMP_HH

#include <cmath>
#include "generic_point.hpp"

template<typename Graph>
double compute_euclidean_distance(typename boost::graph_traits<Graph>::vertex_descriptor a, 
				  				  typename boost::graph_traits<Graph>::vertex_descriptor b,
				  				  Graph const& G){
				  				  
	const unsigned int dim = G[a].get_dim(); // dim is the dimension of the points we are considering
	double distance = 0.0; //it's the return value and it will contain the euclidean distance
	for(int i = 0; i < dim; ++i){
		distance += (G[a].get(i) - G[b].get(i)) * (G[a].get(i) - G[b].get(i));
	}
	
	return sqrt(distance);  
}

# endif
