#ifndef _HH_MESH_HH
#define _HH_MESH_HH

#include <vector>
#include <functional>
#include <memory>
#include "domain.hpp"
#include "meshGenerators.hpp"
#include "edge_geometry.hpp"

namespace BGLgeom{

/*!
	@brief	A class to handle the creation of a 1D mesh
	@note	Code from the examples of PACS course
*/
class Mesh1D{

	public:
		using MeshGenerator=std::function<double (double const & x)>;
		
		//! Default constructor is deleted.
		Mesh1D()=default;

		/*! 
			@brief Constructor for an equally spaced mesh
			@param d  A domain
		    @param n  Number of intervals (not nodes!)
		*/
		Mesh1D(Domain1D const & d, unsigned int const &n);

		/*! 
			@brief Constructor for a variable spaced mesh
			@param d  A domain
		    @param n  Max number of intervals (not nodes!)
		    @param generatingFunction The function providing the desired spacing (density function for the nodes).
		    							Anything that can be converted to a std::function<double (double const & x)>
		*/
		Mesh1D(Domain1D const & d, unsigned int const &n,std::function<double (double const & x)> generatingFunction);

		/*! 
			I use the default copy-constructor (C++11)
		    I explicitely report the default even if not strictly necessary, to 
		    recall the reader that C++ provides a synthetic copy constructor.
		*/
		Mesh1D(Mesh1D const&)=default;

		//! Move constructor
		/*
		    Since I store the data in a vector<T> and std vectors implement
		    move semantic I can just use the synthetic move constructor!
		*/
		Mesh1D(Mesh1D&&)=default; //C++11

		//! I use default copy-assignement.
		/*!
		    I explicitely report the default even if not strictly necessary, to 
		    recall the reader that C++ provides a synthetic copy assignment.
		*/
		Mesh1D & operator=(Mesh1D const &)=default;

		//! I use default move-assignement.
		/*!
		    I explicitely report the default even if not strictly necessary, to 
		    recall the reader that C++ provides a synthetic move assignment.
		    Here I use the default because the data is stored in a vector<T> and
		    std vector already support move semantic!
		*/
		Mesh1D & operator=(Mesh1D&&)=default;

		/*! 
			@brief Generate mesh (it will destroy old mesh)
		    @param mg A mesh generator
		*/
		void reset(OneDMeshGenerator const & mg);

		//! Number of nodes.
		unsigned int numNodes() const { return myNodes.size(); }
		//! The i-th node.
		double operator [](int i) const { return myNodes[i]; }
		//! The whole mesh
		std::vector<double> getMesh() { return myNodes; }
		//! To use the mesh in range based for loop I need begin()
		std::vector<double>::iterator begin() { return myNodes.begin(); }
		std::vector<double>::const_iterator begin()const { return myNodes.cbegin(); }
		//! To use the mesh in range based for loop I need end()
		std::vector<double>::iterator end() { return myNodes.end();}
		std::vector<double>::const_iterator end() const { return myNodes.cend(); }
		//! I return a copy of the Domain1D (in case it is needed)
		Domain1D domain() const { return myDomain; }
		/*! 
			@brief	The mesh size.
			@detail For non uniform meshes, it returns the maximum distance between
					two consecutives nodes
		*/
		double h() const;
	private:
		//! The domain
		Domain1D myDomain;
		//! The nodes of the mesh
		std::vector<double> myNodes;
};	//Mesh1D


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
	uniform_mesh(unsigned int const& n, std::function<vect_pts(vect)> const& eval ) {
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
					std::function<vect_pts(vect)> const& eval){
		BGLgeom::Mesh1D temp_mesh(BGLgeom::Domain1D(0,1), n, spacing_function);
		parametric= temp_mesh.getMesh();
		real = eval(parametric);
	}
};	//mesh
 
}	//BGLgeom
#endif
