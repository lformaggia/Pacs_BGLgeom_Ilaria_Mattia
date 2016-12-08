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

#include <boost/graph/adjacency_list.hpp>

#include <string>

#include "data_structure.hpp"
#include "linear_edge.hpp"
#include "writer_vtk.hpp"

using namespace boost;
using namespace BGLgeom;

int main(int, char *[]){
	using Graph = adjacency_list<vecS,vecS,directedS,Vertex_base_property<3>,Edge_base_property_static<linear_edge<3>,3> >;
	Graph G;
	Edge_desc<Graph> e;
	Vertex_desc<Graph> a,b,c;
	a = add_vertex(G);
	b = add_vertex(G);
	c = add_vertex(G);
		
	G[a].coordinates = point<3>(0,0,0);
	G[b].coordinates = point<3>(1,1,1);
	G[c].coordinates = point<3>(0,1,0);
	
	e = add_edge(a,b,G).first;
	G[e].geometry.set_source(G[a].coordinates);
	G[e].geometry.set_target(G[b].coordinates);

	e = add_edge(a,c,G).first;
	G[e].geometry.set_source(G[a].coordinates);
	G[e].geometry.set_target(G[c].coordinates);		
	
	std::string filename("/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/libBGLgeom/data/out.vtp");
	writer_vtk<Graph,3> W(filename.c_str());
	W.export_vtp(G);
}
