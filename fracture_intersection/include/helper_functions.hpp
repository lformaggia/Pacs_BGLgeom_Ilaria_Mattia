#ifndef HH_HELPER_FUNCTIONS_HH
#define HH_HELEPR_FUNCTIONS_HH

#include<vector>
#include<algorithm>
#include<functional>

#include"intersections2D_utilities.hpp"
#include"local_data_structure.hpp"
#include"fracture_graph_properties.hpp"
#include"reader_ASCII.hpp"

namespace BGLgeom{
	Vertex_d get_vertex_descriptor(const Fracture::Vertex_prop & v_prop, Graph & G);
	void create_graph(Graph & G, BGLgeom::reader_ASCII<Fracture::Vertex_prop, Fracture::Edge_prop> & R);
	bool asc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2);
	bool desc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2);
	bool is_duplicate(const BGLgeom::Int_layer<Graph> & I1, const BGLgeom::Int_layer<Graph> & I2);
	void refine_graph(Graph &G, const Vertex_d & src, const Vertex_d & tgt, const Fracture::Edge_prop & e_prop, const unsigned int frac_num, BGLgeom::Int_layer<Graph> & I, Vertex_d & next_src);
	bool same_coordinates(const Vertex_d & v1, const Vertex_d & v2, const Graph & G);
	void add_new_edge(const Vertex_d & src, const Vertex_d & tgt, const Fracture::Edge_prop & e_prop, const unsigned int frac_num, Graph & G);
	Vertex_d add_new_vertex(const point2 & P, Graph & G);
	Vertex_d add_new_vertex(const Fracture::Vertex_prop & V, Graph & G);
	void cut_old_edge(Edge_d &e, const Vertex_d & v, const unsigned int frac_num, Graph & G);
	void update_edge_properties(Edge_d &e, const Fracture::Edge_prop & new_edge_prop, Graph &G);
}

#endif
