#ifndef HH_HELPER_FUNCTIONS_HH
#define HH_HELPER_FUNCTIONS_HH

#include<vector>
#include<algorithm>
#include<functional>
#include<cmath>
#include<limits> 

#include"intersections2D_utilities.hpp"
#include"local_data_structure.hpp"
#include"fracture_graph_properties.hpp"
#include"reader_ASCII.hpp"

namespace BGLgeom{
	void create_graph(Graph & G, BGLgeom::reader_ASCII<Fracture::Vertex_prop, Fracture::Edge_prop> & R, double tol = 20*std::numeric_limits<double>::epsilon());
	bool asc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2);
	bool desc_order(BGLgeom::Int_layer<Graph> & I1, BGLgeom::Int_layer<Graph> &I2);
	bool is_duplicate(const BGLgeom::Int_layer<Graph> & I1, const BGLgeom::Int_layer<Graph> & I2);
	void refine_graph(Graph &G, const Vertex_d & src, const Vertex_d & tgt, Fracture::Edge_prop & e_prop, BGLgeom::Int_layer<Graph> & I, Vertex_d & next_src);
	void cut_old_edge(Edge_d &e, const Vertex_d & v, Graph & G);
	void update_edge_properties(Edge_d &e, Fracture::Edge_prop & new_edge_prop, Graph &G);
}

#endif
