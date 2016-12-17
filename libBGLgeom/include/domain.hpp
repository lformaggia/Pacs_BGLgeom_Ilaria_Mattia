#ifndef HH_DOMAIN_HH
#define HH_DOMAIN_HH

#include <cmath>

namespace BGLgeom{

/*!
	@brief Defines a 1D domain.
*/
class Domain1D {
	public:
		//!Constructor. Default creates (0,1)
		explicit Domain1D(double const & a=0., double const & b=1.) : M_a(a), M_b(b) {};
		/*!
			\defgroup Accessor Accessing elements
		@{ */
		double left() const { return M_a; }
		double right() const { return M_b; }
		double & left() { return M_a; }
		double & right() { return M_b; }
		double length() const { return std::abs(M_b-M_a); }
		/*! @}*/
	private:
		//! Right extreme
		double M_a;
		//! Left extreme
		double M_b;
};	//Domain1D

}	//BGLgeom
#endif
