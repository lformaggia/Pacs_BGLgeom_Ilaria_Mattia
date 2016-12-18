#include "parametric_mesh_generator.hpp"

namespace BGLgeom{

void
parametric_mesh_generator::uniform_mesh(unsigned int const& n){
	p_mesh.clear();		//we clear the previous mesh, if present.
	p_mesh.resize(n+1);
	BGLgeom::Mesh1D temp_mesh(BGLgeom::Domain1D(0,1), n);
	p_mesh = temp_mesh.getMesh();
}

void
parametric_mesh_generator::variable_mesh(unsigned int const& n,
										 std::function<double(double const& t)> spacing_function){
	p_mesh.clear();
	p_mesh.resize(n+1);		//but probably it will contain less nodes than n
	BGLgeom::Mesh1D temp_mesh(BGLgeom::Domain1D(0,1), n, spacing_function);
	p_mesh = temp_mesh.getMesh();
	//se sovrascrivo
}		

}	//BGLgeom
