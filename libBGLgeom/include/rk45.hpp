/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*
   Copyright (C) 2016 Luca Formaggia
   
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
	@file	rk45.hpp
	@author	Luca Formaggia
	@date	Jan, 2017
	@brief	Declaration of functions to solve ODE systems
	
	This utilities will be used to compute a 1D variable size mesh
*/

#ifndef HH_RK45INTEGRATOR_HH
#define HH_RK45INTEGRATOR_HH

#include <functional>
#include <vector>
#include <cmath>
#include <stdexcept>

namespace ODE {

	//! Defailt max number of iteration
	constexpr std::size_t MAX_STEPS=10000;

	//! Single step of RK
	/*!
		for the solution of \f$ dy/dt=f(y(y),t)

		The routine returns the error as the absoute value of the
		difference between the fifth order and fourth order
		approximation. An optimal step size could be computed by
		postprocessing the error with the formula
		\f[
		h_{opt}=(\frac{\epsilon h}{2error})^{1/4}
		]\f
		where \epsilon is the error control tolerance.

		@param dy  The right hand side function :f(t,y)
		@param y0  Initial value
		@param h   Time step size
		@param error The estimated error
		@return The computed value for y
	*/
	double rk45_step(std::function<double (double const &, double const &)> const & dy,double const & y0,double const & h, double & error);


	//! Adaptive RK
	/*! 
		Integrates /f[ dy/dt(t)=f(t,y(t)) \quad t \in (t0,T] \quad
		y(t0)=y0 /f] controlling the error at t=T. I avoid recursion
		because too inefficient. I do not use the formula for the optimal
		time step. I adopt a simple bisection procedure: the step is
		halved or doubled according to a test on the error. A maximal time step size
		is given to avoid too large time steps, which may cause overlooking variation of f
		with small support.

		@param dy The right hand side f(t,y)
		@param t0 Initial time
		@param T Final time
		@param y0 Initial value
		@param h_initial Initial time step size
		@param h_max maximal Time step size allowed
		@param final_error Desired error at t=T
		@param status: 0=Ok 1=initial step too small; 2 number of steps exceeded; 
		                3 hmin reached, cannot guarantee error
		@return The solution as pairs t,y
	*/
	std::vector<std::pair<double,double>> 
	rk45(std::function<double (double const &, double const &)> const & dy,
		 double const & t0,
		 double const & T,
		 double const & y0,
		 double const & h_initial, 
		 double const & h_max, 
		 double const & final_error,
		 int & status,
		 std::size_t const & maxSteps=MAX_STEPS);
	 
}	//BGLgeom
#endif
