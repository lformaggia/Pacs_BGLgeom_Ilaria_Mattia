/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	graph_builder.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Utilities to build a graph and adapters to some BGL functions
	
	In this file we define some functions needed to build and manage the 
	graph, and to acces and iterate over its properties.
	We provide: \n
	- an alias for edge, vertex descriptors, edge, vertex iterators; \n
	- the redefinition of some useful and frequently used BGL function: 
		getting source and target of an edge, getting the edge and vertex 
		iterators for that graph; \n
	- the redefinition of the BGL functions concerning the creation of 
		vertices and edge; \n
	- some other functions to add edges to the graph knowing a priori 
		the geometry of an edge, in order to automatize some setting 
		procedure. This functions in general also perform some additional 
		checks on the geometric properties.
*/

#ifndef HH_GRAPH_BUILDER_HH
#define HH_GRAPH_BUILDER_HH

#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <boost/graph/adjacency_list.hpp>

#include "data_structure.hpp"
#include "generic_geometry.hpp"
#include "linear_geometry.hpp"
#include "bspline_geometry.hpp"

namespace BGLgeom{

/*!
	@defgroup	Accessi_Graph_items	Aliases type for BGL
	
	We provide alias for vertex and edge descriptors, vertex
	and edge iterators.
	@{
*/
//! Vertex descriptor alias type
template <typename Graph>
using Vertex_desc = typename boost::graph_traits<Graph>::vertex_descriptor;

//! Vertex iterator alias type
template <typename Graph>
using Vertex_iter = typename boost::graph_traits<Graph>::vertex_iterator;

//! Edge descriptor alias type
template <typename Graph>
using Edge_desc = typename boost::graph_traits<Graph>::edge_descriptor;

//! Edge iterator alias type
template <typename Graph>
using Edge_iter = typename boost::graph_traits<Graph>::edge_iterator;
/*! @} */


/*!
	@defgroup	Function aliases	Some useful aliases for function from BGL, used
									very often to create a graph and iterate over it

	They are Utilities to hidden boost::edges(G) and to allow to use only 
	one namespace (BGLgeom)
	@{
*/
/*!
	@brief	Gets the edge iterators to the firts and to the last edge in the graph
	
	@param G The graph
	@return A std::pair containing: \n
			- first: the edge iterator pointing to the first edge in the graph \n
			- second: the edge iterator pointing to the last edge in the graph
*/
template <typename Graph>
std::pair< BGLgeom::Edge_iter<Graph>, BGLgeom::Edge_iter<Graph> >
edges(Graph const& G) {
	return boost::edges(G);
}	//edges

/*!
	@brief Gets the vertex iterators to the first and to the last vertex in the graph
	
	@param G The graph
	@return A std::pair containing: \n
			- first: the vertex iterator pointing to the first edge in the graph \n
			- second: the vertex iterator pointing to the last edge in the graph
*/
template <typename Graph>
std::pair< BGLgeom::Vertex_iter<Graph>, BGLgeom::Vertex_iter<Graph> >
vertices(Graph const& G) {
	return boost::vertices(G);
}	//vertices

/*!
	@brief	Gets the vertex descriptor of the source of the given edge
	
	@param e The edge descriptor of the edge
	@param G the graph
	@return The vertex descriptor of the source of edge e
*/
template <typename Graph>
BGLgeom::Vertex_desc<Graph>
source(BGLgeom::Edge_desc<Graph> const& e, Graph const& G){
	return boost::source(e, G);
}

/*!
	@brief	Gets the vertex descriptor of the target of the given edge
	
	@param e The edge descriptor of the edge
	@param G the graph
	@return The vertex descriptor of the target of edge e
*/
template <typename Graph>
BGLgeom::Vertex_desc<Graph>
target(BGLgeom::Edge_desc<Graph> const& e, Graph const& G){
	return boost::target(e, G);
}
/*! @} */


/*!
	@breif Helper function to check if an edge is correctly inserted in graph
	
	It prints an error message on the screen if the insertion of the
	edge failed according to the scenarios described in the reference
	for the function boost::add_edge: 
	"For graphs that do not allow parallel edges, if the edge is already 
	in the graph then a duplicate will not be added and the bool flag 
	will be false. When the flag is false, the returned edge descriptor 
	points to the already existing edge." 
	(quote from http://www.boost.org/doc/libs/1_37_0/libs/graph/doc/adjacency_list.html)
*/
template <typename Graph>
void check_if_edge_inserted(BGLgeom::Edge_desc<Graph> const& e, bool const& inserted){
	if(!inserted){
		std::cerr << "Error while inserting edge!" << std::endl;
		std::cerr << "Failed insertion for edge " << e << "." << std::endl;
		std::cerr << "See documentation of the Boost Graph Library on function boost::add_edge." << std::endl;
	}
}	//check_edge_inserted

/*!
	@brief	Creates a new vertex in the graph
	
	The new vertex has all the properties with a defaulted value
	
	@remark	Please, remember that this function leaves all the vertex 
			properties to its default value. They have to be set in 
			a following moment
	
	@param G The graph where to insert the new vertex
	@return The vertex descriptor of the new vertex
*/
template <typename Graph>
BGLgeom::Vertex_desc<Graph>
new_vertex(Graph & G){	
	return boost::add_vertex(G);
}	//new_vertex

/*!
	@brief	Creates a new vertex in the graph with the given properties

	@param v_prop The properties to be assigned to the new vertex
	@param G The graph where to insert the new vertex
	@param check_unique Boolean: set true if you want to check wheter the
			coordinates of the vertex you want to insert are already present
			in the graph (i.e. if there is or no another vertex with the same 
			coordinates in the graph)
	@return The vertex descriptor of the new vertex
*/
template <typename Graph, typename Vertex_prop>
BGLgeom::Vertex_desc<Graph>
new_vertex(Vertex_prop const& v_prop,
		   Graph & G, 
		   const bool check_unique = false){
	
	if(check_unique){	
		BGLgeom::Vertex_iter<Graph> v_it,v_end;	
		for(std::tie(v_it,v_end)=boost::vertices(G); v_it != v_end; ++v_it){
			if(v_prop.coordinates == G[*v_it].coordinates){
				#ifndef NDEBUG
					std::cout << "Vertex already existing" << std::endl;	
				#endif
				return *v_it;
			}
		}
	}
	// if we arrived here, either check_unique = false or check_unique = true but there is no vertex with the same coordinates
	#ifndef NDEBUG
		std::cout << "New vertex created" << std::endl;
	#endif
	return boost::add_vertex(v_prop,G);	
}	//new_vertex

/*!
	@brief	Creates a new edge in the graph
	
	The edge properties are not assigned, so are left with a default value.
	This function checks for the right insertion of the edge
	
	@remark	Please, remember that this function leaves all the edge 
			properties to its default value. They have to be set in 
			a following moment
	
	@param src The vertex descriptor of the source
	@param tgt The vertex descriptor of the target
	@param G The graph where to insert the new edge
	@return The edge descriptor of the new edge inserted
*/
template <typename Graph>
BGLgeom::Edge_desc<Graph> 
new_edge(	BGLgeom::Vertex_desc<Graph> const& src,
			BGLgeom::Vertex_desc<Graph> const& tgt,
			Graph & G){
	
	bool inserted;	
	BGLgeom::Edge_desc<Graph> e;	
	std::tie(e, inserted) = boost::add_edge(src, tgt, G);
	check_if_edge_inserted<Graph>(e, inserted);
	#ifndef NDEBUG
		/*
		We do not print infos about geometry (<< G[e].geometry) since 
		its source and target may not be defined yet, especially in 
		the linear case	
		*/
		std::cout << "New edge created" << std::endl;
	#endif
	return e;
}	//new_edge (without properties)

/*!
	@brief	Creates a new edge in the graph assigning it the given properties
	
	This function checks for the right insertion of the edge
	
	@param src The vertex descriptor of the source
	@param tgt The vertex descriptor of the target
	@param e_prop The edge properties to be assigned to the edge
	@param G The graph where to insert the new edge
	@return The edge descriptor of the new edge inserted
*/
template <typename Graph, typename Edge_prop>
BGLgeom::Edge_desc<Graph> 
new_edge(	BGLgeom::Vertex_desc<Graph> const& src,
			BGLgeom::Vertex_desc<Graph> const& tgt,
			Edge_prop const & E_prop,
			Graph & G){
	
	bool inserted;
	BGLgeom::Edge_desc<Graph> e;	
	std::tie(e, inserted) = boost::add_edge(src, tgt, E_prop, G);
	check_if_edge_inserted<Graph>(e, inserted);
	#ifndef NDEBUG
		std::cout << "New edge created: " << G[e].geometry << std::endl;
	#endif
	return e;
}	//new_edge (with properties)

/*!
	@brief	Adds a new linear edge to the graph
	
	@remark	Use this only when you set "linear_geometry<dim>" as template parameter of the
			Edge_base_property
			
	It adds a new edge assuming that the underlying geometry is the linear one.
	It takes care of setting up the geometry.
	
	@note 	It performs a check on the insertion of the edge
	@pre	This version of the function assumes that the coordinates of the vertices
			are already defined in the vertex properties of the vertices
	@pre	Obviously the BGLgeom::Edge_base_property struct or derived is required 
			as edge property of the graph
	@remark	Please, remember that this function leaves all the edge 
			properties (but the geometry, that is correctly set up) 
			to its default value. They have to be set in a following 
			moment
			
	@param src Vertex descriptor for the source
	@param tgt Vertex descriptor fot the target
	@param G The graph where to insert the new edge
	@return The edge descriptor of the new edge
*/
template <typename Graph>
BGLgeom::Edge_desc<Graph>
new_linear_edge	(BGLgeom::Vertex_desc<Graph> const& src,
				 BGLgeom::Vertex_desc<Graph> const& tgt,
				 Graph & G){
	bool inserted;
	BGLgeom::Edge_desc<Graph> e;
	std::tie(e, inserted) = boost::add_edge(src, tgt, G);
	check_if_edge_inserted<Graph>(e, inserted);
	
	// Setting up the geometry
	G[e].geometry.set_source(G[src].coordinates);
	G[e].geometry.set_target(G[tgt].coordinates);
	#ifndef NDEBUG
		std::cout << "New edge created: " << G[e].geometry << std::endl;
	#endif
	return e;
}	//new_linear_edge

/*!
	@brief	Adding a new generic edge to the graph
	
	@remark	Use this only when you set "generic_geometry<dim>" as template parameter of the
			Edge_base_property
			
	It adds a new edge assuming that the underlying geometry is the generic one.
	It takes care of setting up the geometry, and so requires some additional
	parameters to do this.
	
	@note 	It performs a check on the insertion of the edge
	@note	It checks if the ends of the parameterization (t=0 and t=1) conincide with
			the coordinates of source and vertex passed in the vertex descriptors. If not,
			it displays a warning message in the screen
	@pre	Obviously the BGLgeom::Edge_base_property struct or derived is required 
			as edge property of the graph
	@remark	Please, remember that this function leaves all the edge 
			properties (but the geometry, that is correctly set up) 
			to its default value. They have to be set in a following 
			moment
			
	@param src Vertex descriptor for the source
	@param tgt Vertex descriptor fot the target
	@param G The graph where to insert the new edge
	@param _fun Parametrized function describing the curve of the edge
	@param _first_der Parametrized function describing the first derivative of the curve
	@param _second_der Parametrized function describing the second derivative of the curve
	@return The edge descriptor of the new edge
*/
template <typename Graph, unsigned int dim>
BGLgeom::Edge_desc<Graph>
new_generic_edge(BGLgeom::Vertex_desc<Graph> const& src,
				 BGLgeom::Vertex_desc<Graph> const& tgt,
				 Graph & G,
				 std::function<BGLgeom::point<dim>(double)> const& _fun,
				 std::function<BGLgeom::point<dim>(double)> const& _first_der,
				 std::function<BGLgeom::point<dim>(double)> const& _second_der){
	bool inserted;
	BGLgeom::Edge_desc<Graph> e;
	std::tie(e, inserted) = boost::add_edge(src, tgt, G);
	check_if_edge_inserted<Graph>(e, inserted);
	
	if(G[src].coordinates != _fun(0))
		std::cerr << "WARNING: source coordinates " << G[src].coordinates 
				<< " do not coincide with the parametrized function evaluated in t=0" << std::endl;
	if(G[tgt].coordinates != _fun(1))
		std::cerr << "WARNING: target coordinates " << G[tgt].coordinates
				<< " do not coincide with the parametrized function evaluated in t=1" << std::endl;
	// Setting up the geometry
	G[e].geometry.set_function(_fun);
	G[e].geometry.set_first_der(_first_der);
	G[e].geometry.set_second_der(_second_der);
	#ifndef NDEBUG
		std::cout << "New edge created: " << G[e].geometry << std::endl;
	#endif
	return e;	
}	//new_generic_edge

/*!
	@brief	Adding a new bspline edge to the graph
	
	@remark	Use this only when you set "bspline_geometry<dim,deg>" as template parameter of the
			Edge_base_property
	
	It adds a new edge assuming that the underlying geometry is the bspline one.
	It takes care of setting up the geometry, so some additional parameters are
	required.
	@note 	It performs a check on the insertion of the edge
	@note	It checks if the ends of the parameterization (t=0 and t=1) conincide with
			the coordinates of source and vertex passed in the vertex descriptors. If not,
			it displays a warning message in the screen
	@pre	Obviously the BGLgeom::Edge_base_property struct or derived is required 
			as edge property of the graph
	@remark	Please, remember that this function leaves all the edge 
			properties (but the geometry, that is correctly set up) 
			to its default value. They have to be set in a following 
			moment
			
	@param src Vertex descriptor for the source
	@param tgt Vertex descriptor fot the target
	@param G The graph where to insert the new edge
	@param C The vector of control points
	@return The edge descriptor of the new edge
*/
template <typename Graph, unsigned int dim>
BGLgeom::Edge_desc<Graph>
new_bspline_edge	(BGLgeom::Vertex_desc<Graph> const& src,
					 BGLgeom::Vertex_desc<Graph> const& tgt,
					 std::vector<BGLgeom::point<dim>> const& C,
					 Graph & G){
	bool inserted;
	BGLgeom::Edge_desc<Graph> e;	
	std::tie(e, inserted) = boost::add_edge(src, tgt, G);
	check_if_edge_inserted<Graph>(e, inserted);
	
	// Setting up the geometry
	G[e].geometry.set_bspline(C);

	if(G[src].coordinates != C.front())
		std::cerr << "WARNING: source coordinates " << G[src].coordinates 
				<< " do not coincide with the parametrized function evaluated in t=0" << std::endl;
	if(G[tgt].coordinates != C.back())
		std::cerr << "WARNING: target coordinates " << G[tgt].coordinates
				<< " do not coincide with the parametrized function evaluated in t=1" << std::endl;

	#ifndef NDEBUG
		std::cout << "New edge created: " << G[e].geometry << std::endl;
	#endif
	return e;				 
}	//new_bspline_edge

/*!
	@brief	Adding a new bspline edge to the graph
	
	@remark	Use this only when you set "bspline_geometry<dim,deg>" as template parameter of the
			Edge_base_property
	It adds a new edge assuming that the underlying geometry is the bspline one.
	It takes care of setting up the geometry, so some additional parameters are
	required.
	@note 	It performs a check on the insertion of the edge
	@note	It checks if the ends of the parameterization (t=0 and t=1) conincide with
			the coordinates of source and vertex passed in the vertex descriptors. If not,
			it displays a warning message in the screen
	@pre	Obviously the BGLgeom::Edge_base_property struct or derived is required 
			as edge property of the graph
	@remark	Please, remember that this function leaves all the edge 
			properties (but the geometry, that is correctly set up) 
			to its default value. They have to be set in a following 
			moment
			
	@param src Vertex descriptor for the source
	@param tgt Vertex descriptor fot the target
	@param G The graph where to insert the new edge
	@param C The vector of control points
	@param k The knot vector for the bspline
	@return The edge descriptor of the new edge
*/
template <typename Graph, unsigned int dim>
BGLgeom::Edge_desc<Graph>
new_bspline_edge	(BGLgeom::Vertex_desc<Graph> const& src,
					 BGLgeom::Vertex_desc<Graph> const& tgt,
					 std::vector<BGLgeom::point<dim>> const& C,
					 std::vector<double> const& k,
					 Graph & G){
	bool inserted;
	BGLgeom::Edge_desc<Graph> e;	
	std::tie(e, inserted) = boost::add_edge(src, tgt, G);
	check_if_edge_inserted<Graph>(e, inserted);
	
	// Setting up the geometry
	G[e].geometry.set_bspline(C,k);
	
	if(G[src].coordinates != C.front())
		std::cerr << "WARNING: source coordinates " << G[src].coordinates 
				<< " do not coincide with the parametrized function evaluated in t=0" << std::endl;
	if(G[tgt].coordinates != C.back())
		std::cerr << "WARNING: target coordinates " << G[tgt].coordinates
				<< " do not coincide with the parametrized function evaluated in t=1" << std::endl;
	
	#ifndef NDEBUG
		std::cout << "New edge created: " << G[e].geometry << std::endl;
	#endif
	return e;				 
}	//new_bspline_edge

}	//BGLgeom

#endif	//HH_GRAPH_BUILDER_HH
