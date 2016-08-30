#include<iostream>
#include<string>		//per le label dei nodi
#include<tuple>			//per tie
#include<boost/graph/adjacency_list.hpp>		//per struttura dati dove immagazinare il grafo
#include<boost/property_map/property_map.hpp>	//per usare funzioni e strutture delle property map

//Questa per immagazinare la proprietà/caratteristica del nodo nella property map dei vertici.
//Questa è la classe (quindi il tipo) dei dati che voglio associare ai nodi. Posso mettere quante variabili interne voglio.
struct Vertex_Labels{
	//public:		//per forza sempre public! Perché ci devo accedere per assegnare i valori! Quindi con struct vado sempre sul sicuro.
	char label;
	//int index;
	//string first_name;
	//string second_name;
	//...
	//posso mettere tutti i campi che voglio!!!!
};

//Classe da usare come bundle property per gli archi:
struct Edge_Weight{
	unsigned int weight;
};

//Ora creo i tipi e le strutture che mi servono per dare le proprietà a vertici e archi usando bene le property maps.
enum vertex_property_t {vertex_label /*, index, first_name, second_name, ... */};  //posso mettere tutti i campi che voglio!!! (vedi sopra)
namespace boost {BOOST_INSTALL_PROPERTY(vertex,property);};
using Label_property = boost::property<vertex_property_t, std::string>;

//Creo le strutture per associare pesi agli archi:
enum edge_property_t {weight2};
namespace boost {BOOST_INSTALL_PROPERTY(edge,property);};
using Edge_weight2 = boost::property<edge_property_t, unsigned int>;

//Nuova struttura per vertici con più proprietà:
enum vertex_fname_t {first_name};
enum vertex_sname_t {second_name};
namespace boost {BOOST_INSTALL_PROPERTY(vertex,fname);
				 BOOST_INSTALL_PROPERTY(vertex,sname);
				 };
