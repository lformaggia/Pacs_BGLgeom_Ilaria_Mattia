#ifndef HH_HELPER_FUNCTIONS_HH
#define HH_HELEPR_FUNCTIONS_HH

#include<vector>
#include<algorithm>
#include<functional>

#include"intersections2D_utilities.hpp"
#include"local_data_structure.hpp"


namespace BGLgeom{
	Vertex_d get_vertex_descriptor(const point2 &P, Graph & G);
	void create_graph(Graph & G, const std::vector<line> & Lvect);
	bool asc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2);
	bool desc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2);
	bool is_duplicate(const BGLgeom::Int_layer<Graph> & I1, const BGLgeom::Int_layer<Graph> & I2);
	void refine_graph(Graph &G, const Vertex_d & src, const Vertex_d & tgt, BGLgeom::Int_layer<Graph> & I, Vertex_d & next_src);
	bool same_coordinates(const Vertex_d & v1, const Vertex_d & v2, const Graph & G);
	void add_new_edge(const Vertex_d & src, const Vertex_d & tgt, Graph & G);
	Vertex_d add_new_vertex(const point2 & P, Graph & G);
	void cut_old_edge(Edge_d &e, const Vertex_d & v, Graph & G);
	void update_edge_properties(Edge_d &e, Graph &G);
}

#endif
