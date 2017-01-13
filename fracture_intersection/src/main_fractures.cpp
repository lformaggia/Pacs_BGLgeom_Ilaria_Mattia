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
	
	std::string filename1("../data/fractureElevenVF.dat");
	std::string filename2("../data/fracture.txt");
	
	Graph G;
	Graph G2;
	reader_fractures R(filename1);
	reader_fractures_twolines R2(filename2);
	
	cout<<"________________ GRAPH 1 ________________"<<endl;
	// Reading the file1
	R.ignore_dummy_lines(7);
	
	Edge_it e_it, e_end;
	Edge_d e; // this will be the descriptor of the new added edge
	Intersection intobj_tmp; //object of class Intersection (BGLgeom)
	Vertex_prop src_prop, tgt_prop; // here we store source and target properties read from the input file (Fracture)
	Edge_prop e_prop; //(Fracture)
	bool edge_alone; //if the edge doesn't intersect any of the other edges we need to create it, otherwise it will appear in the graph already divided in sub-edges connecting each intersection to the next one
	
	std::vector<Int_layer<Graph>> intvect; // Int_layer (BGLgeom) is an object containing all the info we need about the type of intersection. For each new pair of points representing a fracture, intvect will contain the ordered sequence of the intersections between this new line and the edges already in the graph
	
	int count_e = 0; // They count edges and vertices in the graph respectively. Useful only for output messages
	int count_v = 0;
	
	unsigned int frac_num = 0;
	
	while(!R.is_eof()){
		++frac_num;
		cout<<endl;
		std::cout<<"-------------------READING LINE "<<frac_num<<"-------------------"<<std::endl;

		cout << "Current situation: ";		
		count_v = 0; 
		Vertex_it vv,vvend;
		for(std::tie(vv,vvend) = boost::vertices(G); vv!=vvend; ++vv) 
			++count_v;
		std::cout<<count_v<<" vertices, ";
	
		R.get_data();
		src_prop = R.get_source_data();
		tgt_prop = R.get_target_data();
		e_prop = R.get_edge_data();
		e_prop.index = frac_num; // set the index in the edge_property equal to frac_num
		
		Vertex_d src = new_vertex(src_prop, G, true); //inserts a new vertex and returns the vertex desc or simply returns an already existing vertex desc in the case the coordinates match
		Vertex_d tgt = new_vertex(tgt_prop, G, true);

		edge_alone = true;
		intvect.clear(); // at each line I have to start with the empty vector
		
		count_e = 0;

		// create the linear edge having source in src and target in tgt: this will be the input for the function compute_intersection
		const line L(G[src].coordinates, G[tgt].coordinates);
		
		
		for (std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){ //for loop on al the edges already in the graph
			++count_e;
			
			intobj_tmp = BGLgeom::compute_intersection(G[*e_it].geometry, L); //in intersection info about the type of intersection are stored
			if(intobj_tmp.intersect == true){
				BGLgeom::Int_layer<Graph> intobj(intobj_tmp, *e_it); // this is the structure I'm going to use
				edge_alone = false;
				intvect.push_back(intobj);
			}	
		} //for
		
		std::cout<<count_e<<" edges."<<std::endl;
		
		if(edge_alone){
			std::cout<<"This fracture does no intersect with any of the other fractures already in the graph."<<std::endl;
			e = new_fracture(src,tgt,e_prop,G);
		}
		else{ // there is at least one intersection		
			Vertex_d next_src;
			Vertex_d current_src = src;
			
			//order intvect in increasing or decreasing order based on the relative position of src and tgt 
			if(G[src].coordinates < G[tgt].coordinates){
				std::sort(intvect.begin(), intvect.end(), asc_order);
			}
			else{
				std::sort(intvect.begin(), intvect.end(), desc_order);
			}

			#ifndef NDEBUG
				std::cout<<"ORDERED VECTOR"<<std::endl;	
				for(const BGLgeom::Int_layer<Graph> & I: intvect)
					std::cout<<I<<std::endl;
			#endif
			
			//remove multiple "common_extreme" and "T_old" with same intersection vertex_descriptor: they don't affect the graph so we don't consider them
			auto last = std::unique(intvect.begin(), intvect.end(), is_duplicate);
			intvect.erase(last, intvect.end());
			
			#ifndef NDEBUG
				std::cout<<"VECTOR WITHOUT DUPLICATES"<<std::endl;	
				for(const BGLgeom::Int_layer<Graph> & I: intvect)
					std::cout<<I<<std::endl;
			#endif
									
			// first, we resolve the connection between source and first intersection
			refine_graph(G, src, tgt, e_prop, intvect[0], next_src); //connect src to the first intersection
			
			for(int i=1; i<intvect.size(); ++i){ //here we resolve all the intermediate intersections (if there is only one intrsection this loop is not executed)
				current_src = next_src;
				refine_graph(G, current_src, tgt, e_prop, intvect[i], next_src);	// graph, current source, current intersection object					
			};
			
			// Finally we connect the last intersection point with the target, but only if their vertex_desciptors do not coincide
			current_src = next_src;
			if(!(current_src == tgt));
				e = new_fracture(current_src, tgt, e_prop, G);										
		}// else 
	} //while(!eof)
	
	count_v = 0;
	count_e = 0;
	
	Vertex_it v_it, v_end;
	
	for (std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it) //for loop on all the edges in the graph
		++count_e;
		
	for (std::tie(v_it, v_end) = boost::vertices(G); v_it != v_end; ++v_it) //for loop on all the vertices in the graph
		++count_v;
	
	cout<<endl;
	std::cout << "________________ FINAL SETTING GRAPH 1 ________________"<<std::endl;
	std::cout<<count_v<<" vertices and " <<count_e<<" edges"<<std::endl;
	
	std::string filename1_out("../data/G1.vtp");
	
	writer_vtp<Graph,2> W1(filename1_out.c_str());
	W1.export_vtp(G);
	
	cout<<endl;
	cout<<endl;
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	cout<<endl;
	cout<<"________________ GRAPH 2 ________________"<<endl;
	// Reading file 2
	
	count_e = 0; 
	count_v = 0;
	
	frac_num = 0;
	
	while(!R2.is_eof()){
		++frac_num;
		std::cout<<"-------------------READING LINE "<<frac_num<<"-------------------"<<std::endl;

		cout << "Current situation: ";		
		count_v = 0; 
		Vertex_it vv,vvend;
		for(std::tie(vv,vvend) = boost::vertices(G2); vv!=vvend; ++vv) 
			++count_v;
		std::cout<<count_v<<" vertices, ";
	
		R2.get_data();
		src_prop = R2.get_source_data();
		tgt_prop = R2.get_target_data();
		e_prop = R2.get_edge_data();
		e_prop.index = frac_num; // set the index in the edge_property equal to frac_num
		
		Vertex_d src = new_vertex(src_prop, G2, true); //inserts a new vertex and returns the vertex desc or simply returns an already existing vertex desc in the case the coordinates match
		Vertex_d tgt = new_vertex(tgt_prop, G2, true);

		edge_alone = true;
		intvect.clear(); // at each line I have to start with the empty vector
		
		count_e = 0;

		// create the linear edge having source in src and target in tgt: this will be the input for the function compute_intersection
		const line L(G2[src].coordinates, G2[tgt].coordinates);
		
		
		for (std::tie(e_it, e_end) = boost::edges(G2); e_it != e_end; ++e_it){ //for loop on al the edges already in the graph
			++count_e;
			
			intobj_tmp = BGLgeom::compute_intersection(G2[*e_it].geometry, L); //in intersection info about the type of intersection are stored
			if(intobj_tmp.intersect == true){
				BGLgeom::Int_layer<Graph> intobj(intobj_tmp, *e_it); // this is the structure I'm going to use
				edge_alone = false;
				intvect.push_back(intobj);
			}	
		} //for
		
		std::cout<<count_e<<" edges."<<std::endl;
		
		if(edge_alone){
			std::cout<<"This fracture does no intersect with any of the other fractures already in the graph."<<std::endl;
			e = new_fracture(src,tgt,e_prop,G2);
		}
		else{ // there is at least one intersection		
			Vertex_d next_src;
			Vertex_d current_src = src;
			
			//order intvect in decreasing or decreasing order based on the relative position of src and tgt 
			if(G2[src].coordinates < G2[tgt].coordinates){
				std::sort(intvect.begin(), intvect.end(), asc_order);
			}
			else{
				std::sort(intvect.begin(), intvect.end(), desc_order);
			}

			#ifndef NDEBUG
				std::cout<<"ORDERED VECTOR"<<std::endl;	
				for(const BGLgeom::Int_layer<Graph> & I: intvect)
					std::cout<<I<<std::endl;
			#endif
			
			//remove multiple "common_extreme" and "T_old" with same intersection vertex_descriptor: they don't affect the graph so we don't consider them
			auto last = std::unique(intvect.begin(), intvect.end(), is_duplicate);
			intvect.erase(last, intvect.end());
			
			#ifndef NDEBUG
				std::cout<<"VECTOR WITHOUT DUPLICATES"<<std::endl;	
				for(const BGLgeom::Int_layer<Graph> & I: intvect)
					std::cout<<I<<std::endl;
			#endif
									
			// first, we resolve the connection between source and first intersection
			refine_graph(G2, src, tgt, e_prop, intvect[0], next_src); //connect src to the first intersection
			
			for(int i=1; i<intvect.size(); ++i){ //here we resolve all the intermediate intersections (if there is only one intrsection this loop is not executed)
				current_src = next_src;
				refine_graph(G2, current_src, tgt, e_prop, intvect[i], next_src);	// graph, current source, current intersection object					
			};
			
			// Finally we connect the last intersection point with the target, but only if their vertex_desciptors do not coincide
			current_src = next_src;
			if(!(current_src == tgt));
				e = new_fracture(current_src, tgt, e_prop, G2);										
		}// else 
	} //while(!eof)
	
	count_v = 0;
	count_e = 0;
	
	for (std::tie(e_it, e_end) = boost::edges(G2); e_it != e_end; ++e_it) //for loop on all the edges in the graph
		++count_e;
		
	for (std::tie(v_it, v_end) = boost::vertices(G2); v_it != v_end; ++v_it) //for loop on all the vertices already in the graph
		++count_v;
	
	std::cout << "________________ FINAL SETTING GRAPH 2 ________________"<<std::endl;
	std::cout<<count_v<<" vertices and " <<count_e<<" edges"<<std::endl;
	
	std::string filename2_out("../data/G2.vtp");
	
	writer_vtp<Graph,2> W2(filename2_out.c_str());
	W2.export_vtp(G2);	
	
	return 0;
};
