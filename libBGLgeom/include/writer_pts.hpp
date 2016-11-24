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
#include <string>
#include <boost/graph/adjacency_list.hpp>

#include "data_structure.hpp"

namespace BGLgeom{

/*!
	@brief Classe to export infos about the graph on a .pts file
	@detail In particular, this writer wants to write the mesh genarated on the
			edges of the graph, in order to be properly used by getFEM code
	
	@pre The graph that has to be exported is expected to have at least all the
		properties defined in data_structure.hpp
	@pre The mesh
*/
template <typename Graph>
class writer_pts{
	public:
		//! Default constructor
		writer_pts(std::string _filename) : out_file() {
			if(_filename.substr(_filename.length()-3, 3) != "pts")
				std::cerr << "Warning! The output file does not have 'pts' extension." << std::endl;
			else
				out_file.open(_filename.c_str());
		}
		
		//! Destructor
		virtual ~writer_pts(){
			out_file.close();	//necessary?
		}
		
		//! It exports the mesh and the info contained in the graph in an pts file
		virtual void export_pts(Graph const& G){
			BGLgeom::Edge_iter<Graph> e_it, e_end;
			BGLgeom::Vertex_desc<Graph> src, tgt;
			out_file << "BEGIN_LIST" << std::endl;
			for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
				out_file << "BEGIN_ARC" << std::endl;
				src = boost::source(*e_it, G);
				tgt = boost::target(*e_it, G);
				out_file << G[src].BC << std::endl;
				out_file << G[tgt].BC << std::endl;
				out_file << "\t" << G[src].coordinates << "\t" << "start" << std::endl;
				out_file << "\t" << G[tgt].coordinates << "\t" << "end" << std::endl;
				for(ciclo sui punti della mesh){
					out_file << /*coordinates*/ << "point" << std::endl;
				}
				out_file << "END_ARC" << std::endl;
			}	//for
			out_file << "END_LIST";
			//out_file.close(); ???
		}	//export_pts
		
	protected:
		//! The stream associated to the output file
		std::ofstream out_file;
};	//writer_pts

}	//geograph

#endif	//HH_WRITER_PTS_HH
