/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file writer_vtk.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Base class to write a .vtk file

	@detail
*/

#ifndef HH_WRITER_VTK_HH
#define HH_WRITER_VTK_HH

#include <string>
#include <boost/graph/adjacency_list.hpp>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkXMLPolyDataWriter.h>

#include "data_structure.hpp"

namespace BGLgeom{

/*!
	@brief Classe to export a .vtp file
	@detail This writer outputs a .vtp file containing the graphical representation of the graph 
	
	@pre The graph that has to be exported is expected to have at least all the
		properties defined in data_structure.hpp
		
	@param Graph The type of the graph
	@param Mesh_Container The container that stores the point of the mesh.
			We have to decide whether to keep it Eigen with () to access elements,
			or to use the std containers with [].
*/
template <typename Graph, typename Mesh_Container>
class writer_vtk{

	using CellArray_ptr = vtkSmartPointer<vtkCellArray>;
	using PolyDataWriter_ptr = vtkSmartPointer<vtkXMLPolyDataWriter>;
	using PolyData_ptr = vtkSmartPointer<vtkPolyData>;
	using Points_ptr = vtkSmartPointer<vtkPoints> points;
	
	public:
		//! Default constructor
		writer_vtk(std::string _filename) : {
			  // Write the file
  			  writer = PolyDataWriter_ptr::New();
  			  writer -> SetFileName(_filename);
  			  lines = CellArray_ptr::New();
			  points = Points_ptr::New();
		
			if(_filename.substr(_filename.length()-3, 3) != "vtp")
				std::cerr << "Warning! The output file does not have 'vtp' extension." << std::endl;
		}
		
		/*
		//! Destructor
		virtual ~writer_pts(){
			out_file.close();	//necessary?
		}
		*/
		
		//! It exports the graph in .vtp format (compatible with Paraview). If the edge isn't linear we assume there's a mesh defined on it. If there isn't, a simple straight line will be displayed for that edge, indendently of its geometry
		 
		virtual void export_vtp(Graph const& G, Mesh_Container const& M){
			Edge_iter e_it, e_end;
			Vertex_desc src, tgt;

			for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it){
				add_line(*e_it, G);					
			}	//for
			
			generate_output();

		}	
		
	protected:
		//! The stream associated to the output file
		PolyData_ptr writer;
		//! Container with all the points
		Points_ptr points;
		//! The array containing all the lines corresponding to the edges
		CellArray_ptr lines;
		
		
		void add_line(Edge_desc const& e, Graph const& G){
			// get source and target
			Vertex_desc src = boost::source(e,G);
			Vertex_desc tgt = boost::target(e,G);
			
			//store point-coordinates in an array
			double *SRC = G[src].coordinates.data();
			double *TGT = G[tgt].coordinates.data();
			
			const int ID_start = points->GetNumberOfPoints(); // in points all the points of all the edges are stored so I need to keep track of the ID of my current points
			
			//create SRC point
			points -> InsertNextPoint(SRC);
			
			// if the Mesh is defined create the points of the mesh
			if(!G[e].mesh.empty()){
				const int N = G[e].mesh.size();
				double *P; //it will contain the point coordinates;
				for(const point_t& point: G[e].mesh){
					P = point.data();
					points -> InsertNextPoint(P);
 				}
			}
			
			//create TGT point
			points -> InsertNextPoint(TGT);
			
			const int num_points = points->GetNumberOfPoints() - ID_start; // in this way how many points I have this line
			
			// create a new PolyLine
			vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();
  			polyLine ->GetPointIds()->SetNumberOfIds(num_points); // set the PolyLine to contain num_points elements
  			
  			for(unsigned int i = 0; i < num_points; i++)
    		{
    			polyLine->GetPointIds()->SetId(i,i+ID_start);
    		}
 
   			lines->InsertNextCell(polyLine); // insert the new created line to the container of all lines
		}; // add_line
		
		void generate_output(){
			//! Now all the lines have been stored in lines: we create a PolyData object containing points and lines containers, and it will be the writer input argument
			PolyData_ptr polyData = PolyData_ptr::New();
 
			// Add the points to the dataset
  			polyData->SetPoints(points);
 
  			// Add the lines to the dataset
  			polyData->SetLines(lines);
  
			#if VTK_MAJOR_VERSION <= 5
  			writer->SetInput(polyData);
			#else
  			writer->SetInputData(polyData);	
  			
  			writer -> Write();
		}; // generate_output
		
};	//writer_vtp

}	//geograph

#endif	//HH_WRITER_PTS_HH
