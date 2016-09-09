#include <iostream>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <generic_point.hpp>


namespace boost {
	enum vertex_coordinates_t {vertex_coordinates = 100};		//cioè è la proprietà numero 100.
	//enum edge_weight_t {edge_weight}		//4 a caso, perché c'è già così sul manuale
	BOOST_INSTALL_PROPERTY(vertex, coordinates);
	//BOOST_INSTALL_PROPERTY(edge, weight);
	};

int main(){

using namespace boost;
using Coordinates = property<vertex_coordinates_t, point<double, 3> >;
using Weight = property<edge_weight_t, double>;
using Graph = adjacency_list <vecS, vecS, directedS, Coordinates, Weight>;

Graph G;
graph_traits<Graph>::vertex_descriptor a,b,c,d;
a = add_vertex(G);
b = add_vertex(G);
c = add_vertex(G);
d = add_vertex(G);

property_map <Graph, vertex_coordinates_t>::type Coor = get(vertex_coordinates, G);
Coor[a] = point<double, 3>(0,1,2);
Coor[b] = point<double, 3>(1,2,3);
Coor[c] = point<double, 3>(2,3,4);

//Leggiamo dalla property_map:
graph_traits<Graph>::vertex_iterator v_begin, v_end;
point<double, 3> temp;
for( tie(v_begin, v_end) = vertices(G); v_begin != v_end; v_begin++){
	temp = get(Coor, *v_begin );
	temp.print();
}

std::cout << std::endl << std::endl;

add_edge(a, b, Weight(3.5), G);
add_edge(b, c, Weight(5.0), G);

property_map<Graph, edge_weight_t>::type Pesi = get(edge_weight, G);
//Leggiamo i pesi:
graph_traits<Graph>::edge_iterator e_begin, e_end;
for(tie(e_begin, e_end) = edges(G); e_begin != e_end; e_begin++){
	double p = get(Pesi, *e_begin);
	std::cout << "Peso arco: " << get(Pesi, *e_begin) << std::endl;
}
return 0;
};
