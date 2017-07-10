//
// Created by jhoelccari on 05/05/17.
//
#include <queue>
#include "graph.h"
#include "CFile.h"
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;


template <class Tr>
CGraph<Tr>::CGraph(const string filename, string _user){
    this->user = _user;
    currentId = 0;

    CFile* file = new CFile(filename);
    Node* tmp = new Node(currentId,"master",file);
    tmp->setLabel("root");
    m_nodes.push_back(tmp);

    setCurrentRoot(tmp);     ///guardamos la direccion del nodo centinela como root

    CEdge<traits>* edge = new CEdge<traits>("centinela", nullptr,tmp);
    //tmp->m_edges.push_back(edge);
    CBranch<traits>* branch = new CBranch<traits>("master", edge);
    allEdges.push_back(edge);

    CBranch<traits>* master = new CBranch<traits>("master", edge );
    //branches.push_back(*master);
    currentBranch = master;
    //branches.push_back(*currentBranch);
    /// Creamos la tabla hash
    //hashtable = new CHashTable< CNodeHash<CNode<traits>> , DispersionFunction<string> , ListAdaptor<CNodeHash<CNode<traits>>> >(15);
    hashtable = new CHashTable<traits>(15);

    ///> Creamos el primer estado de la Persistencia
    states.insertNewState(getNodes());
}

template <class Tr>
CGraph<Tr>::~CGraph(){}

template <class Tr>
bool CGraph<Tr>::insert(N& x){
    /// Actualizamos el estado
    currentId +=1;
    /// creamos Node
    CNode<traits>* tmp = new CNode<traits>(currentId,currentBranch->name,&x);

    saveNode(tmp);

    /// creamos Edge
    Node* nodePrev = currentBranch->getEdge()->m_node[1];
    CEdge<traits>* edge = new CEdge<traits>("cambiarEsto",nodePrev,tmp);
    /// guardamos la arista creada
    nodePrev->m_edges.push_back(edge);
    /// actualizamos "currentBranch"
    currentBranch->updateEdge(edge);
    /// guardamos la arista en el nodo creado
    tmp->setPointingEdge(edge);

    allEdges.push_back(edge);

    //CBranch<traits>* branch = new CBranch<traits>("cambiarEstotambien",edge);
    ///*       almacenamos en la hash      *///
    string id = "text"+currentBranch->name+to_string(currentId);
    CNodeHash<traits>* nodeHash = new CNodeHash<traits>(id,tmp,edge);
    hashtable->Insert(*nodeHash);
    cout << ">> Insertando: " << id << endl;
    tmp->setLabel(id);      /// le damos una etiqueta al nodo creado

    /// construimos la persistencia
    string objetivo =currentBranch->getEdge()->getSecondNode()->getLabel();
    buildPersistence(objetivo);

    /// Creamos el grafo de la version actual despues de insertar
    printVersions();

};


/* Ejemplo de una busqueda
 * CNodeHash<traits::GNode>* node= new CNodeHash<traits::GNode>();
 * cout << g->find("textmaster1",*node)<< endl;
 * */
template <class Tr>
bool CGraph<Tr>::find(string finded, HNode* node){
    //cout << "Buscando ... " << finded << endl;
    return hashtable->find(finded,*node);
}

template <class Tr>
bool CGraph<Tr>::remove(string target) {
    /// Actualizamos el estado
    currentId +=1;
    HNode* hnode = new HNode();
    if ( not find(target,hnode))
        return false; ///El nodo no se encontro
    ///borramos
    CEdge<traits>* edge = hnode->getEdge();
    CNode<traits>* prev = edge->m_node[0]; /// en este nodo debemos borrar el "edge" que apuntaba a "target"
    CNode<traits>* next = edge->m_node[1]; /// este es el nodo a eliminar
    cout <<">> Borrando: "<< next->getLabel()<< endl;
    prev->removeEdge(edge);
    next->destroyNode();
    ///Borrar de la tabla hash
    //hashtable->remove(hnode);

    /// Actualizamos CurrentBranch
    currentBranch->updateEdge(prev->getPointingEdge());
    /// construimos la persistencia
    string objetivo =currentBranch->getEdge()->getSecondNode()->getLabel();
    buildPersistence(objetivo);

    /// Creamos el grafo de la version actual despues de remover
    printVersions();
    return true;
}

