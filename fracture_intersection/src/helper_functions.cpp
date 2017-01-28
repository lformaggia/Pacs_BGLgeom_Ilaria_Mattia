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
	@file helper_functions.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Implementations
*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "helper_functions.hpp"
#include "graph_builder.hpp"
#include "graph_access.hpp"
#include "types_definition.hpp"

using namespace Fracture;
// For overload of operators in BGLgeom library, for instance operator< or > for points:
using namespace BGLgeom;

namespace Fracture{	
	
bool asc_order(Fracture::Int_layer<Graph> & I1, Fracture::Int_layer<Graph> &I2){
	// ordering of I1 in case it contains two intersection points
	if(I1.int_pts.size()==2 && I1.int_pts[0] > I1.int_pts[1]){
		std::swap(I1.int_pts[0],I1.int_pts[1]);
		I1.swapped_comp = true;
	}
	// ordering of I2 in case it contains two intersection points
	if(I2.int_pts.size()==2 && I2.int_pts[0] > I2.int_pts[1]){
		std::swap(I2.int_pts[0],I2.int_pts[1]);
		I2.swapped_comp = true;
	}	
	return (I1.int_pts[0] < I2.int_pts[0]);
}; //asc_order


bool desc_order(Fracture::Int_layer<Graph> & I1, Fracture::Int_layer<Graph> &I2){
	// ordering of I1 in case it contains two intersection points
	if(I1.int_pts.size()==2 && I1.int_pts[0] < I1.int_pts[1]){
		std::swap(I1.int_pts[0],I1.int_pts[1]);
		I1.swapped_comp = true;
	}
	// ordering of I2 in case it contains two intersection points
	if(I2.int_pts.size()==2 && I1.int_pts[0] < I1.int_pts[1]){
		std::swap(I2.int_pts[0],I2.int_pts[1]);
		I2.swapped_comp = true;
	}	
	return (I1.int_pts[0] > I2.int_pts[0]);
}; //desc_order


bool is_duplicate(const Fracture::Int_layer<Graph> & I1, const Fracture::Int_layer<Graph> & I2){
	// Checking if I1 
	if(I1.how == intersection_type::Common_extreme || I1.how == intersection_type::T_old){
		for(const point2 & P2: I2.int_pts)
			if(I1.int_pts[0] == P2) return true;
	}
	// Doing the same as before, inverting I1 and I2
	if(I2.how == intersection_type::Common_extreme || I2.how == intersection_type::T_old){
		for(const point2 & P1: I1.int_pts)
			if(I2.int_pts[0] == P1) return true;
	}
	return false;
};


void cut_old_edge(Edge_d &e, const Vertex_d & v, Graph & G){
	Vertex_d src = boost::source(e, G);
	Vertex_d tgt = boost::target(e, G);	
	Edge_d e_tmp;
	
	e_tmp = new_linear_edge(src, v, G[e], G);
	e_tmp = new_linear_edge(v, tgt, G[e], G);	
	
	boost::remove_edge(e, G);
	#ifndef NDEBUG
		std::cout << "Edge removed: " << G[e].geometry << std::endl; 
	#endif
}


void create_graph(Graph & G, 
				  Reader & R,
				  std::function<void(Fracture::Edge_prop &, const Fracture::Edge_prop &)> update_edge_properties){
	// Utilities to create the graph:	
	
	// Edge iterators to iterate over the graph
	Edge_it e_it, e_end;
	// The descriptor of the new added edges (it will be overwritten by any new addition)
	Edge_d e;
	//object of class Intersection (BGLgeom)
	Intersection intobj_tmp; 

	// To store source and target properties read from the input file (Fracture)
	Vertex_prop src_prop, tgt_prop; 
	// To store edge_properties read from the input file (Fracture)
	Edge_prop e_prop;
	/*
	Int_layer (BGLgeom) is an object containing all the info we need about the 
	type of intersection. For each new pair of points representing a fracture, 
	intvect will contain the ordered sequence of the intersections between this 
	new line and the edges already present in the graph.
	intvect stands for "vector of the intersections"
	*/
	std::vector<Int_layer<Graph>> intvect;	

	// Number of the fracture, used as index
	unsigned int frac_num = 0;
	
	// Other debug utilities: counter for edges and vertices in the graph
	int count_e = 0;
	int count_v = 0;

	while(!R.is_eof()){
		++frac_num;		//updating index for the frcture number
		std::cout << std::endl;
		std::cout<< "------------------- READING LINE " << frac_num << "-------------------" << std::endl;
		
		//#ifndef NDEBUG or #ifndef VERBOSE
			std::cout << "Current situation: ";
			count_v = 0; 
			Vertex_it vv,vvend;
			for(std::tie(vv,vvend) = boost::vertices(G); vv!=vvend; ++vv) 
				++count_v;
			std::cout << count_v << " vertices, "<<std::endl;
		//#endif
		
		//Getting data form input file
		R.get_data();
		src_prop = R.get_source_data();
		tgt_prop = R.get_target_data();
		e_prop = R.get_edge_data();
		e_prop.index = frac_num;
		
		// Insertion of new vertices. If the coordinates matches with an already existing one, they returns those vertex descriptors
		Vertex_d src = new_vertex(src_prop, G, true);
		Vertex_d tgt = new_vertex(tgt_prop, G, true);		
		
		// Creation of the new current line we want to insert
		const line L(G[src].coordinates, G[tgt].coordinates);		
		// Preparing variable to the next loop
		count_e = 0;
		intvect.clear();
		// Checking for intersection of L with all the edges already present in the graph
		for (std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
			++count_e;			
			intobj_tmp = BGLgeom::compute_intersection(G[*e_it].geometry, L);
			if(intobj_tmp.intersect == true){
				Fracture::Int_layer<Graph> intobj(intobj_tmp, *e_it);
				intvect.push_back(intobj);
			}	
		} //for
		
		//#ifndef NDEBUG
			std::cout << count_e << " edges." << std::endl;
		//#endif
		
		// Insertion of the new edge, handling intersections if present
		// if intvect is empty it means that the new fracture does not intersect any of the edges in graph
		if(intvect.empty()){
			std::cout << "No intersections" << std::endl;
			e = new_linear_edge(src,tgt,e_prop,G);
		} else { // there is at least one intersection	
			
			// Order intvect in increasing or decreasing order based on the relative position of src and tgt 
			if(G[src].coordinates < G[tgt].coordinates)
				std::sort(intvect.begin(), intvect.end(), asc_order);
			else
				std::sort(intvect.begin(), intvect.end(), desc_order);

			#ifndef NDEBUG
				std::cout << "ORDERED VECTOR" << std::endl;	
				for(const Fracture::Int_layer<Graph> & I: intvect)
					std::cout << I << std::endl;
			#endif
			
			// Removal of duplicated intersection points with Common_extreme or T_old intersection situation
			auto last = std::unique(intvect.begin(), intvect.end(), is_duplicate);
			intvect.erase(last, intvect.end());
			
			#ifndef NDEBUG
				std::cout << "VECTOR WITHOUT DUPLICATES" << std::endl;	
				for(const Fracture::Int_layer<Graph> & I: intvect)
					std::cout << I << std::endl;
			#endif
			
			#ifdef NDEBUG	//In debug mode there already are more detailed infos about intersections
				std::cout << "Number of intersection objects: " << intvect.size() << std::endl;
			#endif
			
			/* 
			We now create connections and break old edges proceeding from an 
			intersection point to the following intersection point, starting 
			obviously by the source
			*/
			Vertex_d next_src;
			Vertex_d current_src = src;											
			// First, we resolve the connection between source and first intersection
			refine_graph(G, src, intvect[0], e_prop, tgt, next_src, update_edge_properties);
			// Then, all the other ones
			for(std::size_t i=1; i<intvect.size(); ++i){
				current_src = next_src;
				refine_graph(G, current_src, intvect[i], e_prop, tgt, next_src, update_edge_properties); // graph, current source, current intersection object
			}	
			// Finally we connect the last intersection point with the target, but only if their vertex_desciptors do not coincide
			current_src = next_src;
			if(!(current_src == tgt))
				e = new_linear_edge(current_src, tgt, e_prop, G);										
		}// else 
	} //while
}; //create_graph

void refine_graph	(Graph &G, 
					 const Vertex_d & src, 
					 Fracture::Int_layer<Graph> & I,
					 Fracture::Edge_prop & e_prop, 
					 const Vertex_d & tgt, 
					 Vertex_d & next_src,
					 std::function<void(Fracture::Edge_prop &, const Fracture::Edge_prop &)> update_edge_properties){
					 
	/* The edge descriptor of the new inserted edges 
	(it will be overwritten for each new insertion) */
	Edge_d e;
	using int_type = BGLgeom::intersection_type;
	
	switch(I.how) {
	
		case int_type::X : {
			// New vertex_descriptor for the intersection point
			Fracture::Vertex_prop v_prop(I.int_pts.front());
			Vertex_d v = new_vertex(v_prop, G);			
			
			e = new_linear_edge(src, v, e_prop, G);
			cut_old_edge(I.int_edge, v, G);
			
			// Setting the source vertex descriptor for the following iteration
			next_src = v;
			break;
		}	//X
		
		case int_type::T_new : {
			// if the intersection point corresponds to the target I add a new edge
			if(I.intersected_extreme_new == 1){
				e = new_linear_edge(src, tgt, e_prop,G);
				cut_old_edge(I.int_edge, tgt,G);
				next_src = tgt;
			}	
			else{
				cut_old_edge(I.int_edge, src, G);
				next_src = src;			
			}			
			break;
		}	//T_new
		
		case int_type::T_old : {
			// Getting the vertex descriptor of the extreme involved in the intersection
			Vertex_d v;
			if(I.intersected_extreme_old == 0)
				v = boost::source(I.int_edge, G);
			else
				v = boost::target(I.int_edge, G);
				
			e = new_linear_edge(src, v, e_prop, G);
			
			// Setting the source vertex descriptor for the following iteration
			next_src = v;
			break;
		}
		
		case int_type::Common_extreme : {
			// Getting the vertex descriptor of the extreme involved in the intersection
			Vertex_d v;
			if(I.intersected_extreme_old == 0) //first extreme, i.e the source
				v = boost::source(I.int_edge, G);
			else
				v = boost::target(I.int_edge, G);				
			
			// If src has same coordinates as v there's nothing to do, so we consider only the opposite case
			if(!(src == v))
				e = new_linear_edge(src, v, e_prop, G);
			
			next_src = v;			
			break;
		}	//Common_extreme
		
		case int_type::Identical : {
			// The edge_descriptor contained into object I is totally overlapped by the new edge, therefore I update its properties to take into account this fact
			update_edge_properties(G[I.int_edge], e_prop);

 			//Vertex_d v1;
			Vertex_d v2;

			if(!I.swapped_comp){
				//v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);
			}
			else{
				//v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);				
			}
			
			next_src = v2; 			
 			break;		
		}	//Identical
		
		case int_type::Overlap : {
			Vertex_d v1;
			Vertex_d v2;

			if(!I.swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);				
			}
			
			if(I.intersected_extreme_new == 1){	//it means that src is outside and tgt inside
				e = new_linear_edge(src,v1, e_prop, G);
				
				e = new_linear_edge(v1,tgt, G[I.int_edge], G);	
				// Edge e is overlapped by the new edge, therefore I update its properties to take into account this fact			
				update_edge_properties(G[e],e_prop);				
				
				e = new_linear_edge(tgt,v2,G[I.int_edge],G);
				
				/* Since next_src = tgt, no new line will be added in the last step 
				(this is necessarily the last intersection of the vector, otherwise 
				it couldn't involve the target)	*/			
				next_src = tgt;
			}		
			else{	//it means that src is inside and tgt outside
				e = new_linear_edge(v1,src,G[I.int_edge],G);
				
				e = new_linear_edge(src,v2,G[I.int_edge],G);
				// Edge e is overlapped by the new edge, therefore I update its properties to take into account this fact			
				update_edge_properties(G[e],e_prop);
								
				next_src = v2;
			}	
			
			boost::remove_edge(I.int_edge, G);
			#ifndef NDEBUG
				std::cout<<"Edge removed"<< G[e].geometry <<std::endl;
			#endif
			break;			
		}	//Overlap
	
		case int_type::Overlap_inside : {
			Vertex_d v1;
			Vertex_d v2;
								
			if(!I.swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);						
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);						
			}

