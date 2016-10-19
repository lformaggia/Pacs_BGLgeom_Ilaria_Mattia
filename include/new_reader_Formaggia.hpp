/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file new_reader_Formaggia.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief 

	@detail 
*/

#ifndef HH_NEW_READER_FORMAGGIA_HH
#define HH_NEW_READER_FORMAGGIA_HH

#include "new_reader_class.hpp"
#include "generic_point.hpp"

//! Struct that contains the data from formaggia's input file
struct data_from_line{
	unsigned int frac_number;
	BGLgeom::point<2> SRC, TGT;
};

//! Class for reading Formaggia's input file
class new_reader_Formaggia : public new_reader_class{
	public:
		//! Overriding of method to read one line of input file. User defined
		virtual void get_data_from_line(data_from_line & D){
			this->iss_line >> D.frac_number >> D.SRC >> D.TGT;
		}
};	//new_reader_Formaggia

#endif	//HH_NEW_READER_FORMAGGIA_HH

