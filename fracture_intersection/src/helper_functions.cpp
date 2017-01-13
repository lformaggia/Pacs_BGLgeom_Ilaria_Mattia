/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file helper_functions.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Implementations
*/

#include <iostream>
#include <cstdlib>
#include "helper_functions.hpp"
#include "graph_builder.hpp"
#include "type_aliases.hpp"

using namespace Fracture;
// For overload of operators in BGLgeom library, for instance operator< or > for points:
using namespace BGLgeom;

namespace Fracture{	
	
bool asc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2){
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


bool desc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2){
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


bool is_duplicate(const BGLgeom::Int_layer<Graph> & I1, const BGLgeom::Int_layer<Graph> & I2){
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
	
	e_tmp = new_fracture(src, v, G[e], G);
	e_tmp = new_fracture(v, tgt, G[e], G);	
	
	boost::remove_edge(e, G);
	#ifndef NDEBUG
		std::cout << "Edge removed: " << G[e] << std::endl; //G[src].coordinates<<";"<<G[tgt].coordinates<<")"<<std::endl;
	#endif
}


void update_edge_properties(Edge_d &e, Fracture::Edge_prop & new_edge_prop, Graph &G){
	// Doing nothing, i.e. keeping the old properties present on this edge
	#ifndef NDEBUG
		std::cout << "Updating properties of edge " << G[e].index << "..." << std::endl;
	#endif
// in the edges requiring update we inserted as properties the old ones, but there is an overlapping with the new inserted edge. Here you can decide how to "mix" the edge properties: just keep the old ones (as it is, so this function will do nothing in this case), just the new ones, or any other combination of the two
}


Edge_d new_fracture(Vertex_d const& src, Vertex_d const& tgt, Fracture::Edge_prop & edge_prop, Graph & G){
	// set source and target in the edge property (they may have changed since we are splitting some edges, so that they have new source and target)
	edge_prop.geometry.set_source(G[src].coordinates);
	edge_prop.geometry.set_target(G[tgt].coordinates);
		
	std::cout <<"New edge created. "<<edge_prop.geometry<<". Fracture number: "<<edge_prop.index<<std::endl; 
	return boost::add_edge(src,tgt,edge_prop,G).first;  
}



void refine_graph	(Graph &G, 
					 const Vertex_d & src, 
					 const Vertex_d & tgt, 
					 Fracture::Edge_prop & e_prop, 
					 BGLgeom::Int_layer<Graph> & I, 
					 Vertex_d & next_src){
	/* The edge descriptor of the new inserted edges 
	(it will be overwritten for each new insertion) */
	Edge_d e;
	using int_type = BGLgeom::intersection_type;
	
	switch(I.how) {
	
		case int_type::X : {
			// New vertex_descriptor for the intersection point
			Fracture::Vertex_prop v_prop(I.int_pts.front());
			Vertex_d v = new_vertex(v_prop, G);			
			
			e = new_fracture(src, v, e_prop, G);
			cut_old_edge(I.int_edge, v, G);
			
			// Setting the source vertex descriptor for the following iteration
			next_src = v;
			break;
		}	//X
		
		case int_type::T_new : {
			// if the intersection point corresponds to the target I add a new edge
			if(I.intersected_extreme_new == 1){
				e = new_fracture(src, tgt, e_prop,G);
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
				
			e = new_fracture(src, v, e_prop, G);
			
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
				e = new_fracture(src, v, e_prop, G);
			
			next_src = v;			
			break;
		}	//Common_extreme
		
		case int_type::Identical : {
 			update_edge_properties(I.int_edge,e_prop, G); 			
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
				e = new_fracture(src,v1, e_prop, G);
				e = new_fracture(v1,tgt, G[I.int_edge], G);				
				update_edge_properties(e,e_prop,G);				
				e = new_fracture(tgt,v2,G[I.int_edge],G);
				
				/* Since next_src = tgt, no new line will be added in the last step 
				(this is necessarily the last intersection of the vector, otherwise 
				it couldn't involve the target)	*/			
				next_src = tgt;
			}		
			else{	//it means that src is inside and tgt outside
				e = new_fracture(v1,src,G[I.int_edge],G);
				e = new_fracture(src,v2,G[I.int_edge],G);
				update_edge_properties(e,e_prop,G);				
				next_src = v2;
			}	
			
			boost::remove_edge(I.int_edge, G);
			#ifndef NDEBUG
				std::cout<<"Edge removed"<<std::endl;
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

			e = new_fracture(v1,src,G[I.int_edge],G);
			e = new_fracture(src,tgt,G[I.int_edge],G);
			// Updating properties of the overlapped edge
			update_edge_properties(e, e_prop, G);			
			e = new_fracture(tgt,v2,G[I.int_edge],G);		
			
			// Removal of preexisting edge
			boost::remove_edge(I.int_edge,G);
			#ifndef NDEBUG
				std::cout<<"Edge removed"<<std::endl;						
			#endif
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
				e = new_fracture(src, v1, e_prop, G);
		
 			update_edge_properties(I.int_edge, e_prop,G);
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
				e = new_fracture(tgt,v2,G[I.int_edge],G);
				e = new_fracture(src,tgt, G[I.int_edge],G);
			}						
			else{ //the common extreme is the target
				e = new_fracture(v1,src,G[I.int_edge],G);
				e = new_fracture(src,tgt,G[I.int_edge],G);
			}
			
			// in both cases of the if-else clause e is the edge_descr of the edge connecting src and tgt
			update_edge_properties(e,e_prop,G); 
			
			// Removal of preexisitng edge
			boost::remove_edge(I.int_edge,G);
			#ifndef NDEBUG
				std::cout<<"Edge removed"<<std::endl;
			#endif
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
				e = new_fracture(src,v1, e_prop, G);
				
			// Then we have simply to update the properties of the other part of the edge
			update_edge_properties(I.int_edge, e_prop,G);
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
