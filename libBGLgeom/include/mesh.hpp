#ifndef _HH_MESH_HH
#define _HH_MESH_HH

#include <vector>
#include <functional>
#include "domain.hpp"
#include "meshGenerators.hpp"

namespace Geometry{

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
			@brief Constructor for a variably spaced mesh
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
		//! To use the mesh in range based for loop I need begin()
		std::vector<double>::iterator begin() { return myNodes.begin(); }
		std::vector<double>::const_iterator begin()const { return myNodes.cbegin(); }
		//! To use the mesh in range based for loop I need end()
		std::vector<double>::iterator end() { return myNodes.end();}
		std::vector<double>::const_iterator end() const { return myNodes.cend(); }
		//! I return a copy of the Domain1D (in case it is needed)
		Domain1D domain() const { return myDomain; }
		/*! 
			@brief The mesh size.
			@detail For non uniform meshes, it returns the maximum dinstance between
					two consecutives nodes
		*/
		double h() const;
	private:
		//! The domain
		Domain1D myDomain;
		//! The nodes of the mesh
		std::vector<double> myNodes;
};	//Mesh1D
 
}	//BGLgeom
#endif
