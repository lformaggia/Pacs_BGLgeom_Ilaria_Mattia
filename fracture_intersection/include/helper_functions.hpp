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

#include <functional>

#include "reader_ASCII.hpp"
#include "intersections2D_utilities.hpp"
#include "types_definition.hpp"
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
	bool asc_order(Fracture::Int_layer<Graph> & I1, Fracture::Int_layer<Graph> &I2);
	
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
	bool desc_order(Fracture::Int_layer<Graph> & I1, Fracture::Int_layer<Graph> &I2);
	
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
	bool is_duplicate(const Fracture::Int_layer<Graph> & I1, const Fracture::Int_layer<Graph> & I2);
	
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
		@brief	Creates the graph while reading the input file

		@param G The graph to be built
		@param R Concrete reader class to read the input file
		@param update_edge_properties   (optional) Lambda function used inside function refine_graph (see there its detailed description). The default is the empty function
		
	*/
	
	void create_graph(Graph & G, 
					  BGLgeom::reader_ASCII<Fracture::Vertex_prop, Fracture::Edge_prop> & R, 
					  std::function<void(Fracture::Edge_prop &, const Fracture::Edge_prop &)> update_edge_properties = [](Fracture::Edge_prop & current_prop, const Fracture::Edge_prop & new_prop){} );
	
	/*!
		@brief	Refines the graph
		
		It takes into account all the intersections of the current edge we want to 
		insert and, according to them, creates new vertices and edges and breaks 
		already present edges
		
		@param G        				The graph
		@param src 						Vertex descriptor of the source of the current "piece" of edge we are inserting
		@param I						Object of class Int_layer containing all the information about the intersection we are currently dealing with
		@param e_prop					Contains the properties of the new edge we are inserting in the graph
		@param tgt 						Vertex descriptor of the target of the new edge we are inserting in the graph 
		@param next_src					Vertex descriptor which at the end of the function will contain the src for the next loop iteration
		@param update_edge_properties   Lambda function which performs the "merge" operation between the edge properties of two overlapping edges (e.g. in the Identical case). Since the considered section of the edge currently inserted overlaps an already existing edge, no new edge will be added, but, through this function, the user has the possibility to decide how to update the edge properties of the overlapped edge (which are passed by reference for this reason); the edge property of the overlapping edge is passed as second parameter. The default behaviour is to keep the actual edge property as it is, without any modification due to the overlapping edge.
	*/
	void refine_graph	(Graph &G, 
						 const Vertex_d & src,  						 
						 Fracture::Int_layer<Graph> & I, 
						 Fracture::Edge_prop & e_prop, 
						 const Vertex_d & tgt,
						 Vertex_d & next_src,
						 std::function<void(Fracture::Edge_prop &, const Fracture::Edge_prop &)> update_edge_properties);
}	//Fractures

#endif
