/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	data_structure.hpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Definition of the base vertex and edge geometrical properties
	
	This file defines two data structure: one for the vertex properties, 
	and one for the edge properties. Each one of them contains the minimal 
	data we thought were necessary for a graph in a geometrical context, 
	and for a graph that will be mainly used to generate meshes on which 
	FEM problems will run. Therefore the vertex property contains a struct 
	to handle boundary condition that may be present on the vertex (we 
	also considered the case the user need more than one boundary condition)
	and the edge property contains a struct to handle creation of meshes. 
	The truly geometric properties are: the coordinates in the space for 
	the vertex, and a class handling the geometry of the edge for the edges. 
	We also provide both properties with a label and an index record, to 
	allow the user to index or label vertices and edge as he/she prefers.
	
	@remark	The two struct are thought to be base class. For the applications, 
			it may be necessary to include to vertex and edge properties 
			more data and functionalities. To do this, it is enough to 
			inherit form this base class and define the wider vertex or 
			edge property with all new data, classes, methods that the 
			user wants. Remember only that it is mandatory to provide 
			at least the default constructor for the inherited struct, 
			since the BGL function used in this library to create a new 
			edge in the graph requires to the vertex and edge properties 
			to be default constructible. 	
			
	@todo	The Edge_base_property provided here is a struct templatized 
			on the type of the geometry. This implies that all the edges 
			in the graph will have the same fixed geometry. It may be 
			useful or interesting in term of efficiency to implement also 
			an edge property that can accept for each edge a different 
			geometry. This may be useful for particular applications, 
			for instance a graph where only some edges are required to 
			be modelled with a complex curve (using bspline or generic 
			geometry, for instance),but all the other edge are linear, 
			and so the user could use the ligther and faster linear 
			geometry.
*/

#ifndef HH_DATA_STRUCTURE_HH
#define HH_DATA_STRUCTURE_HH

#include <iostream>
#include <memory>
#include <array>
#include <string>

#include "point.hpp"
#include "boundary_conditions.hpp"
#include "edge_geometry.hpp"
#include "mesh.hpp"

namespace BGLgeom{

/*!
	@brief Minimal data structure for the vertex geometrical properties
	@detail

	@param N Space dimension
	@param num_bc Dimension of the array containing values of the boundary
				condition. It is defaulted to one, since in general multiple
				values are related to user-defined boundary conditions.
*/
template <unsigned int N, unsigned int num_bc = 1>
struct Vertex_base_property{
	//!Definition of some types which may be useful to see outside the struct
	using point_t = typename BGLgeom::point<N>;
	using bc_t = BGLgeom::boundary_condition;	
	
	//! Coordinates of the vertex
	point_t coordinates;
	/*!
		@brief	Boundary conditions on the vertex
		@detail	An array is used to contain all the possible boundary condition.
				This allows the user to set different boundary conditions (which 
				may be needed by its specific problem) in the same vertex
	*/
	std::array<bc_t, num_bc> BC;
	//! A label for the vertex (if needed)
	std::string label;
	//! An index for the vertex (if the user wants to keep track of the vertices)
	int index;
	
	//! Default constructor
	Vertex_base_property() : coordinates(), BC(), label(), index(-1) {};
	
	//! Constructor with only the coordinates
	Vertex_base_property(point_t const& _coordinates) : coordinates(_coordinates),
														BC(),
														label(),
														index(-1) {};
	
	//! Constructor with coordinates and index
	Vertex_base_property(point_t const& _coordinates,
						 unsigned int const& _index) : 	coordinates(_coordinates),
														BC(),
						 								label(),
						 								index(_index) {};
						 								
	//! Constructor with coordinates and label
	Vertex_base_property(point_t const& _coordinates,
						 std::string const& _label) :	coordinates(_coordinates),
														BC(),
														label(_label),					 						
								 						index(-1) {};
						 						
	//! Constructor with coordinates and BC
	Vertex_base_property(point_t const& _coordinates,
						 bc_t const& _BC) :	coordinates(_coordinates),
											BC(_BC),						 						
					 						label(),
					 						index(-1) {};
								
	//! Full constructor
	Vertex_base_property(point_t const& _coordinates,
						 std::array<bc_t,num_bc> const& _BC,
						 std::string const& _label,
						 unsigned int const& _index) : 	coordinates(_coordinates),
						 								BC(_BC),
						 								label(_label),
						 								index(_index) {};

	//! Copy constructor
	Vertex_base_property(Vertex_base_property const&) = default;
	
	//! Move constructor
	Vertex_base_property(Vertex_base_property &&) = default;
	
	//! Assignment operator
	Vertex_base_property & operator=(Vertex_base_property const&) = default;
	
	//! Move assignment
	Vertex_base_property & operator=(Vertex_base_property &&) = default;
	
	//! Destructor
	virtual ~Vertex_base_property() = default;
	
