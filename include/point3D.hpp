/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file point3D.hpp
	@author Ilaria Speranza and Mattia Tantardini
	@date Sept, 2016
	@brief Class to handle 3D points
*/

#ifndef HH_POINT3D_HH
#define HH_POINT3D_HH

#include <array>
#include <iostream>

class point3D {
	public:
		//! Default constructor
		point3D() : coord{0.0,0.0,0.0} {};
		
		//! Constructor
		point3D(double const& x, double const& y, double const& z) : coord{x,y,z} {};
		
		//! Copy constructor
		point3D(point3D const&) = default;
		
		//! Assignment operator
		point3D & operator=(point3D const&) = default;
		
		//! Get the x coordinate. More readable for the user
		double x(){ return coord[0]; }
		double x() const { return coord[0]; }
		
		//! Get the y coordinate. More readable for the user
		double y(){ return coord[1]; }
		double y() const { return coord[1]; }
		
		//! Get the z coordinate. More readable for the user.
		double z(){ return coord[2]; }
		double z() const { return coord[2]; }
		
		//! Overloading of operator[] to access coordinates in a more general way. Useful for algorithms
		double operator[](std::size_t i){ return coord[i]; }
		double operator[](std::size_t i) const { return coord[i]; }
		
		//! Get the whole point, so the container containing the coordinates
		std::array<double,3> get_point(){ return coord; }
		std::array<double,3> get_point() const { return coord; }
		
		//! Set coordinate x of the point, corresponding to coord[0]
		void set_x(double const& x){ coord[0] = x; }
		
		//! Set coordinate y of the point, corresponding to coord[1]
		void set_y(double const& y){ coord[1] = y; }
		
		//! Set coordinate z of the point, corresponding to coord[2]
		void set_z(double const& z){ coord[2] = z; }
		
		//! Set both the coordinates, giving three doubles
		void set(double const& x, double const& y, double const& z){
			coord[0] = x;
			coord[1] = y;
			coord[2] = z;
		}
		
		//! Set both the coordinates, giving another point3D
		void set(point3D const& P){
			coord[0] = P.x();
			coord[1] = P.y();
			coord[2] = P.z();
		}
		
		//! Overloading of operator<< to output points
		friend std::ostream & operator<< (std::ostream & out, point3D const& P){
			out << "(" << P.x() << "," << P.y() << "," << P.z() << ")";
		}
		
		//! Overloading of operator>> to read points
		friend std::istream & operator>> (std::istream & in, point3D & P){
			in >> P.coord[0] >> P.coord[1] >> P.coord[2];
		}
		
		/*!
			 @brief Operator< overloading			 
			 @detail Point1 < Point2 if Point1.x is smaller than Point2.x;
			 		 if they are equal, compare in the same way the y coordinate;
			 		 if also the y coordinates are equal, compare in the same way the z coordinates. 
			 		 (Lexicografic ordering)		
		*/
		bool operator< (point3D const& P2) const {
			//controllo della dimensione dei punti con exception?
								
			for(std::size_t i = 0; i < 3; i++){
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
		bool operator> (point3D const& P2) const {
			return !(*this < P2);
		}		
		
	private:
		std::array<double,3> coord;
};	//point3D

#endif	//HH_POINT3D_HH