using First_Name = boost::property<vertex_fname_t, std::string>;
using All_Name = boost::property<vertex_sname_t, std::string, First_Name>;

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
	
	std::cout << "Dovrei aver messo i nodi" << endl << endl;
	
	//Ora proviamo a fare una prova di lettura:
	std::cout << "Leggo i nomi dei nodi: " << endl;
	//creo un vertex_iterator per accedere a tutti i nodi:
	using vertex_iter_type = typename graph_traits<Graph>::vertex_iterator;	//typename è opzionale qui, ma forse meglio metterlo per chiarezza.
	std::pair<vertex_iter_type, vertex_iter_type> v_iterator = vertices(G);		//vertices mi dà come output l' "indice" del primo nodo e il totale dei nodi (quindi l'ultimo più uno, ovvero in relazione agli iterator, una posizione dopo l'ultima)
	for( ; v_iterator.first != v_iterator.second; v_iterator.first++)
		std::cout << get(&Vertex_Labels::label, G, *v_iterator.first)  << endl;
		//Come ho usato get: get(variabile/proprietà (nella classe che uso come proprietà) che voglio estrarre, grafo (quindi da dove), "numero/indice" del vertice/lato di cui voglio estrarre la variabile/proprietà.
	
	std::cout << endl;
	std::cout << "Ora mettiamo gli archi:" << endl;
	
	//creo un edge descriptor per l'arco e un bool perché add_edge funziona così, mi dice se l'arco è effettivamente stato inserito o no.
	typename graph_traits<Graph>::edge_descriptor ed;
	bool inserted;
	
	//creo i lati: (in realtà non è necessario fare tie(,) ... , basta solo add_edge!)
	//Come funziona add_edge: primi due argomenti sono i nodi tra cui voglio mettere un arco, il terzo è il Grafo in cui voglio metterlo.
	tie(ed,inserted) = add_edge(a,b,G);
	G[ed].weight = 15;
	std::cout << "Inserito arco tra a e b: " << inserted << endl;
	tie(ed,inserted) = add_edge(a,c,G);
	G[ed].weight = 10;
	std::cout << "Inserito arco tra a e c: " << inserted << endl;
	tie(ed,inserted) = add_edge(c,d,G);
	G[ed].weight = 5;
	std::cout << "Inserito arco tra c e d: " << inserted << endl;
	tie(ed,inserted) = add_edge(b,d,G);
	G[ed].weight = 1;
	std::cout << "Inserito arco tra b e d: " << inserted << endl;
	
	std::cout << endl;
	std::cout << "Proviamo a vedere se riesco a leggere anche i pesi:" << endl;
	using edge_iter_type = typename graph_traits<Graph>::edge_iterator;
	std::pair<edge_iter_type, edge_iter_type> e_iterator = edges(G);
	//Stampo:
	for( ; e_iterator.first != e_iterator.second; e_iterator.first++)
		std::cout << "Il peso dell'arco " << *e_iterator.first << " è: " << get(&Edge_Weight::weight, G, *e_iterator.first) << endl;
	
	//Proviamo ora ad associare delle caratteristiche agli archi, ad esempio un peso. Devo creare all'inizio una classe da usare come bundle property. Però per farlo ho bisogno l'edge descriptor, e ne ho uno che viene continuamente sovrascritto. Quindi praticamente devo aggiungere il peso uno alla volta non appeno creo l'arco.
	//Come ho fatto qui? Esattamente nella stessa maniera che per i vertici. In particolare però, l'edge_iterator dereferenziato mi dà la paira che contiene l'indice interno a vecS dei nodi, per cui mi dice che quello è ad esempio l'arco (1,2) e non (b,c). Cioè, per stampare l'arco con i nomi che effettivamente gli ho dato, devo fare in un'altra maniera.
	
	//Proviamo ora ad attaccare a vertici ed archi le proprietà usando bene le property maps. Devo fare ancora un po' di magheggiamenti all'inizio.
	std::cout << endl;
	std::cout << "Proviamo ora a costruire un nuovo grafo in cui associamo ai nodi le proprietà usando bene le property_map:" << endl;
	using Graph2 = adjacency_list<vecS,vecS,undirectedS,Label_property,Edge_weight2>;
	Graph2 G2(4);		//crea un grafo di tipo Graph2 con 4 nodi
	
	//Per ora non metto archi, ma creo la property map dei nomi dei vertici e assegno i nomi:
	property_map<Graph2, vertex_property_t>::type name = get(vertex_label, G2);		//name è un property_map del tipo giusto per essere una property_map<Graph2, vertex_property_t>. Forse ho capito. Allora. vertex_property_t è una enum, quindi può contenere campi/nomi/record diversi. Ora qui io sto dicendo che voglio una property_map associata ai grafi di tipo Graph2 e che abbia come attributi in ogni nodo tutti i campi contenuti nella enum vertex_property_t. name è una property_map del tipo giusto per matchare con quello che viene restituito da get, che in questo caso mi restituisce il campo vertex_label (che è uno dei campi contenuti nella enum vertex_property_t con cui ho costruito la mappa) del grafo G2. Qual è il tipo che viene restituito quindi? Una stringa. Con la specifica ::type, name diventa di tipo stringa.
	//Ora mettiamo i label che vogliamo con put() o con l'operatore []:
	put(name, 0, "Pippo");	//put(nome della property_map in cui voglio inserire il valore, chiave che identifica quel nodo/arco, valore da inserire)
	put(name, 1, "Pluto");
	name[2] = "Paperino";
	name[3] = "Topolino";
	std::cout << "Fatto!" << endl;
	
	std::cout << endl;
	std::cout << "Proviamo a vedere se ha funzionato:" << endl;
	//Tra l'altro, queste 4 righe per stampare gli attributi credo che si possano templatizzare con una comoda funzione facilmente.
	using vertex2_iter_type = typename graph_traits<Graph2>::vertex_iterator;	
	std::pair<vertex2_iter_type, vertex2_iter_type> v2_iterator = vertices(G2);		
	for( ; v2_iterator.first != v2_iterator.second; v2_iterator.first++)
		std::cout << get(name, *v2_iterator.first)  << endl;
		
		
	//Ora provo ad associare dei pesi agli archi. Devo definire la enum all'inizio.
	std::cout << endl;
	std::cout << "Associo proprietà (pesi) agli archi nel nuovo grafo usando la property_map: " << endl;
	//Creo la property_map:
	property_map<Graph2, edge_property_t>::type Weights = get(weight2, G2);
	//Ora dovrei fare put() dei vari pesi negli archi, ma gli archi ancora non ce li ho. Potrei fare in due modi: o così (ma non saprei bene come fare a indicare la chiave di quell'arco), oppure, se non ho ancora aggiunto gli archi, creare gli archi e contemporaneamente associargli i pesi, usando add_edge:
	//Posso usare i numeri per i nodi perché sto utilizzando vecS come container per i nodi:
	graph_traits<Graph2>::edge_descriptor test_edge;
	bool ins;
	tie(test_edge,ins) = add_edge(0,1,Edge_weight2(20),G2);
	add_edge(1,2,Edge_weight2(15),G2);
	add_edge(2,3,Edge_weight2(10),G2);
	add_edge(0,3,Edge_weight2(50),G2);
	std::cout << "Dovrei aver inserito gli archi con i loro pesi. Controlliamo:" << endl;
	using edge2_iter_type = typename graph_traits<Graph2>::edge_iterator;
	std::pair<edge2_iter_type, edge2_iter_type> e2_iterator = edges(G2);
	for( ; e2_iterator.first != e2_iterator.second; e2_iterator.first++){
		graph_traits<Graph2>::vertex_descriptor s = source(*e2_iterator.first, G2);
		graph_traits<Graph2>::vertex_descriptor t = target(*e2_iterator.first, G2);
		std::cout << "Il peso dell'arco da " << get(name, s) << " a " << get(name, t) << " è: " << get(Weights, *e2_iterator.first) << endl;
		}
	std::cout << "Benissimo! Fatto." << endl;
	
	std::cout << endl << "Ora proviamo a cambiare il valore della proprietà di un arco con put. (il problema è: qual è la chiave?)" << endl;
	put(Weights, test_edge, 100);
	std::cout << "Ho cambiato il peso al primo arco. Controlliamo che effettivamente sia così:" << endl;
	std::pair<edge2_iter_type, edge2_iter_type> e3_iterator = edges(G2);
	for( ; e3_iterator.first != e3_iterator.second; e3_iterator.first++){
		graph_traits<Graph2>::vertex_descriptor s = source(*e3_iterator.first, G2);
		graph_traits<Graph2>::vertex_descriptor t = target(*e3_iterator.first, G2);
		std::cout << "Il peso dell'arco da " << get(name, s) << " a " << get(name, t) << " è: " << get(Weights, *e3_iterator.first) << endl;
		}
	std::cout << "Bella lì! Quindi per cambiare le proprietà ad un arco con put, la chiave da usare è semplicemente l'edge_descriptor di quell'arco. Probabilmente va bene anche l'edge_iterator a quell'arco." << endl;
	
	//Ora l'ultima cosa che rimane da fare è provare a creare un grafo con proprietà per vertici e archi che abbiano più di un solo campo. Rifacciamo tutto dall'inizio, creiamo nuove strutture per le proprietà e un nuovo grafo. Facciamo solo per i vertici per ora.
	using Graph3 = adjacency_list<vecS,vecS,undirectedS,All_Name>;
	Graph3 G3(4);		//Grafo di tipo Graph3 con 4 nodi;
	property_map<Graph3, vertex_fname_t>::type f_name = get(first_name, G3);
	property_map<Graph3, vertex_sname_t>::type s_name = get(second_name, G3);
	//Metto nei nodi i dati: nome e cognome:
	f_name[0] = "Anna"; s_name[0] = "Bolena";
	f_name[1] = "Barbara"; s_name[1] = "Santa";
	put(f_name, 2, "Cinzia"); put(s_name, 2, "Fontana");
	f_name[3] = "Donatella"; s_name[3] = "Balla";
	
	//stampiamo:
	using vertex3_iter_type = typename graph_traits<Graph3>::vertex_iterator;	
	std::pair<vertex3_iter_type, vertex3_iter_type> v3_iterator = vertices(G2);
	std::cout << endl;
	std::cout << "Ora creiamo proprietà con più dati per i nodi. Nomi dei nodi:" << endl;	
	for( ; v3_iterator.first != v3_iterator.second; v3_iterator.first++)
		std::cout << get(f_name, *v3_iterator.first) << " " << get(s_name, *v3_iterator.first) << endl;
	std::cout << "Fatto, funziona. Quindi in teoria per mettere più caratteristiche sui nodi o archi bisogna fare le proporty annidate l'una nell'altra, e poi ci sono un po' di sintassi varie per assegnare i valori" << endl;

	
	
	return 0;
}


