/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file point2D.hpp
	@author Ilaria Speranza and Mattia Tantardini
	@date Sept, 2016
	@brief Class to handle 2D points
*/

#ifndef HH_POINT2D_HH
#define HH_POINT2D_HH

#include <array>
#include <iostream>

class point2D {
	public:
		//! Default constructor
		point2D() : coord{0.0,0.0} {};
		
		//! Constructor
		point2D(double const& x, double const& y) : coord{x,y} {};
		
		//! Copy constructor
		point2D(point2D const&) = default;
		
		//! Assignment operator
		point2D & operator=(point2D const&) = default;
		
		//! Overload of assignment operator that may be useful
		point2D & operator=(std::array<double,2> const& P){
			coord[0] = P[0];
			coord[1] = P[1];
		}
		
		//! Get the x coordinate. More readable from the user
		double x(){ return coord[0]; }
		double x() const { return coord[0]; }
		
		//! Get the y coordinate. More readable from the user
		double y(){ return coord[1]; }
		double y() const { return coord[1]; }
		
		//! Overloading of operator[] to access coordinates in a more general way. Useful for algorithms
		double operator[](std::size_t i){ return coord[i]; }
		double operator[](std::size_t i) const { return coord[i]; }
		
		//! Get the whole point, so the container containing the coordinates
		std::array<double,2> get_point(){ return coord; }
		std::array<double,2> get_point() const { return coord; }
				
		//! Set coordinate x of the point, corresponding to coord[0]
		void set_x(double const& x){ coord[0] = x; }
		
		//! Set coordinate y of the point, corresponding to coord[1]
		void set_y(double const& y){ coord[1] = y; }
		
		//! Set both the coordinates, giving two doubles
		void set(double const& x, double const& y){
			coord[0] = x;
			coord[1] = y;
		}
		
		//! Set both the coordinates, giving another point2D
		void set(point2D const& P){
			coord[0] = P.x();
			coord[1] = P.y();
		}
		
		//! Overloading of operator<< to output points
		friend std::ostream & operator<< (std::ostream & out, point2D const& P){
			out << "(" << P.x() << "," << P.y() << ")";
		}
		
		//! Overloading of operator>> to read points
		friend std::istream & operator>> (std::istream & in, point2D & P){
			in >> P.coord[0] >> P.coord[1];
		}
		
		/*!
			 @brief Operator< overloading			 
			 @detail Point1 < Point2 if Point1.x is smaller than Point2.x;
			 		 if they are equal, compare in the same way the y coordinate. 
			 		 (Lexicografic ordering)		
		*/
		bool operator< (point2D const& P2) const {
			//controllo della dimensione dei punti con exception?
								
			for(std::size_t i = 0; i < 2; i++){
				if(coord[i] < P2[i])
					return true;
				else if (coord[i] > P2[i])
					return false;
			}			
			return false;		//if they are equal
		}
				
		/*!
			@brief Operator> overloading			
			@detail It is the negation of operator<
		*/
		bool operator> (point2D const& P2) const {
			return !(*this < P2);
		}		
		
	private:
		std::array<double,2> coord;
};	//point2D

#endif	//HH_POINT2D_HH