template <class Tr>
bool CGraph<Tr>::merge(string nodeLabel1, string nodeLabel2) {
    /// Actualizamos el estado
    currentId +=1;
    ///> Buscamos los nodos con las etiquetas nodeLabel1, y nodeLabel2
    HNode* hnode1= new HNode();
    HNode* hnode2= new HNode();
    Node* node1; Node* node2;
    if ( not find(nodeLabel1,hnode1) ) return false; /// Si alguno de los 2 no es un nodo, terminamos la funcion
    if ( not find(nodeLabel2,hnode2) ) return false; /// Si alguno de los 2 no es un nodo, terminamos la funcion
    node1 = hnode1->getNode();
    node2 = hnode2->getNode();

    ///> Recuperamos el texto que se encuenta en node1 y node2
    string n1 = "1";//node1->getFile()->getData();
    string n2 = "2";//node2->getFile()->getData();
    string namefile = node1->getFile()->getNamefile();

    /// Creamos un nuevo archivo
    CFile *m = new CFile(namefile);
    Node* merge = new Node(currentId,currentBranch->getName(), m);
    /// Hacemos un merge con los datos de ambos nodos
    merge->getFile()->merge(n1);
    merge->getFile()->merge(n2);

    ///> Linkeamos el nuevo nodo creado con sus dos padres
    Edge* edge1 = new Edge("dato de arista", node1, merge);
    Edge* edge2 = new Edge("dato de arista", node2, merge);
    node1->saveEdge(edge1);
    node2->saveEdge(edge2);
    ///
    merge->setLabel("merge");
    merge->setPointingEdge(edge1);
    merge->setPointingEdge2(edge2);
    ///guardamos el nodo merge
    //m_nodes.push_back(merge);
    saveNode(merge);
    cout << ">> Merge: (" << node1->getLabel() << " and " << node2->getLabel() <<") -> "<< merge->getLabel() << endl;
    /// Actualizamos currentBranch
    currentBranch->updateEdge(edge1);   // verificar aqui !!!

    ///*       almacenamos en la hash      *///
    string id = "text"+currentBranch->name+to_string(currentId);
    CNodeHash<traits>* nodeMergeHash = new CNodeHash<traits>(id,merge,edge1);
    CNodeHash<traits>* nodeMergeHash2 = new CNodeHash<traits>(id,merge,edge2);
    hashtable->Insert(*nodeMergeHash);
    hashtable->Insert(*nodeMergeHash2);
    cout << ">> Insertando: " << id << endl;
    merge->setLabel(id);      /// le damos una etiqueta al nodo creado

    /// construimos la persistencia
    string objetivo = currentBranch->getEdge()->getSecondNode()->getLabel();
    cout << " antes de persistence; objetivo "<< objetivo << endl;
    buildPersistence(objetivo);
    cout << " despues de persistence" << endl;
    /// Creamos el grafo de la version actual despues de merge
    printVersions();
    return true;
}

template <class Tr>
bool CGraph<Tr>::restore(int state) {
    m_nodes = buildGraph(state);
    printVersions();
    return true;
}

template <class Tr> /// No recuerdo para que hize esta funcion
bool CGraph<Tr>::checkCurrentBranch(CGraph::Edge *b,Branch* tmp) {
    for (int i = 0; i <branches.size() ; ++i) {
        if(branches[i].getEdge() == b){
            tmp = &branches[i] ;
            return true;
        }
    }
    return false;
}

template <class Tr>
bool CGraph<Tr>::isNewBranch(string nameBranch) {
    for (int i = 0; i <branches.size() ; ++i) {
        if(branches[i].getName() == nameBranch)
            return false;
    }
    return true;
}

template <class Tr>
bool CGraph<Tr>::createBranch(string nameBranch){ /// crea un branch y lo almacena en "branches"
    /// primero guardamos el branch actual
    if( isNewBranch(currentBranch->getName()) ){
        branches.push_back(*currentBranch);
    }
    //else cout << "/* ERROR: al crear, branch ya existente*/" << endl;

    /// El nuevo branch apunta a Edge de "currentBranch"
    Edge* newEdge = new Edge(currentBranch->getName(),currentBranch->getEdge()->getFirstNode(), currentBranch->getEdge()->getSecondNode());
    CBranch<traits>* branch = new CBranch<traits>(nameBranch,newEdge);
    //CBranch<traits>* branch = new CBranch<traits>(nameBranch,currentBranch->getEdge());
    cout << "creando branch: "<< branch << endl;
    branches.push_back(*branch);
    return true;
};


