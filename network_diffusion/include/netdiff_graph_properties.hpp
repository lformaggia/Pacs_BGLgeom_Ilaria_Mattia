/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/

/*!
	@file netdiff_graph_properties.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Properties that the graph for diffusion on network appliocation will have
	@datail All the data structures have to be provided with at least 
			the default constructor and a constructor, and the copy constructor,
			assignment operator and move constructor if needed (the default ones
			may be enough)
*/

#ifndef HH_NETDIFF_GRAPH_PROPERTIES_HH
#define HH_NETDIFF_GRAPH_PROPERTIES_HH

#include "data_structure.hpp"
#include "linear_edge.hpp"

namespace NetDiff{

//! Properties that vertices will have
using Vertex_prop = BGLgeom::Vertex_base_property<3>;

//! Properties that vertices will have
struct Edge_prop : public BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<3>,3>{
	//! Diameter
	double diam;
	
	//! Default constructor
	Edge_prop() : 	BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<3>,3>(),
					diam() {};
	//! Constructor
	Edge_prop(double const& _diam) : BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<3>,3>(),
									diam(_diam) {};
	//! Explicit declaration of default assignment operator
	Edge_prop & operator=(Edge_prop const&) = default;
	
};	//Edge_prop 

/*!
	@brief Topological properties of the graph
	@detail The input file gives us explicit infos about the structure of the
			graph. We can use this infos to index the vertex descriptors like
			the given data. Moreover, since the label of the vertices in the
			input file are integers, we can assume to use a vecS,vecS graph
			in order not to templatize the vertex descriptors in this struct.
*/
struct Topological_prop{
	//! Vertex_descriptor for the source
	unsigned int src;
	//! Vertex_descriptor for the target
	unsigned int tgt;
	
	//! Default constructor
	Topological_prop() : src(), tgt() {};
	
	//! Constructor
	Topological_prop(unsigned int const& _src, unsigned int const& _tgt) : src(_src), tgt(_tgt) {};
};	//Topological_prop

}	//NetDiff

#endif	//HH_NETDIFF_GRAPH_PROPERTIES_HH
