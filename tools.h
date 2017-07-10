//
// Created by jhoelccari on 10/05/17.
//

#ifndef VERSIONCONTROLLER_TOOLS_H
#define VERSIONCONTROLLER_TOOLS_H
#include <iostream>
#include "graph.h"
using namespace std;

template <class Tr>
class CBranch{
public:
    typedef typename Tr::Self self;
    typedef typename Tr::String e;
    string name;

    CEdge<self>* edge;

    const string &getName() const {
        return name;
    }

    void setName(const string &name) {
        CBranch::name = name;
    }

    void setEdge(CEdge<self> *edge) {
        CBranch::edge = edge;
    }

    CEdge<self> *getEdge() const {
        return edge;
    }

    CBranch(){
        name = "master";
        edge = nullptr;
    }

    CBranch(string n,CEdge<self>* p=nullptr){
        name = n;
        edge = p;
    }

    bool updateEdge(CEdge<self>* e){
        //cout << "Update: " << edge->m_node[0] << " to " << e->m_node[0] << endl;
        edge = e;
    }
};
#endif //VERSIONCONTROLLER_TOOLS_H
