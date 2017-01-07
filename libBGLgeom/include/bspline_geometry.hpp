/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	bspline_geometry.hpp
	@author	Carlo de Falco
	@date	Dec, 2016
	@brief	Definition of a B-spline geometry for an edge
	@detail 
*/

#ifndef HH_BSPLINE_GEOMETRY_HH
#define HH_BSPLINE_GEOMETRY_HH

#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <functional>
#include "edge_geometry.hpp"
#include "adaptive_quadrature.hpp"

#define tol_dist 1e-8

namespace BGLgeom{

using vect = std::vector<double>;

/*!
	@brief	Find the knot span of the parametric point u.
	@note	This is NOT	Algorithm A2.1 from 'The NURBS BOOK' pg68
			as that algorithm only works for nonperiodic
			knot vectors, nonetheless the results should
			be EXACTLY the same if U is nonperiodic
			
	@param n Number of control points - 1
	@param p Spline degree
	@param t Parametric point
	@param U Knot sequence
	@return	Knot span
	
	@todo	This implementation has linear, rather than log complexity
*/
int findspan (int n, int p, double u, const vect &U);

/*!
	@brief	Compute the functions of the basis
	@note	Algorithm A2.2 from 'The NURBS BOOK' pg70.
	
	@param i Knot span (from findspan())
	@param t Parametric point
	@param p Spline degree
	@param U Knot sequence
	@return	Vector of the functions of the basis (p+1 dimensional)	
*/
void basisfun (int i, double t, int p, const vect &U, vect &N);


/*!
	@brief Class to handle edges described by a bspline
	@detail
	@note	We do not perform checks on the value of the input parameter when evaluating
			the curve since this check is already performed by the code in findspan, 
			which returns an error message and exits.
	@param dim Dimension of the space
	@param deg Degree of the spline
*/
template <int dim = 3, int deg = 3>
class
bspline_geometry : public BGLgeom::edge_geometry<dim> {

	public:
	
		using point = BGLgeom::point<dim>;
		using vect_pts = std::vector<point>;
		
		//! Default constructor
		bspline_geometry() : nc(0), k(), C(), dk(), d2k(), dC(), d2C() {};

		//! Constructor with control points
		bspline_geometry (const vect_pts &C_)
			: nc(C_.size ()), k(make_knots(nc)), C(C_) {

			// construction of spline for the vector of first derivative
			dC.resize (nc-1);
			dk.resize (k.size () - 2, 0.0);
			bspderiv (deg, C, nc, k, k.size (), dC, dk);

			// construction of spline for the vector of second derivative
			d2k.resize (dk.size () - 2, 0.0);    
			d2C.resize (nc-2);
			bspderiv (deg-1, dC, (nc-1), dk, dk.size (), d2C, d2k);
		}

		//! Constructor with control points and knot vector
		bspline_geometry (const vect_pts &C_, const vect &k_)
			: nc(C_.size ()), k(k_), C(C_) {

			// construction of spline for the vector of first derivative
			dC.resize (nc-1);				
			dk.resize (k.size () - 2, 0.0);
			bspderiv (deg, C, dim, nc, k, k.size (), dC, dk);

			// construction of spline for the vector of second derivative
			d2k.resize (dk.size () - 2, 0.0);    
			d2C.resize (nc-2);
			bspderiv (deg-1, dC, dim, (nc-1), dk, dk.size (), d2C, d2k);
		};
		
		//! Copy constructor
		bspline_geometry(bspline_geometry const&) = default;
		
		//! Move constructor
		bspline_geometry(bspline_geometry &&) = default;
		
		//! Destructor
		virtual ~bspline_geometry() = default;
		
		//! Assignment operator
		bspline_geometry & operator=(bspline_geometry const&) = default;
		
		//! Move assignment
		bspline_geometry & operator=(bspline_geometry &&) = default;
		
		/*! 
			@defgroup Building the bspline if default constructed
			@{
		*/
		//! Setting bspline with control points
		void
		set_bspline(vect_pts const& _C){
			C = _C;
			nc = _C.size();
			k = make_knots(nc);

			// construction of spline for the vector of first derivative
			dC.resize (nc-1);
			dk.resize (k.size () - 2, 0.0);
			bspderiv (deg, _C, nc, k, k.size(), dC, dk);
			
			// construction of spline for the vector of second derivative
			d2k.resize (dk.size () - 2, 0.0);    
			d2C.resize (nc-2);
			bspderiv (deg-1, dC, (nc-1), dk, dk.size(), d2C, d2k);
		}
		
		//! Setting bspline with control points and knot vector
		void
		set_bspline(vect_pts const& _C, vect const& _k){
			C = _C;
			nc = _C.size();
			k = _k;

			// construction of spline for the vector of first derivative
			dC.resize (nc-1);				
			dk.resize (k.size () - 2, 0.0);
			bspderiv (deg, _C, dim, nc, k, k.size(), dC, dk);

			// construction of spline for the vector of second derivative
			d2k.resize (dk.size () - 2, 0.0);    
			d2C.resize (nc-2);
			bspderiv (deg-1, dC, dim, (nc-1), dk, dk.size(), d2C, d2k);
		}
		/*! @} */
		
		//! Evaluation of the curve at a given value of the parameter
		point 
		operator() (double const& t) const {
			point P = point::Zero();
			bspeval (deg, C, nc, k, t, P);
			return P;
		};
		
		//! Evaluation in a vector of parameters
		vect_pts
		operator() (vect const& t) const {
			vect_pts PP(dim*t.size());
			bspeval (deg, C, nc, k, t, PP);
			return PP;
		};
		
		//! Evaluation of the first derivative at a given value of the parameter
		point 
		first_der (double const& t) const {
			point P = point::Zero();
			bspeval (deg-1, dC, nc-1, dk, t, P);
			return P;
		};
		
		//! Evaluation in a vector of parameters
		vect_pts
		first_der (vect const& t) const {
			vect_pts PP(t.size());
			bspeval (deg-1, dC, nc-1, dk, t, PP);
			return PP;
		};
		
		//! Evaluation of the second derivative at a given value of the parameter
		point 
		second_der (double const& t) const {
			point P = point::Zero();
			bspeval (deg-2, d2C, nc-2, d2k, t, P);
			return P;
		};
		
		//! Evaluation in a vector of parameters
		vect_pts
		second_der (vect const& t) const	{
			vect_pts PP(t.size());
			bspeval (deg-2, d2C, nc-2, d2k, t, PP);
			return PP;
		};
		
		//! Evaluation of the curvilinear abscissa at a given value of the parameter
		double
		curv_abs (double const& t) const {
			double retval =
			    BGLgeom::integrate ([&] (double u) {return velocity (u);}, 0, t);
			return retval;
		};
		
		//! Evaluation in a vector of parameters
		vect
		curv_abs (vect const& t) const {
			vect retval (t.size (), .0);
			for (std::size_t ii = 1; ii < t.size (); ++ii)
			    retval[ii] = retval[ii-1] +
			    BGLgeom::integrate ([&] (double u) {return velocity (u);}, t[ii-1], t[ii]);
			return retval;
		};
		
		//! Evaluation of the curvature at a given value of the parameter
		double
		curvature(double const& t) const {
			if( (this->first_der(t)).norm() < tol_dist )
				return 0; // otherwise at the denominator I will have zero or very close to it
			double numerator;
			if(dim == 3){
				// explicit computation of the determinant
				Eigen::Matrix<double,1,3> tmp( (this->first_der(t)(0,0) * this->second_der(t)(0,1) -
							 					this->first_der(t)(0,1) * this->second_der(t)(0,0)),
							 				   (this->first_der(t)(0,2) * this->second_der(t)(0,0) -
							 					this->first_der(t)(0,0) * this->second_der(t)(0,2)),
							 				   (this->first_der(t)(0,1) * this->second_der(t)(0,2) -
							 					this->first_der(t)(0,2) * this->second_der(t)(0,1))   );
				numerator = tmp.norm();
			} else {	//dim == 2
				numerator = std::abs(this->first_der(t)(0,0) * this->second_der(t)(0,1) - 
							 		 this->first_der(t)(0,1) * this->second_der(t)(0,0));
			}
			double denominator( ((this->first_der(t)).norm()) *
								((this->first_der(t)).norm()) *
								((this->first_der(t)).norm()) );
			return numerator/denominator;
		}
		
		//! Evaluation in a vector of parameters
		vect
		curvature(vect const& t) const {
			vect C(t.size());
			for(std::size_t i = 0; i < t.size(); ++i)
				C[i] = this->curvature(t[i]);
			return C;
		}
		
		/*!
			@brief	Overload of operator<<
			@detail It only tells the coordinates of its extremes. May be useful for debugging
		*/
		friend std::ostream & operator<<(std::ostream & out, bspline_geometry<dim,deg> const& edge) {
			out << "(bspline)\tSource: " << edge(0) << ", Target: " << edge(1);
			return out;
		}

	private:
		//! Number of control points
		int nc;
		//! Knot vector
		vect k;
		//! Vector of the control points
		vect_pts C;
		//! Knot vectors of the first and second derivatives
		vect dk, d2k;
		//! Vectors of the control points of the first and second derivatives
		vect_pts dC, d2C;
		
		//! Norm of the first derivative (to compute curvilinear abscissa)
		double
		velocity (double x)	{
			point tmp = point::Zero();
			bspeval (deg-1, dC, (nc-1), dk, x, tmp);
			return tmp.norm();
		};
		
		//! Creates n knots in the interval [0,1]
		vect 
		make_knots (int n) {
			vect retval;

			for (int ii = 0; ii < deg+1; ++ii)
			    retval.push_back (0.0);

			int nint = n - (deg + 1);
			assert (nint >= 0);

			if (nint > 0){
			    double Dk = 1. / (nint + 1);
			    for (int ii = 0; ii < nint; ++ii)
			        retval.push_back (retval.back () + Dk);
			}

			for (int ii = 0; ii < deg+1; ++ii)
			    retval.push_back (1.0);

			return retval;
		};
		
		/*!
			@brief Compute the first derivative of the curve as a bspline
			
			@param d Degree of the bspline
			@param C Vector of the control points (mc x nc matrix stored as column major)
			@param nc Number of control points
			@param k Knot sequence (nk x 1 vector)
			@param dc vector of the derivative control points (output)
			@param dk Knot sequence of the derivative ((nk-1) x 1 vector) (output)
		*/
		void
		bspderiv (int d, const vect_pts &C, int nc,
		          const vect &k, int nk, vect_pts &dC, vect &dk) const {
			int i, j;
			double tmp;
			for (i = 0; i < nc-1; i++){
			    tmp = d / (k[i+d+1] - k[i+1]);
			    for (j = 0; j < dim; j++)
			    	dC[i](j) = tmp * (C[i+1](j) - C[i](j));
			}
			for (i = 1; i < nk-1; i++)
				dk[i-1] = k[i];
		}	//bspderiv

		/*!
			@brief Evaluates the bspline at the given parametric point
			
			@param d Degree of the bspline
			@param C Vector of the control points
			@param nc Number of control points
			@param k Knot sequence (nk x 1 vector)
			@param t Parametric evaluation point
			@param P Evaluated point (output)
		*/
		void
		bspeval (const int d, const vect_pts &C, const int nc,
		         const vect &k, double t, point &P) const {
			int s, tmp1, ii, i;
			vect N (d+1, 0.0);
			s = findspan (nc-1, d, t, k);
			basisfun (s, t, d, k, N);
			tmp1 = s - d;
			//std::cout << tmp1 << ", " << C.size() << std::endl;
			for (i = 0; i < dim; ++i)
				for (ii = 0; ii <= d; ++ii)
			    	P(i) += N[ii] * C[tmp1+ii](i);
		}	//bspeval

		/*!
			@brief Evaluates the bspline at the given parametric points
			
			@param d Degree of the bspline
			@param C Vector of the control points
			@param nc Number of control points
			@param k Knot sequence (nk x 1 vector)
			@param t Vector of parametric evaluation point
			@param P Vector of evaluated point (output)
		*/
		void
		bspeval (const int d, const vect_pts &C, const int nc,
		         const vect &k, const vect &t, vect_pts & P_vect) const {
			int s, tmp1, ii, i_pt;
			vect N (d+1, 0.0);
			auto nt = t.size ();
			P_vect.resize(nt); //reserve nu places in the output vector
			for (std::size_t i_vect = 0; i_vect < nt; ++i_vect){
			    s = findspan (nc-1, d, t[i_vect], k);
			    basisfun (s, t[i_vect], d, k, N);
			    tmp1 = s - d;
			    			    			
			    for (i_pt = 0; i_pt < dim; ++i_pt){
			    	for (ii = 0; ii <= d; ++ii)
			        	P_vect[i_vect](i_pt) += N[ii] * C[tmp1 + ii](i_pt);
			    }
			}	//for
		}	//bspeval

}; // class

} //BGLgeom

#endif	//HH_BSPLINE_GEOMETRY_HH
