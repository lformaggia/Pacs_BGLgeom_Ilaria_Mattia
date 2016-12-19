#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include "writer_pts.hpp"
#include "data_structure.hpp"
#include "linear_edge.hpp"

using namespace boost;
using namespace BGLgeom;

int main(){
	using Graph = adjacency_list<vecS,vecS,undirectedS,Vertex_base_property<3,2>,Edge_base_property_static<linear_edge<3>,3> >;
	Graph G;
	
	Vertex_desc<Graph> a,b,c;
	a = add_vertex(G);
	b = add_vertex(G);
	c = add_vertex(G);
	G[a].coordinates = point<3>(0,1,2);
	G[b].coordinates = point<3>(2,3,5);
	G[c].coordinates = point<3>(3,1,6);
	G[a].BC.type[0] = BC_type::DIR;
	G[a].BC.value[0] = 3.5;
	G[a].BC.type[1] = BC_type::DIR;
	G[a].BC.value[1] = 3.7;
	G[c].BC.type[0] = BC_type::NEU;
	G[c].BC.value[0] = 4.5;
	G[c].BC.type[1] = BC_type::INT;
	G[c].BC.value[1] = 3.5;
	
	Edge_desc<Graph> e1,e2;
	e1 = add_edge(a,b,G).first;
	e2 = add_edge(b,c,G).first;
	
	G[e1].geometry.set_source(G[a].coordinates);
	G[e1].geometry.set_target(G[b].coordinates);
	G[e2].geometry.set_source(G[b].coordinates);
	G[e2].geometry.set_target(G[c].coordinates);
	
	//Creating a uniform mesh
	G[e1].geometry.uniform_mesh(20);
	G[e1].mesh = G[e1].geometry( G[e1].geometry.get_mesh() );
	G[e2].geometry.uniform_mesh(30);
	G[e2].mesh = G[e2].geometry( G[e2].geometry.get_mesh() );
	
	std::string filename = "/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/libBGLgeom/data/out_test.pts";
	writer_pts<Graph,3,2> W(filename);
	W.export_pts(G);
	
	return 0;
}
