#include<vector>

#include"data_structure.hpp"
#include"intersections2D.hpp"
#include"linear_edge.hpp"


using point = BGLgeom::point<2>;
using Graph = boost::adjacency_list<boost::vecS,boost::vecS,boost::directedS,BGLgeom::Vertex_base_property<2>,BGLgeom::Edge_base_property_static<BGLgeom::linear_edge<2>,2> >;

namespace BGLgeom{
	struct Int_layer{
		bool intersect;
		BGLgeom::intersection_type how;
		BGLgeom::Edge_desc<Graph> int_edge;
		std::vector<BGLgeom::point<2> > int_pts;
		
		
		Int_layer(const BGLgeom::Intersection & I, const BGLgeom::Edge_desc<Graph> & e): intersect(I.intersect), how(I.how), int_edge(e){
			for(std::array<double,2> A : I.intersectionPoint){
				BGLgeom::point<2> P(A[0], A[1]);
				int_pts.push_back(P);
			}
		}
	};
} //namespace
