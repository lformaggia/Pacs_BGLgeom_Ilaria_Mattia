/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*!
	@file	graph_access.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Types and functions from BGL to access vertices and edges
	
	We collect here the most used boost::graph_traits and functions 
	to access, create and handle the elements of a graph.
	In particular we give a type alias for vertex and edge descriptors 
	and for vertex and edge iterators, and we include in the BGLgeom 
	namespace the boost function to get the vertex and the edge iterators
	ant to get the vertex descriptors of a given edge
	
*/

#ifndef HH_GRAPH_ACCESS_HH
#define HH_GRAPH_ACCESS_HH

#include <boost/graph/adjacency_list.hpp>

namespace BGLgeom{

/*!
	@defgroup	Accessing_Graph_items	Aliases type for BGL
	
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
	@defgroup	Accessing_functions	Some useful aliases for function from BGL, used
									very often to access vertices or edges in the 
									graph

	They are utilities to hidden boost::edges(G) and to allow to use only 
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
	@param G The graph
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
	@param G The graph
	@return The vertex descriptor of the target of edge e
*/
template <typename Graph>
BGLgeom::Vertex_desc<Graph>
target(BGLgeom::Edge_desc<Graph> const& e, Graph const& G){
	return boost::target(e, G);
}
/*! @} */

}	//BGLgeom

#endif	//HH_GRAPH_ACCESS_HH
