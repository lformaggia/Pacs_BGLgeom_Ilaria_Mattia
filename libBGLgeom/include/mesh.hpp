/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file mesh.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief
*/

#ifndef HH_MESH_HH
#define HH_MESH_HH

#include <vector>
#include <functional>
#include <memory>
//#include "domain.hpp"
#include "mesh_generators.hpp"
#include "point.hpp"
//#include "edge_geometry.hpp"

namespace BGLgeom{

/*!
	@brief	A struct to create meshes on graphs' edges
	@detail	This struct will be both the container for the generated mesh
			on the edge and the generator for such a mesh. We declare it as
			a struct because we want public acces to its members.
	
	@param Geom The geometry of the edge
	@param dim The dimension of the space
*/
template <unsigned int dim>
struct mesh{
	// Aliases
	using vect_pts = std::vector<BGLgeom::point<dim>>;
	using vect = std::vector<double>;

	//! The vector containing the real mesh, the coordinates of the points in the space
	vect_pts real;
	/*!
		@brief	The vector containing the parametric mesh, used to compute the real one
		@detail	We keep track of the parametric mesh because the user may want to evaluate
				some other quantities related to the geometry of the edge (first derivative, 
				curvature, ecc) in correspondece of the real points of the mesh
	*/
	vect parametric;
	
	//! Default constructor
	mesh() : real(), parametric() {};
	
	//! Constructor
	mesh(vect_pts _real, vect _parametric) : real(_real), parametric(_parametric) {};
	
	//! Copy constructor
	mesh(mesh const&) = default;
	
	//! Move constructor
	mesh(mesh &&) = default;
	
	//! Assignment operator
	mesh & operator=(mesh const&) = default;
	
	//! Move assignment
	mesh & operator=(mesh &&) = default;
	
	//! Destructor
	virtual ~mesh() = default;
	
	/*! 
		@brief	Creates a uniform parametric mesh, and then evaluates it
		@detail SRC and TGT are included in the mesh points
		
		@param n Number of intervals
		@param eval Function used to evaluate the parametric mesh (it will be one of the geometries)
	*/
	void
	uniform_mesh(unsigned int const& n, std::function<vect_pts(vect const&)> const& eval ) {
		BGLgeom::Mesh1D temp_mesh(BGLgeom::Domain1D(0,1), n);
		parametric = temp_mesh.getMesh();
		real = eval(parametric);
	}
	
	/*! 
		@brief	Creates a non-uniform parametric mesh and then evaluates it
		@detail SRC and TGT are included in the mesh points
		
		@param n Maximum number of intervals
		@param spacing_function Spacing function
		@param eval Function used to evaluate the parametric mesh (it will be one of the geometries)
	*/
	void
	variable_mesh(	unsigned int const& n,
					std::function<double(double)> const& spacing_function,
					std::function<vect_pts(vect const&)> const& eval){
		BGLgeom::Mesh1D temp_mesh(BGLgeom::Domain1D(0,1), n, spacing_function);
		parametric= temp_mesh.getMesh();
		real = eval(parametric);
	}
};	//mesh
 
}	//BGLgeom

#endif	//HH_MESH_HH
