//
// Created by jhoelccari on 05/05/17.
//

#ifndef VERSIONCONTROLLER_GRAPH_H
#define VERSIONCONTROLLER_GRAPH_H

#include "nodeAndEdge.h"
#include "tools.h"
#include "hashTable.h"

#include <vector>
using namespace std;

template <class G>
class CNode;

template <class G>
class CEdge;

template <class Tr>
class CGraph
{
public:
    typedef typename Tr::Self traits;
    typedef CNode<traits> Node;
    typedef CEdge<traits> Edge;
    typedef typename Tr::File N;
    typedef typename Tr::String E;
    typedef typename Tr::HNode HNode;
    typedef typename Tr::Branch Branch;
    typedef typename Tr::Persistence Persistence;

    /* data */
    string user;
    int currentId;
    vector<Node*> m_nodes;
    vector<Node*> nodesGraph;
    CBranch<traits>* currentBranch;
    vector<Edge*> allEdges;
    vector<CBranch<traits>> branches;
    Node* currentRoot;

    ///Tenemos que tener un root para guardar los estados del grafo en un tiempo n
    //CHashTable<CNodeHash<CNode<traits>>,DispersionFunction<string>,ListAdaptor<CNodeHash<CNode<traits>>>>* hashtable;
    CHashTable<traits>* hashtable;

    /// PARA PATHCOPY
    /// Almacenaremos el camino de "root" hacia un "Node"
    vector<Node*> path;

    /// PARA PERSISTENCIA
    /// Almaneremos punteros a vectores de nodos en otro vector llamado states
    Persistence states;
    //vector<vector<Node*>* > states;


/******************************************************************************/
/****************************** FUNCTIONS *************************************/
/******************************************************************************/
    CGraph(const string filename, string _user);
    ~CGraph();

    bool insert(N& x);

    /// esta funcion busca un nodo, usando como estructura la tabla hash "CHashTable<traits>* hashtable;"
    /// @Param string finded ->
    /// @Param HNode* node ->(opcional) retorna la direccion de memoria del nodo buscado
    bool find(string finded, HNode* node= nullptr);
    bool remove(string target);
    bool merge(string nodeLabel1, string nodeLabel2);
    bool restore(int state);

    bool checkCurrentBranch(Edge* b, Branch* tmp= nullptr);
    bool isNewBranch(string nameBranch);

    bool buildPersistence(string target);


    void saveNode(Node* newNode){
        m_nodes.push_back(newNode);
        nodesGraph.push_back(newNode);
    }

    bool createBranch(string nameBranch);
    bool checkout(string nameBranch);
    bool changeUser(string another);

    Node *getCurrentRoot()  const { return currentRoot; }
    void setCurrentRoot(Node * newRoot){ currentRoot = newRoot; }

    void printGraph(string namefile);
    void printPersistence();
    void printVersions();

    bool findNodeInPath(Node* n){
        for(int i=0; i<path.size(); i++){
            if( path[i] == n)
                return true;
        }
        return false;
    }

    /// esta funcion buscara un camino desde la raiz a nodo "target"
    /// por lo que guardara los nodos en medio en el "vector<Node*> path"
    vector<Node*> findPath(string targ){

        ///buscamos el nodo
        HNode* tmp = new HNode();
        if (find(targ,tmp) )
            cout << "---> Encontrado" << endl;
        else
            cout << "---> No encontrado" << endl;
        Node* _target = tmp->getNode();
        vector<Node*> pathAux;
        Node* aux = _target;
        //cout << "findpath before" << endl;
        //cout << "target: " << aux->getLabel() << endl;
        while( aux != getCurrentRoot() and aux != nullptr){
            pathAux.push_back(aux);
            aux = aux->getPointingEdge()->getFirstNode();
            //cout << aux->getLabel() << " | ";
        }
        //cout << endl;
        //cout << "findpath after" << endl;
        //if (not findNodeInPath( getCurrentRoot() ) ) {
            ///cout << "Agregando root" << endl;
        pathAux.push_back(getCurrentRoot());
        //}
        vector<Node*> pathFinal;
        for(int i=0; i<pathAux.size(); i++)
            pathFinal.push_back(pathAux[pathAux.size()-i-1]);
        /*
        for(int i=0; i<pathAux.size(); i++)
            cout << pathFinal[i]->getLabel()<< " ";
        cout <<"|" <<endl;
         */
        return pathFinal;
    }

    const vector<Node *> &getNodes() const {
        return m_nodes;
    }

    const vector<Node *> &getPath() const {
        return path;
    }

    bool findInVector(string label, vector<Node*>& tmp){
        for(int i=0; i< tmp.size(); i++){
            if (label == tmp[i]->getLabel())
                return true;
        }
        return false;
    }
    ///* Contruye el grafo a partir de las versiones 0-(state-1)
    vector<Node*> buildGraph(int state){
        cout << "entra de buildGraph" << endl;
        vector<Node*> tmp;
        if (state > states.getStatesNumber())
            state = states.getStatesNumber();

        for(int i=0; i<state; i++){
            for ( int j=0; states.getVersion(i).size(); j++ ){
                if( not findInVector(states.getVersion(i)[j]->getLabel(),tmp))
                    cout << "agreaga" << endl;
                    tmp.push_back(states.getVersion(i)[j]);
            }
        }
        cout << "sale de buildGraph" << endl;
        return tmp;
    }

};

#endif //VERSIONCONTROLLER_GRAPH_H
