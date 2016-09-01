#ifndef HH__OUTPUT__HH
#define HH__OUTPUT__HH

#include <iostream>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>


//Tutte le funzione e le classi template vanno definite negli header files. Due possibilità:
//	- Faccio un file template.hpp dove metto le dichiarazioni, e un file template_imp.hpp dove metto le definizioni (implementazione). Alla fine del primo file metto #include template_imp.hpp;
//	- Metto tutto nello stesso file, magari per chiarezza comunque prima tutte le dichiarazioni, e poi sotto tutte le definizioni, soprattutto se sono lunghe.
//provo ad adottare questo secondo metodo.

//Per stampare le proprietà di tutti i vertici. Buona per ora solo se c'è un'unica proprietà per vertice.
template <typename Graph_type, typename Name_Vertex_Map>
void print_vertex_properties (Graph_type const& G, Name_Vertex_Map map_name){
	using namespace boost;
	//creo innanzitutto il vertex_iterator che passa in rassegna tutti i nodi:
	using iter_type = typename graph_traits<Graph_type>::vertex_iterator;
	//estraggo i vertici iniziale e finale:
	std::pair<iter_type, iter_type> v_iter = vertices(G);
	//passo in rassegna i nodi e stampo:
	std::size_t count = 0; //o parto da 1?
	for( ; v_iter.first != v_iter.second; v_iter.first++){
		std::cout << "Property of node " << count << ": " << "\t" << get(map_name,*v_iter.first) << std::endl;
		count++;
	}
};


//Per stampare le proprietà di tutti gli archi. Buona per ora solo se c'è un'unica proprietà per vertice.
template <typename Graph_type, typename Name_Edge_Map>
void print_edge_properties (Graph_type const& G, Name_Edge_Map map_name){
	using namespace boost;
	using iter_type = typename graph_traits<Graph_type>::edge_iterator;
	std::pair<iter_type, iter_type> e_iter = edges(G);
	for( ; e_iter.first != e_iter.second; e_iter.first++)
		std::cout << "Property of edge " << *e_iter.first << " is: " << get(map_name, *e_iter.first) << std::endl;
};



#endif