	/*!
		@brief Overload of output operator
		@detail It prints out what is in the vertex property. Shows "NOT DEFINED" if
				the corresponding value was left defaulted
	*/
	friend std::ostream & operator<<(std::ostream & out, Vertex_base_property const& v_prop) {
		out << "Coordinates: " << v_prop.coordinates << std::endl;
		if(v_prop.index != -1)
			out << "Index: " << v_prop.index << std::endl;
		else
			out << "Index: NOT DEFINED" << std::endl;
		if(v_prop.label.empty())
			out << "Label: NOT DEFINED" << std::endl;
		else
			out << "Label: "<< v_prop.label << std::endl;
		if(v_prop.BC.empty())
			out << "Boundary condition(s): NOT DEFINED" << std::endl;
		else{
			out << "Boundary condition(s): " << std::endl;
			for(std::size_t i = 0; i < v_prop.BC.size(); ++i)
				 out << "\t" << i+1 << ") " << v_prop.BC[i] << std::endl; 
		}
		return out;
	}
	
};	//Vertex_base_property


/*!
	@brief	Minimal data structure for the edge geometrical properties in "static" version
	@detail	The type of the geometry of the edge is choose as template parameter.
			This implies that all edges in the graph will have the same geometry
	
	@param Geom Type of the geometry for the edge
	@param dim The dimension of the space
*/
template <typename Geom, unsigned int dim>
struct Edge_base_property{
	//! Definition of some types which may be useful to see outside the struct
	using geom_t = Geom;
	using mesh_t = typename BGLgeom::mesh<dim>;

	//! The class handling the parameterization of the edge
	geom_t geometry;
	//! The struct handling the mesh
	mesh_t mesh;
	//! A label for the vertex (if needed)
	std::string label;
	//! An index for the vertex (if the user wants to keep track of the edge)
	int index;
	
		
	//! Default constructor
	Edge_base_property() : geometry(), mesh(), label(), index(-1) {};
	
	//! Constructor with the geometry
	Edge_base_property(geom_t const& _geometry) :	geometry(_geometry),
													mesh(),
													label(),
													index(-1) {};
	
	//! Constructor with label
	Edge_base_property(std::string const& _label) :	geometry(),
					 								mesh(),
					 								label(_label),
					 								index(-1) {};
								 								
	//! Constructor with index
	Edge_base_property(int const& _index) :	geometry(),
			 								mesh(),
			 								label(),
			 								index(_index) {};
	
	//! Constructor with label and index
	Edge_base_property	(std::string const& _label,
						 int const& _index) :	geometry(),
				 								mesh(),
				 								label(_label),
				 								index(_index) {};
	
	//! Full constructor
	Edge_base_property	(geom_t _geometry, 
						 std::string const& _label, 
						 int const& _index) :	geometry(_geometry), 
						 						mesh(),
						 						label(_label),
						 						index(_index) {};
	
	//! Copy constructor
	Edge_base_property(Edge_base_property const&) = default;
	
	//! Move_constructor
	Edge_base_property(Edge_base_property &&) = default;
	
	//! Destructor
	virtual ~Edge_base_property() = default;
	
	//! Assignment operator
	Edge_base_property & operator=(Edge_base_property const&) = default;
	
	//! Move assignment
	Edge_base_property & operator=(Edge_base_property &&) = default;
	
	//! Helper method to create a mesh using the uniform_mesh() method of struct mesh
	void make_uniform_mesh(unsigned int const& n){
		mesh.uniform_mesh(n, geometry);
	}
	
	//! Helper method to create a mesh using the variable_mesh() method of struct mesh
	void make_variable_mesh(unsigned int const& n, std::function<double(double)> const& spacing_function){
		mesh.variable_mesh(n, spacing_function, geometry);
	}
	
	/*!
		@brief	Overload of operator<<
		@detail It prints out what is in the edge property, explaining the geometry of 
				the edge and its source and target. It shows "NOT DEFINED" if the 
				corresponding value was left defaulted. Concerning the mesh, it returns
				whether a mesh on the edge was computed (and present) or not.
	*/
	friend std::ostream & operator<<(std::ostream & out, Edge_base_property const& e_prop) {
		out << e_prop.geometry << std::endl;
		if(e_prop.index != -1)
			out << "Index: " << e_prop.index << std::endl;
		else
			out << "Index: NOT DEFINED" << std::endl;
		if(e_prop.label.empty())
			out << "Label: NOT DEFINED" << std::endl;
		else
			out << "Label: " << e_prop.label << std::endl;			
		if(e_prop.mesh.empty())
			out << "Mesh: empty" << std::endl;
		else
			out << "Mesh: already computed" <<std::endl;
		return out;
	}
	
};	//Edge_base_property

}	//BGLgeom

#endif	//HH_DATA_STRUCTURE_HH