			e = new_linear_edge(v1,src,G[I.int_edge],G);
			
			e = new_linear_edge(src,tgt,G[I.int_edge],G);
			// Updating properties of the overlapped edge
			update_edge_properties(G[e], e_prop);			
			
			e = new_linear_edge(tgt,v2,G[I.int_edge],G);		
			
			// Removal of preexisting edge
			boost::remove_edge(I.int_edge,G);
			#ifndef NDEBUG
				std::cout<<"Edge removed"<< G[e].geometry<<std::endl;						
			#endif
			
			next_src = tgt; // setting next_src to tgt so that no new connection will be performed
			break;
		}	//Overlap_inside
		
		case int_type::Overlap_outside : {
			Vertex_d v1;
			Vertex_d v2; 
			
			// If the components wasn't inverted, the nearest to src is the source, otherwise the target
			if(!I.swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);				
			}
			
			if(v1 != src) // otherwise the edge has already been added
				e = new_linear_edge(src, v1, e_prop, G);
		
			// updating the properties of the overlapped edge
 			update_edge_properties(G[I.int_edge], e_prop);
 			
			next_src = v2; 			
 			break;
		}	//Overlap_outside
		
		case int_type::Overlap_extreme_inside : {
			Vertex_d v1;
			Vertex_d v2;
								
			if(!I.swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);						
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);						
			}			
			
			if(v1 == src){ //the common extreme is the source, because they have the same vertex descriptor
				e = new_linear_edge(tgt,v2,G[I.int_edge],G);
				e = new_linear_edge(src,tgt, G[I.int_edge],G);
			}						
			else{ //the common extreme is the target
				e = new_linear_edge(v1,src,G[I.int_edge],G);
				e = new_linear_edge(src,tgt,G[I.int_edge],G);
			}
			
			// in both cases of the if-else clause e is the edge_descr of the edge connecting src and tgt: here we update its properties
			update_edge_properties(G[e],e_prop); 
			
			// Removal of preexisitng edge
			boost::remove_edge(I.int_edge,G);
			#ifndef NDEBUG
				std::cout<<"Edge removed"<< G[e].geometry<<std::endl;
			#endif
			next_src = tgt; // setting next_src to tgt so that no new connection will be performed
			break;
		}	//Overlap_extreme_inside		
		
		case int_type::Overlap_extreme_outside : {
			Vertex_d v1;
			Vertex_d v2;

			if(!I.swapped_comp){
				v1 = boost::source(I.int_edge, G);
				v2 = boost::target(I.int_edge, G);
			}
			else{
				v1 = boost::target(I.int_edge, G);
				v2 = boost::source(I.int_edge, G);				
			}		
			
			// It means that the common extreme is the target. I have to connect src to v1	
			if(!(src == v1))
				e = new_linear_edge(src,v1, e_prop, G);
				
			// Then we have simply to update the properties of the other part of the edge
			update_edge_properties(G[I.int_edge], e_prop);
			
			next_src = v2;
			break;
		}	//Overlap_extreme_outside
		
		// Should never reach this (handled before using this function in the algorithm)
		case int_type::No_intersection : {
			//Nothing to do. No need to handle the "error".
			break;
		}	//No_intersection
	
		// and this
		case int_type::Something_went_wrong : {
			std::cerr << "Fracture::refine_graph(): Intersection type: Something_went_wrong. Aborting" << std::endl;
			exit(EXIT_FAILURE);
			break;
		}	//Something_went_wrong
			
	}	//switch

}; //refine_graph

}	//Fracture
