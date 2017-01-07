/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	graph_builder.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Sept, 2016
	@brief	Some utilities to build a graph
*/

#ifndef HH_GRAPH_BUILDER_HH
#define HH_GRAPH_BUILDER_HH

#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <limits>
#include <functional>

#include "data_structure.hpp"
#include "generic_geometry.hpp"
#include "linear_geometry.hpp"
#include "bspline_geometry.hpp"

namespace BGLgeom{

/*!
	@breif Helper function to check if an edge is correctly inserted in graph
	@detail It prints an error message on the screen if the insertion of the
			edge failed according to the scenarios described in the reference
			for the function boost::add_edge. See its reference on BGL web page
*/
template <typename Graph>
void check_if_edge_inserted(BGLgeom::Edge_desc<Graph> const& e, bool const& inserted){
	if(!inserted){
		std::cerr << "Error while inserting edge!" << std::endl;
		std::cerr << "Failed insertion for edge " << e << "." << std::endl;
		std::cerr << "See documentation of the Boost Graph Library on function boost::add_edge." << std::endl;
	}
}	//check_edge_inserted



//! Giving to source node v all properties through assigning the Source_data_structure
template <typename Graph, typename Vertex_prop>
void give_vertex_properties	(Vertex_prop const& V_prop,
							 BGLgeom::Vertex_desc<Graph> const& v,
							 Graph & G){
	G[v] = V_prop;
}	//give_vertex_properties

//! Giving to edge e all properties through assigning the Edge_data_structure
template <typename Graph, typename Edge_prop>
void give_edge_properties	(Edge_prop const& E_prop,
							 BGLgeom::Edge_desc<Graph> const& e,
							 Graph & G){
	G[e] = E_prop;
}	//give_edge_properties


template <typename Graph>
BGLgeom::Vertex_desc<Graph>
new_vertex(Graph & G){	
	return boost::add_vertex(G);
}	//new_vertex


template <typename Graph, typename Vertex_prop>
BGLgeom::Vertex_desc<Graph>
new_vertex(Vertex_prop const& v_prop,
		   Graph & G, 
		   const bool check_unique = false){
	
	if(check_unique){	
		BGLgeom::Vertex_iter<Graph> v_it,v_end;	
		for(std::tie(v_it,v_end)=boost::vertices(G); v_it != v_end; ++v_it){
			if(v_prop.coordinates == G[*v_it].coordinates){
				#ifdef OUT_MSG
					std::cout<<"Vertex already existing"<<std::endl;	
				#endif
				return *v_it;
			}
		}
	}
	// if we arrived here, either check_unique = false or check_unique = true but there is no vertex with the same coordinates
	#ifdef OUT_MSG
		std::cout<<"New vertex created"<<std::endl;
	#endif
	return boost::add_vertex(v_prop,G);	
}	//new_vertex


template <typename Graph, unsigned int dim>
BGLgeom::Vertex_desc<Graph>
new_vertex(BGLgeom::point<dim> const& P,
		   Graph & G, 
		   const bool check_unique = false){
	
	BGLgeom::Vertex_base_property<dim> v_prop(P);
	
	if(check_unique){	
		BGLgeom::Vertex_iter<Graph> v_it,v_end;	
		for(std::tie(v_it,v_end)=boost::vertices(G); v_it != v_end; ++v_it){
			if(P == G[*v_it].coordinates){
				#ifdef OUT_MSG
					std::cout<<"Vertex already existing"<<std::endl;	
				#endif
				return *v_it;
			}
		}
	}
	// if we arrived here, either check_unique = false or check_unique = true but there is no vertex with the same coordinates
	#ifdef OUT_MSG
		std::cout<<"New vertex created"<<std::endl;
	#endif
	return boost::add_vertex(v_prop,G);	
}	//new_vertex


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
new_edge(	BGLgeom::Vertex_desc<Graph> const& src,
			BGLgeom::Vertex_desc<Graph> const& tgt,
			Graph & G){
	
	bool inserted;	
	typename BGLgeom::Edge_desc<Graph> e;
	
	std::tie(e, inserted) = boost::add_edge(src, tgt, G);
	check_if_edge_inserted<Graph>(e, inserted);
	#ifdef OUT_MSG
		std::cout<<"New edge created. "<<G[e].geometry<<std::endl;
	#endif
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
	#ifdef OUT_MSG
		std::cout<<"New edge created. "<<G[e].geometry<<std::endl;
	#endif
	return e;
}

/*!
	@brief	Adding a new linear edge to the graph
	@remark	Use this only when you set "linear_geometry<dim>" as template parameter of the
			Edge_base_property
	@detail	It adds a new edge assuming that the underlying geometry is the linear one.
			It takes care of setting up the geometry. Only the geometry is set up,
			the other properties are left defaulted
	@note 	It performs a check on the insertion of the edge
	@pre	This version of the function assumes that the coordinates of the vertices
			are already defined in the vertex properties of the verticesù
	@pre	Obviously the BGLgeom::Edge_base_property struct or derived is required 
			as edge property of the graph
			
	@param src Vertex descriptor for the source
	@param tgt Vertex descriptor fot the target
	@param G The graph
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
	#ifdef OUT_MSG
		std::cout<<"New edge created. "<<G[e].geometry<<std::endl;
	#endif
	return e;
}	//new_linear_edge

/*!
	@brief	Adding a new generic edge to the graph
	@remark	Use this only when you set "generic_geometry<dim>" as template parameter of the
			Edge_base_property
	@detail	It adds a new edge assuming that the underlying geometry is the generic one.
			It takes care of setting up the geometry, and so requires some additional
			parameters to do this. Only the geometry is set up, the other properties 
			are left defaulted
	@note 	It performs a check on the insertion of the edge
	@pre	Obviously the BGLgeom::Edge_base_property struct or derived is required 
			as edge property of the graph
			
	@param src Vertex descriptor for the source
	@param tgt Vertex descriptor fot the target
	@param G The graph
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
	
	if(src.coordinates != _fun(0))
		std::cerr<<"WARNING: source coordinates "<<src.coordinates<<"do not coincide with the parametrized function evaluated in t=0";
	else if(tgt.coordinates != _fun(1))
		std::cerr<<"WARNING: target coordinates "<<src.coordinates<<"do not coincide with the parametrized function evaluated in t=1";
	// Setting up the geometry
	G[e].geometry.set_function(_fun);
	G[e].geometry.set_first_der(_first_der);
	G[e].geometry.set_second_der(_second_der);
	#ifdef OUT_MSG
		std::cout<<"New edge created. "<<G[e].geometry<<std::endl;
	#endif
	return e;	
}	//new_generic_edge

/*!
	@brief	Adding a new bspline edge to the graph
	@remark	Use this only when you set "bspline_geometry<dim,deg>" as template parameter of the
			Edge_base_property
	@detail	It adds a new edge assuming that the underlying geometry is the bspline one.
			It takes care of setting up the geometry, so some additional parameters are
			required. Only the geometry is set up, the other properties are left defaulted
	@note 	It performs a check on the insertion of the edge
	@pre	Obviously the BGLgeom::Edge_base_property struct or derived is required 
			as edge property of the graph
			
	@param src Vertex descriptor for the source
	@param tgt Vertex descriptor fot the target
	@param G The graph
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
	
	if(src.coordinates != C.front())
		std::cerr<<"WARNING: source point "<<src.coordinates<<"does not coincide with the first control point of the b-spline";
	else if(tgt.coordinates != C.back())
		std::cerr<<"WARNING: target point "<<src.coordinates<<"does not coincide with the last control point of the b-spline";
	

	#ifdef OUT_MSG
		std::cout<<"New edge created. "<<G[e].geometry<<std::endl;
	#endif
	return e;				 
}	//new_bspline_edge

/*!
	@brief	Adding a new bspline edge to the graph
	@remark	Use this only when you set "bspline_geometry<dim,deg>" as template parameter of the
			Edge_base_property
	@detail	It adds a new edge assuming that the underlying geometry is the bspline one.
			It takes care of setting up the geometry, so some additional parameters are
			required. Only the geometry is set up, the other properties are left defaulted
	@note 	It performs a check on the insertion of the edge
	@pre	Obviously the BGLgeom::Edge_base_property struct or derived is required 
			as edge property of the graph
			
	@param src Vertex descriptor for the source
	@param tgt Vertex descriptor fot the target
	@param G The graph
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
	
	if(src.coordinates != C.front())
		std::cerr<<"WARNING: source point "<<src.coordinates<<"does not coincide with the first control point of the b-spline";
	else if(tgt.coordinates != C.back())
		std::cerr<<"WARNING: target point "<<src.coordinates<<"does not coincide with the last control point of the b-spline";
	
	#ifdef OUT_MSG
		std::cout<<"New edge created "<<G[e].geometry<<std::endl;
	#endif
	return e;				 
}	//new_bspline_edge

}	//BGLgeom

#endif	//HH_GRAPH_BUILDER_HH
