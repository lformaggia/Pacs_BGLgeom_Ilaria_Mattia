#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include "writer_pts.hpp"
#include "data_structure.hpp"
#include "linear_geometry.hpp"

using namespace boost;
using namespace BGLgeom;

int main(){

	// testing a graph with two boundary conditions in the vertex properties
	using Graph = adjacency_list<vecS,vecS,undirectedS,Vertex_base_property<3,2>,Edge_base_property_static<linear_geometry<3>,3> >;
	Graph G;
	
	Vertex_desc<Graph> a,b,c;
	a = add_vertex(G);
	b = add_vertex(G);
	c = add_vertex(G);
	G[a].coordinates = point<3>(0,1,2);
	G[b].coordinates = point<3>(2,3,5);
	G[c].coordinates = point<3>(3,1,6);
	G[a].BC[0].type = BC_type::DIR;
	G[a].BC[0].value = 3.5;
	G[a].BC[1].type = BC_type::DIR;
	G[a].BC[1].value = 3.7;
	G[c].BC[0].type = BC_type::NEU;
	G[c].BC[0].value = 4.5;
	G[c].BC[1].type = BC_type::INT;
	G[c].BC[1].value = 3.5;
	
	Edge_desc<Graph> e1,e2;
	e1 = add_edge(a,b,G).first;
	e2 = add_edge(b,c,G).first;
	
	G[e1].geometry.set_source(G[a].coordinates);
	G[e1].geometry.set_target(G[b].coordinates);
	G[e2].geometry.set_source(G[b].coordinates);
	G[e2].geometry.set_target(G[c].coordinates);
	
	//Creating a uniform mesh
	G[e1].mesh = G[e1].geometry.uniform_mesh(20);	
	G[e2].mesh = G[e2].geometry.uniform_mesh(20);
	
	std::string filename1 = "/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/libBGLgeom/data/testPTS2bc.pts";
	writer_pts<Graph,3,2> W(filename1);
	W.export_pts(G);
	
	
	// Now testing the writer on the additional infos to print
	using Graph2 = adjacency_list<vecS,vecS,undirectedS,Vertex_base_property<3>,Edge_base_property_static<linear_geometry<3>,3> >;
	Graph2 G2;
	
	Vertex_desc<Graph2> d,e,f;
	d = add_vertex(G2);
	e = add_vertex(G2);
	f = add_vertex(G2);
	G2[d].coordinates = point<3>(0,1,2);
	G2[e].coordinates = point<3>(2,3,5);
	G2[f].coordinates = point<3>(3,1,6);
	G2[d].BC[0].type = BC_type::DIR;
	G2[d].BC[0].value = 3.5;
	G2[f].BC[0].type = BC_type::NEU;
	G2[f].BC[0].value = 4.5;
	
	Edge_desc<Graph2> e3,e4;
	e3 = add_edge(d,e,G2).first;
	e4 = add_edge(e,f,G2).first;
	
	G2[e3].geometry.set_source(G2[d].coordinates);
	G2[e3].geometry.set_target(G2[e].coordinates);
	G2[e4].geometry.set_source(G2[e].coordinates);
	G2[e4].geometry.set_target(G2[f].coordinates);
	G2[e3].index = 1;
	G2[e4].index = 2;
	
	//Creating a uniform mesh
	G2[e3].mesh = G2[e3].geometry.uniform_mesh(20);	
	G2[e4].mesh = G2[e4].geometry.uniform_mesh(20);
	
	std::string filename2 = "/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/libBGLgeom/data/testPTS.pts";
	writer_pts<Graph2,3> W2(filename2);
	W2.export_pts(G2,true);
	
	return 0;
}
