/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
* \file reader_base_class.hpp
* \author Ilaria Speranza & Mattia Tantardini
* \date Sep 14, 2016
* \brief 
*/

#ifndef HH_READER_BASE_CLASS_HH
#define HH_READER_BASE_CLASS_HH

class reader_base_class{
	private:
		
	public:
		
		// la metto qui? Perché fa sempre la stessa cosa, cambia solo la num_dummy_line
		virtual void ignore_dummy_lines(std::ifstream const& file, unsigned int num_dummy_lines = 0){
			std::string dummy;
			for(std::size_t i = 0; i < num_dummy_lines; ++i)
				getline(file, dummy);
		};
		
		virtual void read_data_from_line() = 0;
		
		

};

#endif	//HH_READER_BASE_CLASS_HH
