#include"generic_edge_geometry_srctgt.hpp"
#include"generic_point.hpp"

int main(){
	BGLgeom::generic_edge_geometry_srctgt<3> edge;
	
	BGLgeom::point<3> src;
	BGLgeom::point<3> tgt(1);

	edge.set_source(src);
	edge.set_target(tgt);
	
	std::cout<<edge;
	return 0;
}
