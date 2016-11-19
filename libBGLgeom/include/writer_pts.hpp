/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file writer_pts.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Base class to write a .pts file

	@detail
*/

#ifndef HH_WRITER_PTS_HH
#define HH_WRITER_PTS_HH

#include <fstream>
#include <iostream>

namespace geograph{

template <typename Graph>
class writer_pts{
	public:
		
	protected:
		//! The stream associated to the output file
		std::ofstream out_file;
};	//writer_pts

}	//geograph

#endif	//HH_WRITER_PTS_HH
