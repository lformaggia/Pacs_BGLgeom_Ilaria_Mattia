//Ciao! Sono Mattia, e ora proverò a creare un grafo con qualche nodo. Tipo 4 nodi, dai.

#include<iostream>
#include<tuple>
#include<boost/graph/adjacency_list.hpp>
#include<boost/property_map/property_map.hpp>

//Questa per immagazinare la proprietà/caratteristica del nodo nella property map dei vertici.
//Questa è la classe (quindi il tipo) dei dati che voglio associare ai nodi. Posso mettere quante variabili interne voglio.
struct Vertex_Properties{
	public:		//per forza sempre public! Perché ci devo accedere per assegnare i valori! Quindi con struct vado sempre sul sicuro.
	char index;
};

int main(){

	using namespace boost;
	
	using Graph = adjacency_list<vecS,vecS,undirectedS,Vertex_Properties>;	//primo template per struttura che immagazina i vertici, secondo per i lati	
	
	//costruzione manuale:
	//Dichiaro il grafo:
	Graph G;
	//Dichiaro che ci sono dei nodi del tipo giusto per stare nel grafo:
	typename graph_traits<Graph>::vertex_descriptor a,b,c,d;
	//aggiungo i nodi al grafo, poi subito metto al nodo il nome, che è un carattere:
	a = add_vertex(G); G[a].index = 'a';	//Ho usato le bundles properties. Uso il nome del grafo e come chiave il descriptor del nodo o del lato per accedere alla property_map dei nodi o dei lati (ovviamente accedo al campo che ha quella chiave). In questo caso, G[a] è del tipo Vertex_Properties, che ha come variabili interne solo index (ma potrebbe averne molte di più). Come accedo ad una variabile interna ad una classe? con il punto. E poi ci assegno quello che voglio.
	b = add_vertex(G); G[b].index = 'b';
	c = add_vertex(G); G[c].index = 'c';		
	d = add_vertex(G); G[d].index = 'd';
	
	std::cout << "Dovrei aver messo i nodi" << std::endl;
	/*
	//Ora proviamo a fare una prova di lettura:
	std::cout << "Leggo i nomi dei nodi: " << std::endl;
	for(std::size_t i = 0; i < num_vertices(G); i++)
		std::cout << "Nodo: " << G[			*/
	
	std::cout << "Ora mettiamo gli archi" << std::endl;
	
	//creo un edge descriptor per l'arco e un bool perché add_edge funziona così, mi dice se l'arco è effettivamente stato inserito o no.
	typename graph_traits<Graph>::edge_descriptor ed;
	bool inserted;
	
	//creo i lati: (in realtà non è necessario fare tie(,) ... , basta solo add_edge!)
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


