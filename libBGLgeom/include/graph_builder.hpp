/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file graph_builder.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Utilities to build a graph
*/

#ifndef HH_GRAPH_BUILDER_HH
#define HH_GRAPH_BUILDER_HH

#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <limits>

#include "data_structure.hpp"
#include "generic_edge.hpp"
#include "linear_edge.hpp"
#include "bspline_edge.hpp"

namespace BGLgeom{

/*!
	@breif Helper function to check if an edge is correctly inserted in graph
	@detail It prints an error message on the screen if the insertion of the
			edge failed according to the scenarios described in the reference
			for the function boost::add_edge. See its reference on BGL web page
*/
template <typename Graph>
void check_if_edge_inserted(typename BGLgeom::Edge_desc<Graph> const& e, bool const& inserted){
	if(!inserted){
		std::cerr << "Error while inserting edge!" << std::endl;
		std::cerr << "Failed insertion for edge " << e << "." << std::endl;
		std::cerr << "See documentation of the Boost Graph Library on function boost::add_edge." << std::endl;
	}
}	//check_edge_inserted



//! Giving to source node v all properties through assigning the Source_data_structure
template <typename Graph, typename Vertex_prop>
void give_vertex_properties	(Vertex_prop const& V_prop,
							typename BGLgeom::Vertex_desc<Graph> const& v,
							Graph & G){
	G[v] = V_prop;
}	//give_vertex_properties

//! Giving to edge e all properties through assigning the Edge_data_structure
template <typename Graph, typename Edge_prop>
void give_edge_properties	(Edge_prop const& E_prop,
							typename BGLgeom::Edge_desc<Graph> const& e,
							Graph & G){
	G[e] = E_prop;
}	//give_edge_properties

/*!
	@brief Creates an edge giving the right properties to source and target vertex and to the edge itself
	@detail Topological information, such as source and target vertices, has to be passed as
			standard parameters of the function
	@remark It looses some efficiency due to the creation of an edge descriptor and a bool at
			every call of the function. It also perfom an extra control (not needed in much
			cases, see documentation of boost::add_edge)
*/
template <typename Graph>
BGLgeom::Edge_desc<Graph> 
new_edge(	typename BGLgeom::Vertex_desc<Graph> const& src,
			typename BGLgeom::Vertex_desc<Graph> const& tgt,
			Graph & G){
	
	bool inserted;	
	typename BGLgeom::Edge_desc<Graph> e;
	
	std::tie(e, inserted) = boost::add_edge(src, tgt, G);
	check_if_edge_inserted<Graph>(e, inserted);
	return e;
}	//new_edge (without properties)

//! Creates a new edge and assigns its property values
template <typename Graph, typename Edge_prop>
BGLgeom::Edge_desc<Graph> 
new_edge(	typename BGLgeom::Vertex_desc<Graph> const& src,
			typename BGLgeom::Vertex_desc<Graph> const& tgt,
			Edge_prop const & E_prop,
			Graph & G){
	
	bool inserted;
	typename BGLgeom::Edge_desc<Graph> e;
	
	std::tie(e, inserted) = boost::add_edge(src, tgt, E_prop, G);
	check_if_edge_inserted<Graph>(e, inserted);
	return e;
}

/*
template <typename Graph, unsigned int dim>
BGLgeom::Edge_desc<Graph> 
new_lin_edge(typename boost::graph_traits<Graph>::vertex_descriptor const& src,
			 typename boost::graph_traits<Graph>::vertex_descriptor const& tgt,
			 BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<dim>,dim> & e_dat,
			 Graph & G){
	BGLgeom::Edge_desc<Graph> e = new_edge<Graph, Fracture::Edge_prop>(src,tgt,e_dat,G);	
	G[e].geometry.set_source(G[src].coordinates);
	G[e].geometry.set_target(G[tgt].coordinates);
	
	std::cout<<"New edge created ("<<G[src].coordinates<<";"<<G[tgt].coordinates<<")"<<std::endl;
	std::cout<<"Fracture number: "<<G[e].index<<std::endl;
	
	return e;			 
}
*/

template <typename Graph, unsigned int dim>
BGLgeom::Edge_desc<Graph> 
new_lin_edge(typename boost::graph_traits<Graph>::vertex_descriptor const& src,
			 typename boost::graph_traits<Graph>::vertex_descriptor const& tgt,
			 BGLgeom::Edge_base_property_static<BGLgeom::generic_edge<dim>,dim> const& e_data,
			 Graph & G){
			 
	BGLgeom::Edge_desc<Graph> e(new_edge(src,tgt,e_data,G));	
	// check coherence with the extremes of the parametric curve describing the edge
	if(!(G[src].coordinates == (G[e].geometry)(0)))
		std::cerr<<"Warning: The point inserted as source does not correspond to the paramentric function evaluated in t = 0"<<std::endl;
	if(!(G[tgt].coordinates == (G[e].geometry)(1)))
		std::cerr<<"Warning: The point inserted as target does not correspond to the paramentric function evaluated in t = 1"<<std::endl;
			 
	return e;
}


template <typename Graph>
BGLgeom::Vertex_desc<Graph>
new_vertex(Graph & G){	
	return boost::add_vertex(G);
}	//new_vertex


template <typename Graph, typename Vertex_data_structure>
BGLgeom::Vertex_desc<Graph>
new_vertex(Vertex_data_structure const& v_data,
		   Graph & G, 
		   const bool check_unique = false,
		   const double tol = 20*std::numeric_limits<double>::epsilon()){
	
	if(check_unique){
		const double dist = 0.0;
	
		BGLgeom::Vertex_iter<Graph> v_it,v_end;	
		for(std::tie(v_it,v_end)=boost::vertices(G); v_it != v_end; ++v_it){
			if((v_data.coordinates - G[*v_it].coordinates).norm() < tol){
				std::cout<<"Vertex already existing"<<std::endl;	
				return *v_it;
			}
		}
	}
	// if we arrived here, either check_unique = false or check_unique = true but there is no vertex with the same coordinates
	std::cout<<"New vertex created"<<std::endl;
	return boost::add_vertex(v_data,G);	
}	//new_vertex



/*!
	@brief	Adding a new bspline edge to the graph
	@detail	It add a new edge assuming that the underlying geometry is a bspline one.
			The parameters to set up this geometry are required. Only the geometry is 
			set up, the other properties are left defaulted
	@param src Vertex descriptor for the source
	@param tgt Vertex descriptor fot the target
	@param G The graph
	@param C The vector of control points
*/
template <typename Graph, unsigned int dim>
BGLgeom::Edge_desc<Graph>
new_bspline_edge	(BGLgeom::Vertex_desc<Graph> const& src,
					 BGLgeom::Vertex_desc<Graph> const& tgt,
					 Graph & G,
					 std::vector<BGLgeom::point<dim>> const& C){
	bool inserted;
	BGLgeom::Edge_desc<Graph> e;
	
	std::tie(e, inserted) = boost::add_edge(src, tgt, G);
	check_if_edge_inserted<Graph>(e, inserted);
	
	// Setting up the geometry
	G[e].geometry.set_bspline(C);
	return e;				 
}

}	//BGLgeom

#endif	//HH_GRAPH_BUILDER_HH
