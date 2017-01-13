/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file main_fractures.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Jan, 2017
	@brief Source code for fractures application
*/

#include <iostream>
#include <string>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

#include "type_aliases.hpp"  // using-declarations local to this application 
#include "reader_fractures.hpp"
#include "reader_fractures_twolines.hpp"
#include "graph_builder.hpp"
#include "helper_functions.hpp"
#include "writer_vtp.hpp"

using namespace boost;
using namespace BGLgeom;
using namespace Fracture;

int main(){
	
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
	/*
	Boolean to handle non intersecting new edges.
	If the edge doesn't intersect any of the already present edges we need to 
	create it
	(((otherwise it will appear in the graph already divided in sub-edges connecting 
	each intersection to the next one)))) ??????
	*/	
	bool edge_alone; 
	// Number of the fracture, used as index
	unsigned int frac_num = 0;
	
	// Other debug utilities: counter for edges and vertices in the graph
	int count_e = 0;
	int count_v = 0;
	
	cout << "===================== GRAPH 1 ===================" << endl;
	std::string filename1("../data/fractureElevenVF.dat");
	cout << "Input filename: " << filename1 << endl << endl;
	
	Graph G1;
	
	// Reading the file1
	reader_fractures R1(filename1);	
	R1.ignore_dummy_lines(7);	
	while(!R1.is_eof()){
		++frac_num;		//updating index for the frcture number
		std::cout << std::endl;
		std::cout<< "------------------- READING LINE " << frac_num << "-------------------" << std::endl;
		
		//#ifndef NDEBUG or #ifndef VERBOSE
			cout << "Current situation: ";
			count_v = 0; 
			Vertex_it vv,vvend;
			for(std::tie(vv,vvend) = boost::vertices(G1); vv!=vvend; ++vv) 
				++count_v;
			std::cout << count_v << " vertices, ";
		//#endif
		
		//Getting data form input file
		R1.get_data();
		src_prop = R1.get_source_data();
		tgt_prop = R1.get_target_data();
		e_prop = R1.get_edge_data();
		e_prop.index = frac_num;
		
		// Insertion of new vertices. If the coordinates matches with an already existing one, they returns those vertex descriptors
		Vertex_d src = new_vertex(src_prop, G1, true);
		Vertex_d tgt = new_vertex(tgt_prop, G1, true);		
		
		// Creation of the new current line we want to insert
		const line L(G1[src].coordinates, G1[tgt].coordinates);		
		// Preparing variable to the next loop
		count_e = 0;
		edge_alone = true;
		intvect.clear();
		// Checking for intersection of L with all the edges already present in the graph
		for (std::tie(e_it, e_end) = boost::edges(G1); e_it != e_end; ++e_it){
			++count_e;			
			intobj_tmp = BGLgeom::compute_intersection(G1[*e_it].geometry, L);
			if(intobj_tmp.intersect == true){
				BGLgeom::Int_layer<Graph> intobj(intobj_tmp, *e_it);
				edge_alone = false;
				intvect.push_back(intobj);
			}	
		} //for
		
		//#ifndef NDEBUG
			std::cout << count_e << " edges." << std::endl;
		//#endif
		
		// Insertion of the new edge, handling intersections if present
		if(edge_alone){
			//#ifndef NDEBUG
				std::cout << "This fracture does not intersect with any of the other fractures already in the graph." << std::endl;
			//#endif
			e = new_fracture(src,tgt,e_prop,G1);
		}
		else{ // there is at least one intersection	
			
			// Order intvect in increasing or decreasing order based on the relative position of src and tgt 
			if(G1[src].coordinates < G1[tgt].coordinates)
				std::sort(intvect.begin(), intvect.end(), asc_order);
			else
				std::sort(intvect.begin(), intvect.end(), desc_order);

			#ifndef NDEBUG
				std::cout << "ORDERED VECTOR" << std::endl;	
				for(const BGLgeom::Int_layer<Graph> & I: intvect)
					std::cout << I << std::endl;
			#endif
			
			// Removal of duplicated intersection points with Common_extreme or T_old intersection situation
			auto last = std::unique(intvect.begin(), intvect.end(), is_duplicate);
			intvect.erase(last, intvect.end());
			
			#ifndef NDEBUG
				std::cout << "VECTOR WITHOUT DUPLICATES" << std::endl;	
				for(const BGLgeom::Int_layer<Graph> & I: intvect)
					std::cout << I << std::endl;
			#endif
			
			/* 
			We now create connections and break old edges proceeding from an 
			intersection point to the following intersection point, starting 
			obviously by the source
			*/
			Vertex_d next_src;
			Vertex_d current_src = src;											
			// First, we resolve the connection between source and first intersection
			refine_graph(G1, src, tgt, e_prop, intvect[0], next_src);
			// Then, all the other ones
			for(std::size_t i=1; i<intvect.size(); ++i){
				current_src = next_src;
				refine_graph(G1, current_src, tgt, e_prop, intvect[i], next_src);	// graph, current source, current intersection object
			};			
			// Finally we connect the last intersection point with the target, but only if their vertex_desciptors do not coincide
			current_src = next_src;
			if(!(current_src == tgt));
				e = new_fracture(current_src, tgt, e_prop, G1);										
		}// else 
	} //while
	
	// Counting total of vertices and edges
	count_e = 0;	
	for (std::tie(e_it, e_end) = boost::edges(G1); e_it != e_end; ++e_it)
		++count_e;	
	Vertex_it v_it, v_end;
	count_v = 0;
	for (std::tie(v_it, v_end) = boost::vertices(G1); v_it != v_end; ++v_it)
		++count_v;	
	cout << endl;
	std::cout << "________________ FINAL SETTING GRAPH 1 ________________" << std::endl;
	std::cout << count_v << " vertices and " << count_e << " edges" <<std::endl;
	
	//Producing output
	std::string filename1_out("../data/graph_fractureEleven.vtp");	
	writer_vtp<Graph,2> W1(filename1_out.c_str());
	W1.export_vtp(G1);
	
	cout<<endl;
	cout<<endl;
	
//==================================== THE OTHER INPUT FILE ==================================
	
	cout<<endl;
	cout<<"========================= GRAPH 2 =========================="<<endl;
	
	// Reinitializing variables
	count_e = 0; 
	count_v = 0;	
	frac_num = 0;
	
	Graph G2;
	
	// Reading file 2
	std::string filename2("../data/fracture.txt");
	reader_fractures_twolines R2(filename2);
	while(!R2.is_eof()){
		++frac_num;		//updating index for the frcture number
		std::cout << std::endl;
		std::cout<< "------------------- READING LINE " << frac_num << "-------------------" << std::endl;
		
		//#ifndef NDEBUG
			cout << "Current situation: ";		
			count_v = 0; 
			Vertex_it vv,vvend;
			for(std::tie(vv,vvend) = boost::vertices(G2); vv!=vvend; ++vv) 
				++count_v;
			std::cout << count_v << " vertices, ";
		//#endif
		
		//Getting data form input file
		R2.get_data();
		src_prop = R2.get_source_data();
		tgt_prop = R2.get_target_data();
		e_prop = R2.get_edge_data();
		e_prop.index = frac_num;
		
		// Insertion of new vertices. If the coordinates matches with an already existing one, they returns those vertex descriptors
		Vertex_d src = new_vertex(src_prop, G2, true);
		Vertex_d tgt = new_vertex(tgt_prop, G2, true);		
		
		// Creation of the new current line we want to insert
		const line L(G2[src].coordinates, G2[tgt].coordinates);		
		// Preparing variable to the next loop
		count_e = 0;
		edge_alone = true;
		intvect.clear();
		// Checking for intersection of L with all the edges already present in the graph
		for (std::tie(e_it, e_end) = boost::edges(G2); e_it != e_end; ++e_it){
			++count_e;			
			intobj_tmp = BGLgeom::compute_intersection(G2[*e_it].geometry, L);
			if(intobj_tmp.intersect == true){
				BGLgeom::Int_layer<Graph> intobj(intobj_tmp, *e_it);
				edge_alone = false;
				intvect.push_back(intobj);
			}	
		} //for
		
		//#ifndef NDEBUG
			std::cout << count_e << " edges." << std::endl;
		//#endif
		
		// Insertion of the new edge, handling intersections if present
		if(edge_alone){
			//#ifndef NDEBUG
				std::cout << "This fracture does not intersect with any of the other fractures already in the graph." << std::endl;
			//#endif
			e = new_fracture(src,tgt,e_prop,G2);
		}
		else{ // there is at least one intersection	
			
			// Order intvect in increasing or decreasing order based on the relative position of src and tgt 
			if(G2[src].coordinates < G2[tgt].coordinates)
				std::sort(intvect.begin(), intvect.end(), asc_order);
			else
				std::sort(intvect.begin(), intvect.end(), desc_order);

			#ifndef NDEBUG
				std::cout << "ORDERED VECTOR" << std::endl;	
				for(const BGLgeom::Int_layer<Graph> & I: intvect)
					std::cout << I << std::endl;
			#endif
			
			// Removal of duplicated intersection points with Common_extreme or T_old intersection situation
			auto last = std::unique(intvect.begin(), intvect.end(), is_duplicate);
			intvect.erase(last, intvect.end());
			
			#ifndef NDEBUG
				std::cout << "VECTOR WITHOUT DUPLICATES" << std::endl;	
				for(const BGLgeom::Int_layer<Graph> & I: intvect)
					std::cout << I << std::endl;
			#endif
			
			/* 
			We now create connections and break old edges proceeding from an 
			intersection point to the following intersection point, starting 
			obviously by the source
			*/
			Vertex_d next_src;
			Vertex_d current_src = src;							
			// First, we resolve the connection between source and first intersection
			refine_graph(G2, src, tgt, e_prop, intvect[0], next_src);
			// Then, all the other ones
			for(std::size_t i=1; i<intvect.size(); ++i){
				current_src = next_src;
				refine_graph(G2, current_src, tgt, e_prop, intvect[i], next_src);	// graph, current source, current intersection object
			};			
			// Finally we connect the last intersection point with the target, but only if their vertex_desciptors do not coincide
			current_src = next_src;
			if(!(current_src == tgt));
				e = new_fracture(current_src, tgt, e_prop, G2);										
		}// else 
	} //while
	
	// Counting total of vertices and edges
	count_e = 0;	
	for (std::tie(e_it, e_end) = boost::edges(G2); e_it != e_end; ++e_it)
		++count_e;	
	//Vertex_it v_it, v_end; Previously declared
	count_v = 0;
	for (std::tie(v_it, v_end) = boost::vertices(G2); v_it != v_end; ++v_it)
		++count_v;
	std::cout << endl;
	std::cout << "________________ FINAL SETTING GRAPH 2 ________________" << std::endl;
	std::cout << count_v << " vertices and " << count_e << " edges" << std::endl;
	
	// Production of the output
	std::string filename2_out("../data/graph_fracture.vtp");
	writer_vtp<Graph,2> W2(filename2_out.c_str());
	W2.export_vtp(G2);	
	
	return 0;
};
