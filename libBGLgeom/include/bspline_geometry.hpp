/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/* 
   Copyright (C) 2010-2016 Carlo de Falco

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*!
	@file	bspline_geometry.hpp
	@author	Carlo de Falco, Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Definition of a B-spline geometry for an edge
*/

#ifndef HH_BSPLINE_GEOMETRY_HH
#define HH_BSPLINE_GEOMETRY_HH

#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <functional>
#include <Eigen/Dense>
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
int 
findspan (int n, int p, double u, const vect &U);

/*!
	@brief	Compute the functions of the basis
	
	@note	Algorithm A2.2 from 'The NURBS BOOK' pg70.
	
	@param i (Input) Knot span (from findspan())
	@param t (Input) Parametric point
	@param p (Input) Spline degree
	@param U (Input) Knot sequence
	@param N (Output) Vector of the functions of the basis (p+1 dimensional)	
*/
void
basisfun (int i, double t, int p, const vect &U, vect &N);

/*!
	@brief	enum class to distinguish how to use and create the bspline
	
	B-splines can be used both to smooth (and then approximate) given 
	points, or to interpolate them. Providing this enum class, we will
	explicitly ask the user which are his purpouse, since we need in the 
	constructor and in the setting method to know how to use the vector 
	of points passed as argument. Specifying "Approx" we will use that 
	vector as the vector of control points; saying "Interp", instead, 
	we will use it as the vector of points to interpolate.
*/
enum class BSP_type {Approx, Interp};


