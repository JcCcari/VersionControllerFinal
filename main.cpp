#include <iostream>
#include "graph.cpp"
#include "CPersistence.h"

using namespace std;

struct dataEdge{

};

struct traits{
    typedef traits Self;
    typedef CNode<Self> GNode;
    typedef CEdge<Self> GEdge;
    typedef CFile File;    /// Dato de los nodos
    typedef string String;  /// Dato de las aristas //PD cambiar esto
    //int edge;           /// @Param sera la diferencia de tiempos de creacion(t2 -t1)

    ///Para la tabla Hash
    typedef CNodeHash<Self> HNode;
    typedef DispersionFunction<Self> DFunction;
    typedef ListAdaptor<Self> LAdaptor;
    typedef CBranch<Self> Branch;
    typedef CPersistence<Self> Persistence;
};

int main(int argc, char** argv) {
    typedef typename traits::File File;
    string user = "jhoel";
    File file = File("test.txt");
    CGraph<traits>* g = new CGraph<traits>("test.txt", user);
    g->insert(file); // 1
    g->insert(file); // 2
    g->insert(file); // 3
    g->createBranch("branch1");
    g->checkout("branch1");
    g->insert(file); // 4
    g->checkout("master");
    g->insert(file); // 5
    g->insert(file); // 6
    g->insert(file); // 7
    g->insert(file); // 8
    g->remove("textmaster5"); //6
    g->printPersistence();
        return 0;
}

/*

 * */
/*
 g->insert(file); // 1
    g->insert(file); // 2
    g->insert(file); // 3
    g->createBranch("branch1");
    g->checkout("branch1");
    g->insert(file); // 4
    g->checkout("master");
    g->insert(file); // 5
    g->insert(file); // 6
    g->createBranch("branch2");
    g->checkout("branch2");
    g->insert(file); // 7
    g->insert(file); // 8
    g->checkout("master");
    g->insert(file); // 9
 **/