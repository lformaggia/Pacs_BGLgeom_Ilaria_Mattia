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
#include <vtkXMLPolyDataWriter.h>

#include "data_structure.hpp"

namespace BGLgeom{

/*!
	@brief Class to export a .vtp file
	@detail This writer outputs a .vtp file containing the graphical representation of the graph 
	
	@pre The graph that has to be exported is expected to have at least all the
		properties defined in data_structure.hpp
		
	@param Graph The type of the graph
	@param Mesh_Container The container that stores the point of the mesh.
			We have to decide whether to keep it Eigen with () to access elements,
			or to use the std containers with [].
*/

using CellArray_ptr = vtkSmartPointer<vtkCellArray>;
using PolyDataWriter_ptr = vtkSmartPointer<vtkXMLPolyDataWriter>;
using PolyData_ptr = vtkSmartPointer<vtkPolyData>;
using Points_ptr = vtkSmartPointer<vtkPoints>;

template<int dim>
inline void insert_point(double const* P, Points_ptr & points){};

// template specializations for dim=2 and dim=3
template<>
inline void insert_point<2>(double const* P, Points_ptr & points){
	// create an array with 3 components: copy the components of v in the first 2 comp and add 0 as z component
	double PP[3];
	PP[0] = P[0];
	PP[1] = P[1];
	PP[2] = 0.0;
	points -> InsertNextPoint(PP);
};

template<>
inline void insert_point<3>(double const* P, Points_ptr & points){
	points -> InsertNextPoint(P);
};

template <typename Graph, unsigned int dim>
class writer_vtk{
	
	public:
	
		//! Default constructor 
		writer_vtk() {
			  // Write the file
  			  writer = PolyDataWriter_ptr::New();
  			  writer_vertices = PolyDataWriter_ptr::New();
  			  
  			  std::string filename("output_vtk.vtp");
  			  writer -> SetFileName(filename.c_str());
  			  
  			  
  			  lines = CellArray_ptr::New();
			  points = Points_ptr::New();
		
			  if(_filename.substr(filename.length()-3, 3) != "vtp")
				 std::cerr << "Warning! The output file does not have 'vtp' extension." << std::endl;
				
			  std::string vertex_string("_vertices");
			  std::string filename_vertices(filename);
			  filename_vertices.insert(filename_vertices.end()-4, vertex_string.begin(), vertex_string.begin()+9);
			
			  writer_vertices -> SetFileName(filename_vertices.c_str());
		}
		
		
		//! Constructor with filename
		writer_vtk(std::string _filename) {
			  // Write the file
  			  writer = PolyDataWriter_ptr::New();
  			  writer -> SetFileName(_filename.c_str());
  			  
  			  lines = CellArray_ptr::New();
			  points = Points_ptr::New();
		
			  if(_filename.substr(_filename.length()-3, 3) != "vtp")
				 std::cerr << "Warning! The output file does not have 'vtp' extension." << std::endl;
				
			  std::string vertex_string("_vertices");
			  std::string filename_vertices(_filename);
			  filename_vertices.insert(filename_vertices.end()-4, vertex_string.begin(), vertex_string.begin()+9);
			
			  writer_vertices -> SetFileName(filename_vertices.c_str());
		}
		
		/*
		//! Destructor
		virtual ~writer_pts(){
			out_file.close();	//necessary?
		}
		*/
		
		//! It exports the graph in .vtp format (compatible with Paraview). If the edge isn't linear we assume there's a mesh defined on it. If there isn't, a simple straight line will be displayed for that edge, indendently of its geometry
		 
		virtual void export_vtp(Graph const& G){
			BGLgeom::Edge_iter<Graph> e_it, e_end;
			for(std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it)
				add_line(*e_it, G);							
			generate_output();
		}	
		
	protected:
		//! The stream associated to the output file
		PolyDataWriter_ptr writer;
		PolyDataWriter_ptr writer_vertices;
		//! Container with all the points
		Points_ptr points;
		Points_ptr vertices;
		//! The array containing all the lines corresponding to the edges
		CellArray_ptr lines;
		
		
		void add_line(BGLgeom::Edge_desc<Graph> const& e, Graph const& G){
			// get source and target
			BGLgeom::Vertex_desc<Graph> src = boost::source(e,G);
			BGLgeom::Vertex_desc<Graph> tgt = boost::target(e,G);
			
			//store point-coordinates in an array
			const double *SRC = G[src].coordinates.data();
			const double *TGT = G[tgt].coordinates.data();
			
			const int ID_start = points->GetNumberOfPoints(); // in points all the points of all the edges are stored so I need to keep track of the ID of my current points
			
			// insert src and tgt in "vertices"
			insert_point<dim>(SRC,vertices);
			insert_point(TGT,vertices);				
			
			if(G[e].mesh.first.empty()){
				//create SRC point
				insert_point<dim>(SRC,points);
				//create TGT point
				insert_point<dim>(TGT,points);		
			}
						
			// if the Mesh is defined create the points of the mesh, incuded source and target
			else{
				double const *P; //it will contain the point coordinates;
				for(const BGLgeom::point<dim>& point: G[e].mesh.first){
					P = point.data();
					insert_point<dim>(P,points);			
 				}
			}
			
			const unsigned int num_points = points->GetNumberOfPoints() - ID_start; // in this way how many points I have this line
			
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
			PolyData_ptr polyData_vertices = PolyData_ptr::New();
 
			// Add the points to the dataset
  			polyData->SetPoints(points);
 
  			// Add the lines to the dataset
  			polyData->SetLines(lines);
  			
  			// Add the vertices to polyData_vertices
  			polyData_vertices -> SetPoints(vertices);
  
			#if VTK_MAJOR_VERSION <= 5
  			writer->SetInput(polyData);
  			writer_vertices -> SetInput(polyData_vertices)
			#else
  			writer->SetInputData(polyData);	
    		writer_vertices -> SetInputData(polyData_vertices)
  			#endif
  			
  			writer -> Write();
  			writer_vertices -> Write();

		}; // generate_output
		
};	//writer_vtp



}	//namespace

#endif	//HH_WRITER_PTS_HH
