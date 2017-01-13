/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file helper_functions.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Functions to implement the algorithm to build the graph
*/

#ifndef HH_HELPER_FUNCTIONS_HH
#define HH_HELPER_FUNCTIONS_HH

#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <limits> 

#include "intersections2D_utilities.hpp"
#include "type_aliases.hpp"
#include "fracture_graph_properties.hpp"

namespace Fracture{
	
	/*!
		@brief	Computing which intersection point comes first in ascending order
		
		This function gets two objects of type Int_layer, which contain up to two 
		intersection points, depending on the intersection situation between the 
		two edges. If one object contains two intersection points, the function 
		orders them in ascending order, and then compare the first intersection 
		point of each object to see which one comes first (proceeding from source 
		to target, ascending order). We use this function to sort all the intersections 
		founded between the current edge we are inserting in the graph and all the 
		other edges already present
		
		@param I1 An object of type Int_layer which contains intersection infos about
					one edge
		@param I2 An object of type Int_layer which contains intersection infos about
					another edge
		@return True if the first intersection point of I1 comes first in ascending 
				order w.r.t. the first intersection point of I2; false otherwise
	*/
	bool asc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2);
	
	/*!
		@brief	Computing which intersection point comes first in descending order
		
		This function gets two objects of type Int_layer, which contain up to two 
		intersection points, depending on the intersection situation between the 
		two edges. If one object contains two intersection points, the function 
		orders them in descending order, and then compare the first intersection 
		point of each object to see which one comes first (proceeding from source 
		to target, descending order). We use this function to sort all the intersections 
		founded between the current edge we are inserting in the graph and all the 
		other edges already present
		
		@param I1 An object of type Int_layer which contains intersection infos about
					one edge
		@param I2 An object of type Int_layer which contains intersection infos about
					another edge
		@return True if the first intersection point of I1 comes first in descending 
				order w.r.t. the first intersection point of I2; false otherwise
	*/
	bool desc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2);
	
	/*!
		@brief	Checks if two Int_layer objects share a Common_extreme or T_old 
				intersection situation
		
		Why this? To remove duplicates of the same intersection point with multiple 
		different edges (think of an edge that intersects with one of its extremes 
		exactly in a vertex of the graph, common to two or more edges). This may 
		happen considering the T_old and Common_extreme intersection situations, 
		and we need to remove these multiple intersection not to create "null" edges 
		in the graph (i.e. null length edges from one vertex to the same vertex)
		This function will be used in a std algorithm to find all the duplicates 
		among all the intersection points between the current edge we are inserting 
		in the graph and all the other edges already present
		
		@param I1 The first Int_layer object to be compared
		@param I2 The second Int_layer object to be compared
		@return True if the two objects share an intersection point with a Common_extreme 
				or T_old intersection situation; false otherwise
	*/
	bool is_duplicate(const BGLgeom::Int_layer<Graph> & I1, const BGLgeom::Int_layer<Graph> & I2);
	
	/*!
		@brief	Breaking a preexisting edge that intersects in one of its 
				inner points with the incoming one 
				
		This function breaks an existing edge that intersects with a new one 
		in one of its inner points (but not the extremes), represented by the 
		vertex descriptor v, into two new edges: one from source to v, and 
		one from v to target
				
		@param e The preexisting edge to be cut
		@param v The vertex descriptor of the intersection point 
		@param G The graph
	*/
	void cut_old_edge(Edge_d &e, const Vertex_d & v, Graph & G);
	
	/*!
		@brief	Updating edge properties of overlapping edges
		
		It may happen that two edges intersect with a partial or total overlapping. 
		In this case, the properties of the edge that originates from the overlap 
		has to be set according to some critirion
		
		@remark In this implememntation we keep as properties the ones present on the old 
				edge, i.e. the new edge that overlaps does not bring new information to 
				that edge
		@remark The user can redefine this function and define how the properties 
				on that edge should change
		
		@param e The edge descriptor of the edge whose properties has to be updated
		@param new_edge_prop ???????????????????????????????'
		@param G The graph
	*/
	void update_edge_properties(Edge_d &e, Fracture::Edge_prop & new_edge_prop, Graph &G);
	
	// Maybe not needed
	Edge_d new_fracture(Vertex_d const& src, Vertex_d const& tgt, Fracture::Edge_prop & edge_prop, Graph & G);
	
	/*!
		@brief	Refines the graph
		
		It takes into account all the intersections of the current edge we want to 
		insert and, according to them, creates new vertices and edges and breaks 
		already present edges
		
		@param G The graph
		@param src Vertex descriptor of the source of the current edge we want to insert. NO!!!!
		@param tgt vertex descriptor of the target of the current edge we want to insert. Si, credo.
		@param e_prop ???
		@param I
		@param next_src
		maybe: @param update_edge_prop function???
	*/
	void refine_graph	(Graph &G, 
						 const Vertex_d & src, 
						 const Vertex_d & tgt, 
						 Fracture::Edge_prop & e_prop, 
						 BGLgeom::Int_layer<Graph> & I, 
						 Vertex_d & next_src);
}	//Fractures

#endif
