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
#include <boost/graph/graph_traits.hpp>

#include "data_structure.hpp"
#include "generic_edge.hpp"
#include "linear_edge.hpp"
#include "fracture_graph_properties.hpp"

/*!
	@breif Helper function to check if an edge is correctly inserted in graph
	@detail It prints an error message on the screen if the insertion of the
			edge failed according to the scenarios described in the reference
			for the function boost::add_edge. See its reference on BGL web page
*/
template <typename Graph>
void check_if_edge_inserted(typename boost::graph_traits<Graph>::edge_descriptor e, bool inserted){
	if(!inserted){
		std::cerr << "Error while inserting edge!" << std::endl;
		std::cerr << "Failed insertion for edge " << e << "." << std::endl;
		std::cerr << "See documentation of the Boost Graph Library on function boost::add_edge." << std::endl;
	}
}	//check_edge_inserted



//! Giving to source node v all properties through assigning the Source_data_structure
template <typename Graph, typename Vertex_data_structure>
void give_vertex_properties	(Vertex_data_structure const& D,
							typename boost::graph_traits<Graph>::vertex_descriptor const& v,
							Graph & G){
	G[v] = D;
}	//give_vertex_properties

//! Giving to edge e all properties through assigning the Edge_data_structure
template <typename Graph, typename Edge_data_structure>
void give_edge_properties	(Edge_data_structure const& D,
							typename boost::graph_traits<Graph>::edge_descriptor const& e,
							Graph & G){
	G[e] = D;
}	//give_edge_properties

/*!
	@brief Creates an edge giving the right properties to source and target vertex and to the edge itself
	@detail Topological information, such as source and target vertices, has to be passed as
			standard parameters of the function
	@remark It looses some efficiency due to the creation of an edge iterator and a bool at
			every call of the function. It also perfom an extra control (not needed in much
			cases, see documentation of boost::add_edge)
*/
template <typename Graph>
BGLgeom::Edge_desc<Graph> 
new_edge(	typename boost::graph_traits<Graph>::vertex_descriptor const& src,
			typename boost::graph_traits<Graph>::vertex_descriptor const& tgt,
			Graph & G){
	
	bool inserted;
	
	typename boost::graph_traits<Graph>::edge_descriptor e;
	std::tie(e, inserted) = boost::add_edge(src, tgt, G);
	check_if_edge_inserted<Graph>(e, inserted);
return e;
}	//new_edge (without properties)


template <typename Graph, typename Edge_data_structure>
BGLgeom::Edge_desc<Graph> 
new_edge(	typename boost::graph_traits<Graph>::vertex_descriptor const& src,
			typename boost::graph_traits<Graph>::vertex_descriptor const& tgt,
			Edge_data_structure const & e_data,
			Graph & G){
	
	bool inserted;
	typename boost::graph_traits<Graph>::edge_descriptor e;
	
	std::tie(e, inserted) = boost::add_edge(src, tgt, e_data, G);
	check_if_edge_inserted<Graph>(e, inserted);

return e;
}	//create_edge

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
	@brief This function refines a graph creating a new vertex where the edges intersect
	@detail Given two edges intersecting the intersection point, the function creates new
			vertices in the graph corresponding to the intersection	point. The old edges
			are broken and they are rebuilt, respecting their directions, connecting old 
			extreme vertices to the new one just created.
	@remark This fucntion refers only to a two-dimensional setting.
*/
template <typename Graph>
void refine_graph	(Graph & G,
					BGLgeom::point<2> const& I_point,		//there is only one intersection point between two edges!
					typename boost::graph_traits<Graph>::edge_descriptor & edge1,
					typename boost::graph_traits<Graph>::edge_descriptor & edge2){
	
	using Vertex_desc = typename boost::graph_traits<Graph>::vertex_descriptor;
	using Edge_desc = typename boost::graph_traits<Graph>::edge_descriptor;
	
	Vertex_desc src1, tgt1, src2, tgt2, intersection_new;
	src1 = boost::source(edge1, G);
	tgt1 = boost::target(edge1, G);
	src2 = boost::source(edge2, G);
	tgt2 = boost::target(edge2, G);
	
	intersection_new = boost::add_vertex(G); 
	//dai properietà! Magari meglio appoggiarsi a template e funzione give_vertex_properties che ho sopra.
	G[intersection_new].coordinates = I_point;
	G[intersection_new].BC.type = BGLgeom::BC_type::NONE;
	G[intersection_new].BC.value = 0.0;
	
	Edge_desc e11, e12, e21, e22;
	bool inserted;
	std::tie(e11,inserted) = boost::add_edge(src1, intersection_new, G);
	check_if_edge_inserted<Graph>(e11, inserted);
	//properietà edge! e riscalamento parametrizzazione
	std::tie(e12,inserted) = boost::add_edge(intersection_new, tgt1, G);
	check_if_edge_inserted<Graph>(e12, inserted);
	//idem
	std::tie(e21,inserted) = boost::add_edge(src2, intersection_new, G);
	check_if_edge_inserted<Graph>(e21, inserted);
	//idem
	std::tie(e22,inserted) = boost::add_edge(intersection_new, tgt2, G);
	check_if_edge_inserted<Graph>(e22, inserted);
	//idem
	
	boost::remove_edge(edge1, G);
	boost::remove_edge(edge2, G);
}	//refine_graph

#endif	//HH_GRAPH_BUILDER_HH
