#ifndef HH_GENERATOR_HH
#define HH_GENERATOR_HH

#include <vector>
#include <functional>
#include <stdexcept>
#include "domain.hpp"

namespace BGLgeom{
	
	// Alias
	using MeshNodes=std::vector<double>;
  
	/*!
		@brief Abstract class to model the general interface
		@detail It is left abstract the calling operator. This one will
				generate the mesh in the inherited class
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
	@{*/
	
	//! Uniform mesh
	class Uniform: public OneDMeshGenerator {
		public:
		    /*! 
		    	@brief Constructor
				@param domain A 1D domain
				@param num_elements Number of elements
		    */
			Uniform(Geometry::Domain1D const & domain, unsigned int const & num_elements) :
			    OneDMeshGenerator(domain),M_num_elements(num_elements) {};
			/*! 
				@brief Calling operator
			    @return MeshNodes The nodes of the mesh
			*/
			MeshNodes operator()() const;
		private:
			std::size_t M_num_elements;
	};	//Uniform
  
	/*!
		@brief Variable mesh size (non uniform) 
	@detail The class accepts a spacing function h=h(x) which should return a strictly 
		    positive value for all x in the domain:
		    \f$ h(x)\ge \alpha>0, \quad x\in\Omega \f$. 
		    The mesh is generated so that
		    \f[
		    x_i=\beta\int_0^i h(s)^{-1}ds
		    \f]
		    where \f$\beta\f$ 
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
				@brief Calling operator
			    @return MeshNodes The nodes of the mesh
			*/
			MeshNodes operator()() const;
		private:
			//! The spacing function
			std::function<double (double const & x)> const   M_h;
			//! Maximum number of elements
			std::size_t M_num_elements;
	};	//VariableSize
	
	/*! @}*/
	
}	//BGLgeom
#endif
