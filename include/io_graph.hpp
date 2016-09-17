/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/

/*!
* \file io_graph.hpp
* \author Ilaria Speranza & Mattia Tantardini
* \date Sep 14, 2016
* \brief Declaration of functions related to input and output of the graph 
*/

#ifndef HH_IO_GRAPH_HH
#define HH_IO_GRAPH_HH

template <typename Graph, typename Point>
void read_zunino_old_format(Graph & G, std::string file_name);

template <typename Graph>
void read_Formaggia_format(Graph & G, std::string file_name);

#include "io_graph_imp.hpp"	

#endif 
