/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/

/*!
	@file main_Formaggia.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Source code for fractures application
	@datail
*/

#include <iostream>
#include <string>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

#include "local_data_structure.hpp"
#include "reader_fractures.hpp"
#include "reader_fractures_twolines.hpp"
#include "graph_builder.hpp"
#include "helper_functions.hpp"
#include "writer_vtk.hpp"

using namespace boost;
using namespace BGLgeom;
using namespace Fracture;

int main(){
	
	std::string filename1("/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia/fracture_intersection/data/fractureElevenVF.dat");
	std::string filename2("/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia/fracture_intersection/data/fracture.txt");
	
	Graph G;
	Graph G2;
	reader_fractures R(filename1);
	reader_fractures_twolines R2(filename2);
	
	// Reading the file1
	R.ignore_dummy_lines(7);
	
	Edge_it e_it, e_end;
	Edge_d e; // this will be the descriptor of the new added edge
	BGLgeom::Intersection intobj_tmp; //object of class Intersection
	Fracture::Vertex_prop src_prop, tgt_prop; // here we store source and target properties read from the input file
	Fracture::Edge_prop e_prop;
	bool edge_alone; //if the edge doesn't intersect any of the other edges we need to create it, otherwise it will be created step by step
	
	std::vector<BGLgeom::Int_layer<Graph>> intvect;
	
	int count; //useless. needed only if you want to output something on the screen
	
	unsigned int frac_num = 0;
	
	while(!R.is_eof()){
		++frac_num;
	
		R.get_data_from_line();
		src_prop = R.get_source_data();
		tgt_prop = R.get_target_data();
		e_prop = R.get_edge_data();
		e_prop.index = frac_num; // set the index in the edge_property equal to frac_num
		
		std::cout<<"-------------------STARTING A NEW LINE-------------------"<<std::endl;
		int count_v = 0;
		Vertex_it vv,vvend;
		for(std::tie(vv,vvend) = boost::vertices(G); vv!=vvend; ++vv) ++count_v;
		std::cout<<"There are "<<count_v<<" vertices"<<std::endl;
		
		Vertex_d src = new_vertex(src_prop, G, true); //inserts a new vertex and returns the vertex desc or simply returns an already existing vertex desc in the case the coordinates match
		Vertex_d tgt = new_vertex(tgt_prop, G, true);

		edge_alone = true;
		intvect.clear(); // at each line I have to start with the empty vector
		
		count = 0;

		// create the linear edge having source in src and target in tgt: this will be the input for the function compute_intersection
		const line L(G[src].coordinates, G[tgt].coordinates);
		
		
		for (std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){ //for loop on al the edges already in the graph
			++count;
			
			intobj_tmp = BGLgeom::compute_intersection(G[*e_it].geometry, L); //in intersection info about the type of intersection are stored
			if(intobj_tmp.intersect == true){
				BGLgeom::Int_layer<Graph> intobj(intobj_tmp, *e_it); // this is the structure I'm going to use
				edge_alone = false;
				intvect.push_back(intobj);
			}	
			
			else{
				std::cout<<"NO intersection with fracture number "<< G[*e_it].index <<"("<<G[boost::source(*e_it,G)].coordinates<<";"<<G[boost::target(*e_it,G)].coordinates<<")"<<std::endl;
			}
		} //for
		
		std::cout<<count<<" edges"<<std::endl;
		
		if(edge_alone){
			std::cout<<"edge alone"<<std::endl;
			e = new_fracture(src,tgt,e_prop,G);
		}
		else{ // there is at least one intersection		
			Vertex_d next_src;
			Vertex_d current_src = src;
			
			//order intvect in decreasing or decreasing order based on the relative position of src and tgt and on the first elem in the intersection vector
			if(G[src].coordinates < G[tgt].coordinates){
				std::sort(intvect.begin(), intvect.end(), asc_order);
			}
			else{
				std::sort(intvect.begin(), intvect.end(), desc_order);
			}

			std::cout<<"ORDERED VECTOR"<<std::endl;	
			for(const BGLgeom::Int_layer<Graph> & I: intvect)
			std::cout<<I<<std::endl;

			
			//remove multiple "common_extreme" and "T_old" with same intersection vertex_descriptor: they don't affect the graph so we don't consider them
			auto last = std::unique(intvect.begin(), intvect.end(), is_duplicate);
			intvect.erase(last, intvect.end());
			
			std::cout<<"VECTOR WITHOUT DUPLICATES"<<std::endl;	
			for(const BGLgeom::Int_layer<Graph> & I: intvect)
			std::cout<<I<<std::endl;
			
									
			// first, we resolve the connection between source and first intersection
			refine_graph(G, src, tgt, e_prop, intvect[0], next_src); //collego src alla prima intersection
			
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
	
	int count_vertices = 0;
	int count_edges = 0;
	
	Vertex_it v_it, v_end;
	
	for (std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it) //for loop on al the edges already in the graph
		++count_edges;
		
	for (std::tie(v_it, v_end) = boost::vertices(G); v_it != v_end; ++v_it) //for loop on al the edges already in the graph
		++count_vertices;
	
	std::cout << "________________ FINAL COUNT ________________"<<std::endl;
	std::cout<<count_vertices<<" vertices and " <<count_edges<<" edges"<<std::endl;
	
	std::string filename("/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia/fracture_intersection/data/G1.vtp");
	
	writer_vtk<Graph,2> W1(filename.c_str());
	W1.export_vtp(G);
	
	
	return 0;
};
