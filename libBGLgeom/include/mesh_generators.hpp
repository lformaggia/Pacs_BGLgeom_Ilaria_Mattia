/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	mesh_generators.hpp
	@author	Luca Formaggia
	@date	Jan, 2017
	@brief	Utilities to generate a 1D mesh
	
	@note	Code from the example of the PACS course
*/

#ifndef HH_MESH_GENERATOR_HH
#define HH_MESH_GENERATOR_HH

#include <vector>
#include <functional>
#include <stdexcept>

namespace BGLgeom{

	/*!
		@brief	Defines a 1D domain
		
		It is the hidden structure that the following utilities will use
		to build the mesh
		@note	Code from the examples of PACS course
	*/
	class Domain1D {
		public:
			//!Constructor. Default creates (0,1)
			explicit Domain1D(double const & a=0., double const & b=1.) : M_a(a), M_b(b) {};
			/*!
				@defgroup Accessor Accessing elements
				@{
			*/
			double left() const { return M_a; }
			double right() const { return M_b; }
			double & left() { return M_a; }
			double & right() { return M_b; }
			double length() const { return std::abs(M_b-M_a); }
			/*! @} */
		
		private:
			//! Right extreme
			double M_a;
			//! Left extreme
			double M_b;
	};	//Domain1D
	
	
	// Alias
	using MeshNodes=std::vector<double>;  
	/*!
		@brief Abstract class for a 1D mesh generator
		
		It is left abstract the calling operator. This one will
		generate the mesh in the inherited class
		@note	Code from the esamples of PACS course
	*/
	class OneDMeshGenerator{
		public:
			OneDMeshGenerator(BGLgeom::Domain1D const & d) : M_domain(d) {};
			virtual MeshNodes operator()() const = 0;
			Domain1D getDomain() const { return M_domain; }
			virtual ~OneDMeshGenerator() = default;
		protected:
			BGLgeom::Domain1D M_domain;
	};	//OneDMeshGenerator	
	
	/*! 
		@defgroup meshers Functors which generates a 1D mesh.
		@{
	*/	
	/*! 
		@brief	Uniform
		
		@note	Code from the examples of the PACS course
	*/
	class Uniform: public OneDMeshGenerator {
		public:
		    /*! 
		    	@brief Constructor
		    	
				@param domain A 1D domain
				@param num_elements Number of elements
		    */
			Uniform(BGLgeom::Domain1D const & domain, unsigned int const & num_elements) :
			    OneDMeshGenerator(domain),M_num_elements(num_elements) {};
			/*! 
				@brief	Calling operator
			    @return MeshNodes The nodes of the mesh
			*/
			MeshNodes operator()() const;
			
		private:
			std::size_t M_num_elements;
	};	//Uniform
  
	/*!
		@brief	Variable mesh size (non uniform) 
		
		The class accepts a spacing function h=h(x) which should return a strictly 
	    positive value for all x in the domain:
	    \f$ h(x)\ge \alpha>0, \quad x\in\Omega \f$. 
	    The mesh is generated so that
	    \f[
	    x_i=\beta\int_0^i h(s)^{-1}ds
	    \f]
	    where \f$\beta\f$
		@note	Code from the examples of the PACS course
	*/
	class VariableSize: public OneDMeshGenerator  {
		public:
			/*!
			    @brief Constructor
			    
			    @param domain A 1D domain
			    @param h Spacing function
			    @param max_num_elements Maximum number of elements
			*/
			VariableSize	(const BGLgeom::Domain1D & domain,
							 std::function<double (double const & x)> const & h,
							 const unsigned int & max_num_elements) :	OneDMeshGenerator(domain),
							 											M_h(h),
							 											M_num_elements(max_num_elements) {};
			/*! 
				@brief	Calling operator
				
			    @return MeshNodes The nodes of the mesh
			*/
			MeshNodes operator()() const;
			
		private:
			//! The spacing function
			std::function<double (double const & x)> const   M_h;
			//! Maximum number of elements
			std::size_t M_num_elements;
	};	//VariableSize
	
	/*! @} */
	

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
			Mesh1D(Domain1D const& d, unsigned int const& n);

			/*!
				@brief Constructor for a variable spaced mesh
				
				@param d A domain
				@param n Max number of intervals (not nodes!)
				@param generatingFunction 	The function providing the desired spacing (density function for the nodes).
			    							Anything that can be converted to a std::function<double (double const & x)>
			*/
			Mesh1D(Domain1D const & d, unsigned int const &n,std::function<double (double const& x)> generatingFunction);

			//! Copy constructor
			Mesh1D(Mesh1D const&) = default;

			//! Move constructor
			Mesh1D(Mesh1D &&) = default;

			//! Assignment operator
			Mesh1D & operator=(Mesh1D const&) = default;

			//! Move assignment
			Mesh1D & operator=(Mesh1D &&) = default;

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
				
				For non uniform meshes, it returns the maximum distance between
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

#endif	//H_MESH_GENERATOR_HH
