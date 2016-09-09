/*!
	@file vertex3d.hpp
	@brief Defining 3D position property for vertices. \mathcal{N}
	
	@author Ilaria Speranza and Mattia Tantardini

*/

#include<array>
#include<iostream>

//! Class template for storing the vertex position in two or three dimentional space.
template <typename Storage_t, int dim>
class point {
	private:
		std::array<Storage_t, dim> pos;
	
	public:
		//!Default constructor:
		point(const Storage_t& x = 0, const Storage_t& y = 0, const Storage_t& z = 0) : pos{{x,y}}  {
			if(dim == 3)
				pos[2] = z;
			if(dim == 2 && z != 0)
				std::cout << "Warning: third coordinate not used" << std::endl;
			std::cout << "Normal constructor used" << std::endl;
		};
		
		//!Constructor with array: (dovrebbe essere in realtà il copy constructor, quindi possiamo mettere default)
		point(std::array<Storage_t, dim> m_pos) : pos(m_pos) {};
		
		//Destructor: idem come sopra
		
		//!Const constructor:
		
		//!Const copy constructor:
		
		//!Get coordinates:
		Storage_t 
		
};


