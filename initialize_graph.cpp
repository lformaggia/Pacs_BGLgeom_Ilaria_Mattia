#include <iostream>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <generic_point.hpp>




int main(){

using namespace boost;

using Graph = adjacency_list <vecS, vecS, directedS, point<double,3> >;

Graph G;
graph_traits<Graph>::vertex_descriptor a,b,c,d;
a = add_vertex(G);
b = add_vertex(G);
c = add_vertex(G);
d = add_vertex(G);

//G[a](0,1,2);
G[a] = point<double, 3>(0,1,2);
G[b] = point<double, 3>(1,2,3);
G[c] = point<double, 3>(2,3,4);

//Leggiamo dalla property_map:
graph_traits<Graph>::vertex_iterator v_begin, v_end;
point<double, 3> temp;
for( tie(v_begin, v_end) = vertices(G); v_begin != v_end; v_begin++){
	G[*v_begin].print();
	
	/*temp = get(Coor, *v_begin );
	temp.print();*/
}

std::cout << std::endl << std::endl;
/*
add_edge(a, b, Weight(3.5), G);
add_edge(b, c, Weight(5.0), G);

property_map<Graph, edge_weight_t>::type Pesi = get(edge_weight, G);
//Leggiamo i pesi:
graph_traits<Graph>::edge_iterator e_begin, e_end;
for(tie(e_begin, e_end) = edges(G); e_begin != e_end; e_begin++){
	double p = get(Pesi, *e_begin);
	std::cout << "Peso arco: " << get(Pesi, *e_begin) << std::endl;
}

*/

return 0;
};