/*!
	@brief Class to handle edges described by a bspline

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
		
		/*!
			@brief	Constructor
			
			Constructor capable to build the bspline in two different ways: \n
			- using the argument _P as the vector of control points, if the 
				second argument is specified as "BSP_type::Approx"; \n
			- using the argument _P as the vector of points to be interpolated 
				by the bspline, if the second argument is specified as 
				"BSP_type::Interp". \n
			In the first case, the vector is copied in the inner private 
			attribute, the knot vector (uniform) is built, and then control 
			points and knot vector for the first and the second derivative of 
			the bspline are computed and stored. \n
			In the second case, first a uniform knot vector is built and stored. 
			Then the Greville abscissae are computed and they are used as passage 
			condition to evaluate the basis functions, thus building the matrix 
			from which we recover the control points. We then solve, using Eigen 
			methods, in particular the lu solver, the system V*CC=PP, where V is 
			the previous mentioned matrix, CC is the vector of control points we 
			want to find, and PP is the vector of points passed as argument and 
			traslated into an Eigen vector. Finally, we copy the data in CC in 
			the private attribute that stores the control points and we build the 
			bsplines for the first and second derivative, as in the previous case.
			
			@note	The constructor that creates the splines interpolating the 
					points is implemented only for cubic splines (degree = 3)! 
					If used for other degrees, the constructor will show a message 
					error and will abort the program.
			@todo	Implement the constructor that interpolates the given points 
					for any degree of the spline. Another possible improvement 
					concerns the solver used to recover the control points: it 
					may be used some better solver than the lu method.
					
			@param _P The points used either as control points or interpolating ones
			@param _type If "BSP_type::Approx", uses _P as control points; if 
						 "BSP_type::Interp", uses _P as interpolating points
		*/
		bspline_geometry (vect_pts const& _P, BSP_type const& _type){
			if(_type == BSP_type::Approx){
				nc = _P.size();
				k = make_knots(nc);
				C = _P;
				
				// construction of spline for the vector of first derivative
				dC.resize (nc-1);
				dk.resize (k.size () - 2, 0.0);
				bspderiv (deg, C, nc, k, k.size (), dC, dk);

				// construction of spline for the vector of second derivative
				d2k.resize (dk.size () - 2, 0.0);    
				d2C.resize (nc-2);
				bspderiv (deg-1, dC, (nc-1), dk, dk.size (), d2C, d2k);				
			} else {	// _type == BSP_type::Interp
				if(deg != 3){
					std::cerr << "ERROR! BGLgeom::bspline_geometry(): " << std::endl;
					std::cerr <<"\tinterpolating constructor only available for degree = 3 " << 
						"(cubic bsplines)!" << std::endl;
					std::cerr << "Aborting" << std::endl;
					exit(EXIT_FAILURE);
				} else {
					nc = _P.size();
					k = make_knots(nc);
					// Computing greville abscissae
					vect grev = this->grev_abs();
					
					int span;
					// Building vandermonde matrix to recover the control points
					Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> V = 
						Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Zero(nc,nc);
					for(std::size_t i = 0; i < grev.size(); ++i){
						vect N(deg+1);
						span = findspan(nc-1, deg, grev[i], k);
						basisfun(span, grev[i], deg, k, N);
						V(i, span-deg) = N[0];
						V(i, span-deg+1) = N[1];
						V(i, span-deg+2) = N[2];
						V(i, span-deg+3) = N[3];
					}

					// Building an Eigen matrix where to put the known term of the linear system
					Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> PP(nc,dim);
					for(std::size_t i = 0; i < nc; ++i)
						for(std::size_t j = 0; j < dim; ++j)
							PP(i,j) = _P[i](j);
					
					// Solving the linear system V*CC = PP, where CC are the control points we have to find
					Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> CC = V.lu().solve(PP);

					// Copying the founded control points into the private attribute
					C.resize(nc);
					for(std::size_t i = 0; i < nc; ++i)
						for(std::size_t j = 0; j < dim; ++j)
							C[i](j) = CC(i,j);
							
					// construction of spline for the vector of first derivative
					dC.resize (nc-1);
					dk.resize (k.size () - 2, 0.0);
					bspderiv (deg, C, nc, k, k.size (), dC, dk);

					// construction of spline for the vector of second derivative
					d2k.resize (dk.size () - 2, 0.0);    
					d2C.resize (nc-2);
					bspderiv (deg-1, dC, (nc-1), dk, dk.size (), d2C, d2k);
				}
			}		
		}	//constructor

		/*!
			@brief	Constructor with control points and knot vector
			
			@param C_ Vector of the control points
			@param k_ Knot vector
			@param _type Defaulted to BSP_tyep::Approx. The interpolation is not 
						 available if the user provides a knot vector, since the
						 constructor for interpolation creates a uniform knot 
						 vector.
		*/
		bspline_geometry (const vect_pts &C_, const vect &k_, BSP_type const& _type = BSP_type::Approx){
			if(_type == BSP_type::Approx){
				nc = C_.size();
				k = k_;
				C = C_;

				// construction of spline for the vector of first derivative
				dC.resize (nc-1);				
				dk.resize (k.size () - 2, 0.0);
				bspderiv (deg, C, dim, nc, k, k.size (), dC, dk);

				// construction of spline for the vector of second derivative
				d2k.resize (dk.size () - 2, 0.0);    
				d2C.resize (nc-2);
				bspderiv (deg-1, dC, dim, (nc-1), dk, dk.size (), d2C, d2k);
			} else {	// _type == BSP_type::Approx
				std::cerr << "ERROR! BGLgeom::bspline_geometry(): " << std::endl;
				std::cerr << "\tinterpolating constructor with given knot vector not available!" << std::endl; 
				std::cerr << "If you want to interpolate points, use default constructor " <<
							"(it will create a uniform knot vector by default)" << std::endl;
				std::cerr << "Aborting" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
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
			@defgroup bspline_set Building bspline if default constructed
			@{
		*/
		/*!
			@brief	Setting bspline with control points or interpolating points
			
			Works exactly as explained in the constructor documentation.
		*/
		void
		set_bspline(vect_pts const& _P, BSP_type const& _type){
			if(_type == BSP_type::Approx){
				nc = _P.size();
				k = make_knots(nc);
				C = _P;
				
				// construction of spline for the vector of first derivative
				dC.resize (nc-1);
				dk.resize (k.size () - 2, 0.0);
				bspderiv (deg, C, nc, k, k.size (), dC, dk);

				// construction of spline for the vector of second derivative
				d2k.resize (dk.size () - 2, 0.0);    
				d2C.resize (nc-2);
				bspderiv (deg-1, dC, (nc-1), dk, dk.size (), d2C, d2k);				
			} else {	// _type == BSP_type::Interp
				if(deg != 3){
					std::cerr << "ERROR! BGLgeom::bspline_geometry(): "
						<< "interpolating constructor only available for degree = 3 "
						<< "(cubic bsplines)!" << std::endl;
					std::cerr << "Aborting" << std::endl;
					exit(EXIT_FAILURE);
				} else {
					nc = _P.size();
					k = make_knots(nc);
					// Computing greville abscissae
					vect grev = this->grev_abs();
					
					int span;
					// Building vandermonde matrix to recover the control points
					Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> V = 
						Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Zero(nc,nc);
					for(std::size_t i = 0; i < grev.size(); ++i){
						vect N(deg+1);
						span = findspan(nc-1, deg, grev[i], k);
						basisfun(span, grev[i], deg, k, N);
						V(i, span-deg) = N[0];
						V(i, span-deg+1) = N[1];
						V(i, span-deg+2) = N[2];
						V(i, span-deg+3) = N[3];
					}

					// Building an Eigen matrix where to put the known term of the linear system
					Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> PP(nc,dim);
					for(std::size_t i = 0; i < nc; ++i)
						for(std::size_t j = 0; j < dim; ++j)
							PP(i,j) = _P[i](j);
					
					// Solving the linear system V*CC = PP, where CC are the control points we have to find
					Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> CC = V.lu().solve(PP);

					// Copying the founded control points into the private attribute
					C.resize(nc);
					for(std::size_t i = 0; i < nc; ++i)
						for(std::size_t j = 0; j < dim; ++j)
							C[i](j) = CC(i,j);
							
					// construction of spline for the vector of first derivative
					dC.resize (nc-1);
					dk.resize (k.size () - 2, 0.0);
					bspderiv (deg, C, nc, k, k.size (), dC, dk);

					// construction of spline for the vector of second derivative
					d2k.resize (dk.size () - 2, 0.0);    
					d2C.resize (nc-2);
					bspderiv (deg-1, dC, (nc-1), dk, dk.size (), d2C, d2k);
				}
			}
		}	//set_bspline
		
		/*!
			@brief	Setting bspline with control points or and given knot vector
			
			Works exactly as explained in the constructor documentation.
		*/
		void
		set_bspline(vect_pts const& _C, vect const& _k, BSP_type const& _type = BSP_type::Approx){
			if(_type == BSP_type::Approx){
				nc = _C.size();
				k = _k;
				C = _C;

				// construction of spline for the vector of first derivative
				dC.resize (nc-1);				
				dk.resize (k.size () - 2, 0.0);
				bspderiv (deg, C, dim, nc, k, k.size (), dC, dk);

				// construction of spline for the vector of second derivative
				d2k.resize (dk.size () - 2, 0.0);    
				d2C.resize (nc-2);
				bspderiv (deg-1, dC, dim, (nc-1), dk, dk.size (), d2C, d2k);
			} else {	// _type == BSP_type::Approx
				std::cerr << "ERROR! BGLgeom::bspline_geometry(): " << std::endl;
				std::cerr << "\tinterpolating constructor with given knot vector not available!" << std::endl; 
				std::cerr << "If you want to interpolate points, use default constructor " <<
							"(it will create a uniform knot vector by default)" << std::endl;
				std::cerr << "Aborting" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		/*! @} */
		
		/*! 
			@brief Greville abscissae
			
			Computing the greville abscissae associated to this geometry. 
			Used mainly to construct the spline when interpolating points 
			are given. May be useful to know the value of the parameter 
			associated to the interpolating points, if they are points of 
			particular interest
			
			@note	This method requires a valid knot sequence, computed 
					automatically in the constructors or in the set methods.
					Do not use this if the geometry is only default constructed!
			@remark	Available only for cubic splines! (degree = 3). Otherwise, it 
					aborts the program.
			@return	The greville abscissae (values of the parameter crresponding 
					to the interpolated points)
		*/
		vect
		grev_abs(){
			if(deg != 3){
				std::cerr << "ERROR! BGLgeom::bspline_geometry::grev(): "
					  << "\tcomputing of Greville abscissae only available for degree = 3 "
					  << "(cubic bsplines)!" << std::endl;
				std::cerr << "Aborting" << std::endl;
				exit(EXIT_FAILURE);
			} else {
				vect grev(nc);
				for(std::size_t i = 0; i < nc; ++i)
					grev[i] = (k[i+1]+k[i+2]+k[i+3]) / deg;
				return grev;
			}
		}
		
		//! Length of the curve
		double length() { return this->curv_abs(1); }
		double length() const { return this->curv_abs(1); }
		
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
			
			It only tells the coordinates of its extremes. May be useful for debugging
		*/
		friend std::ostream & operator<<(std::ostream & out, bspline_geometry<dim,deg> const& edge) {
			out << "(bspline)\tSource: " << edge(0) << ", Target: " << edge(1);
			return out;
		}

	private:
		//! Number of control points
		unsigned int nc;
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
		velocity (double x) const {
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
			
			@param d (Input) Degree of the bspline
			@param C (Input) Vector of the control points (mc x nc matrix stored as column major)
			@param nc (Input) Number of control points
			@param k (Input) Knot sequence (nk x 1 vector)
			@param dc (Output) Bector of the derivative control points (output)
			@param dk (Output) Knot sequence of the derivative ((nk-1) x 1 vector) (output)
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
			
			@param d (Input) Degree of the bspline
			@param C (Input) Vector of the control points
			@param nc (Input) Number of control points
			@param k (Input) Knot sequence (nk x 1 vector)
			@param t (Input) Parametric evaluation point
			@param P (Output) Evaluated point (output)
		*/
		void
		bspeval (const int d, const vect_pts &C, const int nc,
		         const vect &k, double t, point &P) const {
			int s, tmp1, ii, i;
			vect N (d+1, 0.0);
			s = findspan (nc-1, d, t, k);
			basisfun (s, t, d, k, N);
			tmp1 = s - d;
			for (i = 0; i < dim; ++i)
				for (ii = 0; ii <= d; ++ii)
			    	P(i) += N[ii] * C[tmp1+ii](i);
		}	//bspeval

		/*!
			@brief Evaluates the bspline at the given parametric points
			
			@param d (Input) Degree of the bspline
			@param C (Input) Vector of the control points
			@param nc (Input) Number of control points
			@param k (Input) Knot sequence (nk x 1 vector)
			@param t (Input) Vector of parametric evaluation point
			@param P (Output) Vector of evaluated point (output)
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
