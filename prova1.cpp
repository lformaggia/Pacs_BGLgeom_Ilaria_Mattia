//Ciao! Sono Mattia, e ora proverò a creare un grafo con qualche nodo. Tipo 4 nodi, dai.

#include<iostream>
#include<tuple>
#include<boost/graph/adjacency_list.hpp>

int main(){

	using namespace boost;
	
	using Graph = adjacency_list<vecS,vecS,undirectedS>;	//primo template per struttura che immagazina i vertici, secondo per i lati
	
	//costruzione manuale:
	//Dichiaro il grafo:
	Graph G;
	typename graph_traits<Graph>::vertex_descriptor a,b,c,d;
	a = add_vertex(G);
	b = add_vertex(G);
	c = add_vertex(G);
	d = add_vertex(G);
	
	std::cout << "Dovrei aver messo i nodi" << std::endl;
	std::cout << "Ora mettiamo gli archi" << std::endl;
	
	//creo un edge descriptor per l'arco e un bool perché add_edge funziona così, mi dice se l'arco è effettivamente stato inserito o no.
	typename graph_traits<Graph>::edge_descriptor ed;
	bool inserted;
	
	//creo i lati:
	tie(ed,inserted) = add_edge(a,b,G);
	std::cout << "Inserito arco tra a e b: " << inserted << std::endl;
	tie(ed,inserted) = add_edge(a,c,G);
	std::cout << "Inserito arco tra a e c: " << inserted << std::endl;
	tie(ed,inserted) = add_edge(c,d,G);
	std::cout << "Inserito arco tra c e d: " << inserted << std::endl;
	tie(ed,inserted) = add_edge(b,d,G);
	std::cout << "Inserito arco tra b e d: " << inserted << std::endl;
	
	std::cout << "Ora dovrei aver messo anche gli archi" << std::endl;
	
	
	return 0;
}


