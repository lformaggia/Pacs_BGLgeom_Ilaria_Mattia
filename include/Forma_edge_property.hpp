/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file Forma_edge_property.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief This contains the struct for edge properties that has to be used for Formaggia's example
	@detail 
*/

#ifndef HH_FORMA_EDGE_PROPERTY_HH
#define HH_FORMA_EDGE_PROPERTY_HH

/*!
	@brief This class holds the parametrization of the edge
	
	@detail
*/
class edge_parametrization{};

/*
	@brief Struct for Formaggia's example edge property
	
	@detail 
*/
struct Forma_edge_property_t{
	//! This track which fracture this edge belongs to
	unsigned int frac_num;
	//! It describes the parametrization of the real structure of the edge
	edge_parametrization param;
};		//Forma_vertex_property_t

#endif 	//HH_FORMA_EDGE_PROPERTY_HH
