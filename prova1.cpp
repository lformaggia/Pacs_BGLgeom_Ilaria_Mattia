//Ciao! Sono Mattia, e ora proverò a creare un grafo con qualche nodo. Tipo 4 nodi, dai.

#include<iostream>
#include<tuple>			//per tie
#include<boost/graph/adjacency_list.hpp>		//per struttura dati dove immagazinare il grafo
#include<boost/property_map/property_map.hpp>	//per usare funzioni e strutture delle property map

//Questa per immagazinare la proprietà/caratteristica del nodo nella property map dei vertici.
//Questa è la classe (quindi il tipo) dei dati che voglio associare ai nodi. Posso mettere quante variabili interne voglio.
struct Vertex_Labels{
	//public:		//per forza sempre public! Perché ci devo accedere per assegnare i valori! Quindi con struct vado sempre sul sicuro.
	char label;
};

//Classe da usare come bundle property per gli archi:
struct Edge_Weight{
	unsigned int weight;
};

int main(){

	using namespace boost;
	using std::endl;
	
	using Graph = adjacency_list<vecS,vecS,undirectedS,Vertex_Labels,Edge_Weight>;	//primo template per struttura che immagazina i vertici, secondo per i lati	
	
	//costruzione manuale:
	//Dichiaro il grafo:
	Graph G;
	//Dichiaro che ci sono dei nodi del tipo giusto per stare nel grafo:
	typename graph_traits<Graph>::vertex_descriptor a,b,c,d;
	//aggiungo i nodi al grafo, poi subito metto al nodo il nome, che è un carattere:
	a = add_vertex(G); G[a].label = 'a';	//Ho usato le bundles properties. Uso il nome del grafo e come chiave il descriptor del nodo o del lato per accedere alla property_map dei nodi o dei lati (ovviamente accedo al campo che ha quella chiave). In questo caso, G[a] è del tipo Vertex_Properties, che ha come variabili interne solo label (ma potrebbe averne molte di più). Come accedo ad una variabile interna ad una classe? con il punto. E poi ci assegno quello che voglio.
	b = add_vertex(G); G[b].label = 'b';
	c = add_vertex(G); G[c].label = 'c';		
	d = add_vertex(G); G[d].label = 'd';
	
	std::cout << "Dovrei aver messo i nodi" << std::endl << endl;
	
	//Ora proviamo a fare una prova di lettura:
	std::cout << "Leggo i nomi dei nodi: " << std::endl;
	//creo un vertex_iterator per accedere a tutti i nodi:
	using vertex_iter_type = typename graph_traits<Graph>::vertex_iterator;	//typename è opzionale qui, ma forse meglio metterlo per chiarezza.
	std::pair<vertex_iter_type, vertex_iter_type> v_iterator = vertices(G);		//vertices mi dà come output l' "indice" del primo nodo e il totale dei nodi (quindi l'ultimo più uno, ovvero in relazione agli iterator, una posizione dopo l'ultima)
	for( ; v_iterator.first != v_iterator.second; v_iterator.first++)
		std::cout << get(&Vertex_Labels::label, G, *v_iterator.first)  << std::endl;
		//Come ho usato get: get(variabile/proprietà (nella classe che uso come proprietà) che voglio estrarre, grafo (quindi da dove), "numero/indice" del vertice/lato di cui voglio estrarre la variabile/proprietà.
	
	std::cout << endl;
	std::cout << "Ora mettiamo gli archi:" << std::endl;
	
	//creo un edge descriptor per l'arco e un bool perché add_edge funziona così, mi dice se l'arco è effettivamente stato inserito o no.
	typename graph_traits<Graph>::edge_descriptor ed;
	bool inserted;
	
	//creo i lati: (in realtà non è necessario fare tie(,) ... , basta solo add_edge!)
	//Come funziona add_edge: primi due argomenti sono i nodi tra cui voglio mettere un arco, il terzo è il Grafo in cui voglio metterlo.
	tie(ed,inserted) = add_edge(a,b,G);
	G[ed].weight = 15;
	std::cout << "Inserito arco tra a e b: " << inserted << std::endl;
	tie(ed,inserted) = add_edge(a,c,G);
	G[ed].weight = 10;
	std::cout << "Inserito arco tra a e c: " << inserted << std::endl;
	tie(ed,inserted) = add_edge(c,d,G);
	G[ed].weight = 5;
	std::cout << "Inserito arco tra c e d: " << inserted << std::endl;
	tie(ed,inserted) = add_edge(b,d,G);
	G[ed].weight = 1;
	std::cout << "Inserito arco tra b e d: " << inserted << std::endl;
	
	std::cout << endl;
	std::cout << "Proviamo a vedere se riesco a leggere anche i pesi:" << std::endl;
	using edge_iter_type = typename graph_traits<Graph>::edge_iterator;
	std::pair<edge_iter_type, edge_iter_type> e_iterator = edges(G);
	//Stampo:
	for( ; e_iterator.first != e_iterator.second; e_iterator.first++)
		std::cout << "Il peso dell'arco " << *e_iterator.first << " è: " << get(&Edge_Weight::weight, G, *e_iterator.first) << std::endl;
	
	//Proviamo ora ad associare delle caratteristiche agli archi, ad esempio un peso. Devo creare all'inizio una classe da usare come bundle property. Però per farlo ho bisogno l'edge descriptor, e ne ho uno che viene continuamente sovrascritto. Quindi praticamente devo aggiungere il peso uno alla volta non appeno creo l'arco.
	//Come ho fatto qui? Esattamente nella stessa maniera che per i vertici. In particolare però, l'edge_iterator dereferenziato mi dà la paira che contiene l'indice interno a vecS dei nodi, per cui mi dice che quello è ad esempio l'arco (1,2) e non (b,c). Cioè, per stampare l'arco con i nomi che effettivamente gli ho dato, devo fare in un'altra maniera.
	
	
	
	
	return 0;
}


