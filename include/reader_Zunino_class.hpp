/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
* \file reader_Zunino_class.hpp
* \author Ilaria Speranza & Mattia Tantardini
* \date Sep 14, 2016
* \brief 
*/

#ifndef HH_READER_ZUNINO_CLASS_HH
#define HH_READER_ZUNINO_CLASS_HH

#include "generic_point.hpp"

class reader_Zunino final: public reader_base_class {
	private:
		point<3> SRC,TGT; 												// they will store vertex coordinates	  
		unsigned int src, tgt; 													// they will read source and target of each edge
		unsigned int edge_num;													// dummy variable;
		double diam, length;													// they will store diameter and length of the edge		
		
	public:
		//! Default constructor:
		reader_Zunino(): reader_base_class(), SRC(), TGT(), src(0), tgt(0), edge_num(0), diam(0.0), length(0.0) {};
		
		//! COnstructor: it assigns value only to the variables in reader_base_class, the others in reader_Zunino are defaulted
		reader_Zunino(unsigned int _num_dummy_lines): 	reader_base_class(_num_dummy_lines),
														SRC(),
														TGT(),
														src(0),
														tgt(0),
														edge_num(0),
														diam(0.0),
														lenght(0.0) {};
		//! Default copy constructor
		reader_Zunino(reader_Zunino const&) = default;
		
		//! Assignment operator
		reader_Zunino & operator=(reader_Zunino const&) = default;
		
		//! Destructor
		virtual ~reader_Zunino(){};
		

};


#endif 	//HH_READER_ZUNINO_CLASS_HH
