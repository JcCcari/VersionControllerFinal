//
// Created by jhoelccari on 05/05/17.
//

#ifndef VERSIONCONTROLLER_NODEANDEDGE_H
#define VERSIONCONTROLLER_NODEANDEDGE_H
#include <iostream>
#include <vector>
#include <utility>
using namespace std;

template <class Tr>
class CNode
{
public:
    typedef typename Tr::GEdge Edge; // typedef typename G::Edge Edge; // tmbien es valido esto porque en la clase grafo, decimos que ambos son sus
    typedef typename Tr::File N;

    int id;
    string owner;
    string label;
    N* file;

    vector<Edge*> m_edges;

    Edge* pointingEdge[2];     /// puntero al edge que apunta al nodo
                                /// es de tamaño 2, por si es resultado de un merge

    CNode(){ }
    CNode(int i,string o,N* x, Edge* pe1 = nullptr,Edge* pe2 = nullptr){
        owner = o;
        id = i;
        file = x;
        pointingEdge[0] = pe1;
        pointingEdge[1] = pe2;
    }
    ~CNode(){};

    void destroyNode(){
        this->~CNode();
    }

    int getId() const {
        return id;
    }

    const string &getOwner() const {
        return owner;
    }

    N *getFile() const {
        return file;
    }

    int findPositionEdge(Edge* e){
        for(int i=0; i< m_edges.size(); i++){
            if(e == m_edges[i])
                return i;
        }
    }

    void removeEdge(Edge* e){
        int pos = findPositionEdge(e);
        typename vector<Edge*>::iterator it = m_edges.begin();
        m_edges.erase(it+pos);
    }

    void saveEdge(Edge* e){
        m_edges.push_back(e);
    }

    Edge *getPointingEdge() const {
        return pointingEdge[0];
    }

    void setPointingEdge(Edge *pe) {
        pointingEdge[0] = pe;
    }

    Edge *getPointingEdge2() const {
        return pointingEdge[1];
    }

    void setPointingEdge2(Edge *pe) {
        pointingEdge[1] = pe;
    }

    const string getLabel() const {
        return label;
    }

    void setLabel(const string &l) {
        label = l;
    }

};

template <class Tr>
class CEdge
{
public:
    typedef typename Tr::GNode Node;
    typedef typename Tr::String E;

    CEdge(const E x, Node* a, Node* b){
        m_data = x;
        m_node[0] = a ;
        m_node[1] = b ;
    }

    Node* getFirstNode(){
        return m_node[0];
    }
    Node* getSecondNode(){
        return m_node[1];
    }

    ~CEdge(){};

    /* data */
    Node* m_node[2];
    E m_data;
};


#endif //VERSIONCONTROLLER_NODEANDEDGE_H
