/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/

/*!
	@file fracture_graph_properties.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Properties that the graph for fractures appliocation will have
	@datail
*/

#ifndef HH_FRACTURE_GRAPH_PROPERTIES_HH
#define HH_FRACTURE_GRAPH_PROPERTIES_HH

#include "data_structure.hpp"
#include "linear_edge.hpp"

namespace Fracture{
/*
	First of all, we define the data structures to be attached to vertices and edges
	on the graph.
	The Vertex_base_property fits the input data, so it is not needed to define a new one.
	This is not the case for the edge properties, where we have to insert four new numerical
	parameters: K_t, K_n, df, source_term
*/
//! The edge data structure
struct Edge_prop : public BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<2>,2>{
	//! Parameter K_t (Permeabilità tangenziale)
	double K_t;
	//! Parameter K_n (Permeabilità normale)
	double K_n;
	//! Apertura
	double df;
	//! Source term
	double source_term;

	
	//! Default constructor
	Edge_prop() :	BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<2>,2>(),
					K_t(0),
					K_n(0),
					df(0),
					source_term(0)
					{};

	//! Full constructor
	Edge_prop	(double const& _K_t,
				 double const& _K_n,
				 double const& _df,				 
				 double const& _source_term) :
							BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<2>,2>(),
							K_t(_K_t),
							K_n(_K_n),
							df(_df),
							source_term(_source_term)
							{};
};	//Edge_prop

//! The vertex data structure
using Vertex_prop = BGLgeom::Vertex_base_property<2>;

}	//Fracture

#endif	//HH_FRACTURE_GRAPH_PROPERTIES_HH