template <class Tr>
bool CGraph<Tr>::checkout(string nameBranch) { /// cambia el valor de "currentBranch" a un branch ya creado

    for(int i=0; i< branches.size(); i++){
        if (nameBranch == branches[i].name) {
            cout <<"OK: "<< currentBranch->getName() <<" a " << branches[i].name << endl;
            //cout << currentBranch->getEdge()->getFirstNode()<< " "<< currentBranch->getEdge()->getSecondNode() << " a " << branches[i].edge->getFirstNode()<< " "<< branches[i].edge->getSecondNode() << endl;
            currentBranch = &branches[i];
            return true;
        }
    }
    cout <<"No existe branch " << nameBranch << endl;
    cout <<"Regresando a branch: "<< branches[0].name << endl;
    currentBranch = &branches[0]; /// Si no se encuentra, se asigna al branch master por default
    return false;
}

template <class Tr>
bool CGraph<Tr>::changeUser(string another){
    this->user = another;
    return true;
}

/*
template <class Tr>
vector<Node*> CGraph<Tr>::findPath(string _target) {
    //Node* start = root;
    ///buscamos el nodo
    HNode* tmp = new HNode();
    //if ( not find(_target,tmp) ) return ;
    Node* target = tmp->getNode();
    vector<Node*> pathAux;
    Node* aux = target;
    do{
        pathAux.push_back(aux);
        aux = aux->pointingEdge->m_node[0];
    }while(aux != getCurrentRoot());
    if ( not std::find( path.begin(), path.end(), getCurrentRoot() ) )
        path.push_back(getCurrentRoot());
    return pathAux;
    //return this->path[path.size()-1]==getCurrentRoot()? true:false;
}
 */

template<class Tr>
bool CGraph<Tr>::buildPersistence(string target) {
    //cout << "En buildPersistence" <<endl;
    //states.insertNewState(getNodes());
    path = findPath(target);
    /*
    for(int i=0; i<path.size(); i++)
        cout << path[i]->getLabel()<< " ";
    cout <<"|" <<endl;
    */
    m_nodes = path;

    states.insertNewState(getNodes());
    return true;
}

template<class Tr>
void CGraph<Tr>::printGraph(string namefile) {
    cout << "Generando grafo: "<< namefile <<" ..."<<endl;
    //vector<Node*> aux = m_nodes;
    //m_nodes = buildGraph(states.getStatesNumber());
    nodesGraph = m_nodes;
    fstream graph;
    graph.open(namefile,fstream::out | fstream::binary);
    if (not graph.is_open()) return; ///no se pudo abrir el archivo, por alguna razon
    graph << "digraph G{ " << endl;
    graph <<     "node [shape=circle];"<< endl;
    for (int i=0; i< m_nodes.size(); i++){

        for (int j=0; j< m_nodes[i]->m_edges.size(); j++){
            //graph << "Imprimiendo grafo222" <<endl;
            graph << m_nodes[i]->getLabel() << " -> " << m_nodes[i]->m_edges[j]->m_node[1]->getLabel() << " [color= red];" << endl;
        }
    }

    graph << "}" << endl;
    graph.close();

    string command = "dot -Tpng "+namefile+" -o versions/images/graph"+to_string(currentId)+".png";
    const char* tmp = command.c_str();
    system(tmp);

}

template<class Tr>
void CGraph<Tr>::printPersistence() {
    cout << "Generando Path de Persistencia: " << currentId <<" ..." <<endl;
    fstream graph;
    graph.open("persistence.dot",fstream::out | fstream::binary);
    if (not graph.is_open()) return; ///no se pudo abrir el archivo, por alguna razon
    //graph << "# Printing printPersistence" << endl;
    graph << "digraph G{ " << endl;
    graph <<     "node [shape=circle];"<< endl;
    for (int k =0; k< states.getStatesNumber(); k++) {
        /*
        if (k ==0)
            graph << states.getVersion(k)[0]->getLabel() << "0 -> centinela"<<";"<< endl ;
        */
        for (int i = 0; i < states.getVersion(k).size()-1; i++){
                graph << states.getVersion(k)[i]->getLabel() <<k<<" -> " << states.getVersion(k)[i+1]->getLabel() <<k<< ";"<< endl;
        }
    }
    graph << "}" << endl;
    graph.close();

    system("dot -Tpng persistence.dot -o persistence.png");
}

template<class Tr>
void CGraph<Tr>::printVersions() {
    string namefileVersion = "versions/dot/graphVersion"+to_string(currentId)+".dot";
    printGraph(namefileVersion);
}



