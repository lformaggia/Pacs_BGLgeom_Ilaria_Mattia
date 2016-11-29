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
		
	@param Graph The type of the graph
*/
template <typename Graph>
class writer_pts{
	public:
		//! Default constructor
		writer_pts(std::string _filename) : out_file() {
			if(_filename.substr(_filename.length()-3, 3) != "pts")
				std::cerr << "Warning! The output file does not have 'pts' extension." << std::endl;
			else
				try{
				out_file.open(_filename);
			} catch(std::exception & e) {
				std::cerr << "Error while opening output file. In particular: " << e.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		//! Destructor
		virtual ~writer_pts(){
			out_file.close();	//necessary?
		}
		
		//! It exports the mesh and the info contained in the graph in an pts file
		void
		export_pts(Graph const& G, Mesh_Container const& M){
			BGLgeom::Edge_iter<Graph> e_it, e_end;
			BGLgeom::Vertex_desc<Graph> src, tgt;
			out_file << "BEGIN_LIST" << std::endl;
			for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
				//The previous code has been put in this function
				this->export_edge(*e_it, src, tgt);
			}	//for
			out_file << "END_LIST";
			//out_file.close(); ???
		}	//export_pts
		
	private:
		//! The stream associated to the output file
		std::ofstream out_file;
	
		//! Inner method to output a single edge
		void
		export_edge(BGLgeom::Edge_desc<Graph> const& e, BGLgeom::Vertex_desc<Graph> & src, BGLgeom::Vertex_desc<Graph> & tgt){
			out_file << "BEGIN_ARC" << std::endl;
			src = boost::source(e, G);
			tgt = boost::target(e, G);
			out_file << G[src].BC << std::endl;
			out_file << G[tgt].BC << std::endl;
			out_file << "\t" << G[src].coordinates << "\t" << "start" << std::endl;
			out_file << "\t" << G[tgt].coordinates << "\t" << "end" << std::endl;
			G[*e_it].mesh::iterator m_it = G[e].mesh.begin();
			G[*e_it].mesh::iterator m_end = G[e].mesh.end();
			for( ; m_it != m_end; ++m_it){
				out_file << "\t" << 11 << "\t" << *m_it << "point" << std::endl;	//what does 11 means?
			}
			out_file << "END_ARC" << std::endl;
		}	//export_edge
};	//writer_pts

}	//BGLgeom

#endif	//HH_WRITER_PTS_HH
