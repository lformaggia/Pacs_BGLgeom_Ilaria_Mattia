/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file meshable_edge.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Defining functionalities to create parametric mesh on th edges
	@detail 
*/

#ifndef HH_MESHABLE_EDGE_HH
#define HH_MESHABLE_EDGE_HH

#include <vector>
#include <functional>
#include "mesh.hpp"
#include "domain.hpp"

namespace BGLgeom{

/*!
	@brief A struct to create a parametric mesh on an edge
	@detail The mesh will be created on an interval [0,1], and then the implementation
			of the geometry of the edge will take care of translating it in a mesh
			on the 2D or 3D space. \n
			Only default constructor will be implemented, since the class will have
			to be default constructed in the instantiation of the edges by boost::add_edge,
			and since the utilities that actually build the mesh will be called within
			the methods. \n
			This class is thougth to be a base class for the implementation for the different
			geometry of edges.
	@remark This utility can be useful also to generate a set of points (uniforms or
			variables, like a mesh) in which to evaluate some quantities of interest
			on the edge (for instance curvature)
*/
class parametric_mesh_generator{
	public:
		//! Default contructor
		parametric_mesh_generator() : p_mesh() {};
		
		//! Copy constructor
		parametric_mesh_generator(parametric_mesh_generator const&) = default;
		
		//! Assigment operator
		parametric_mesh_generator &
		operator=(parametric_mesh_generator const&) = default;
		
		//! Move assignment
		parametric_mesh_generator &
		operator=(parametric_mesh_generator &&) = default;
		
		//! Destructor
		virtual ~parametric_mesh_generator() = default;
		
		//! Get the mesh
		virtual
		std::vector<double>
		get_mesh() { return p_mesh; }
		
		//! Clear the mesh
		virtual
		void
		clear_mesh() { p_mesh.clear(); }
		
		/*
			@brief Generates a uniform parametric mesh
			@detail
			@note If p_mesh contains a previously computed mesh, it will be discarded
			@note When the parametric mesh will be evaluated, also source and target
					of the edge will be contained in the mesh
			
			@param n Number of intervals
		*/
		virtual
		void
		uniform_mesh(unsigned int const& n); /*{
			p_mesh.clear();		//we clear the previous mesh, if present.
			p_mesh.resize(n+1);
			BGLgeom::Mesh1D temp_mesh(BGLgeom::Domain1D(0,1), n);
			p_mesh = temp_mesh.getMesh();
			
		} */
		
		/*!
			@brief Generates a variable sized (non uniform) parametric mesh
			@detail
			@note If p_mesh contains a previously computed mesh, it will be discarded
			@note When the parametric mesh will be evaluated, also source and target
					of the edge will be contained in the mesh
			
			@param n Max number of intervals (maybe they will be less)
			@param spacing_function A function defining the spacing between the nodes 
		*/
		virtual
		void
		variable_mesh(unsigned int const& n, std::function<double(double const& t)> spacing_function); /*{
			
			p_mesh.clear();
			p_mesh.resize(n+1);		//but probably it will contain less nodes than n
			BGLgeom::Mesh1D temp_mesh(BGLgeom::Domain1D(0,1), n, spacing_function);
			p_mesh = temp_mesh.getMesh();
			//se sovrascrivo
			
		}	*/
		
	protected:
		//! The vector containing the parametric mesh
		std::vector<double> p_mesh;
};	//parametric_mesh_generator

}	//BGLgeom

#endif	//HH_MESHABLE_EDGE_HH
