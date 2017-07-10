//
// Created by jhoelccari on 24/05/17.
//

#ifndef VERSIONCONTROLLER_CPERSISTENCE_H
#define VERSIONCONTROLLER_CPERSISTENCE_H
#include <vector>
using namespace std;
template <class Tr>
class CPersistence{
    typedef typename Tr::GEdge Edge;
    typedef typename Tr::GNode Node;
    vector<vector<Node*>> version;
    int current;
public:

    CPersistence() {
        current = 0;
    }

    vector<Node*> getCurrentVersion(){
        return version[current];
    }

    bool insertNewState(vector<Node*> v){
        if ( not version.empty())
            current++;
        version.push_back(v);
        return true;
    };

    bool removeState(int pos){
        ///eliminamos version[pos-1] pero que pasa con las siguientes versiones?

        return true;
    };

    vector<Node*> getVersion(int index){
        if (index >= 0 or index <= current)
            return version[index];
        return getCurrentVersion();
    }

    void changeVersion(int newVersion){
        if (newVersion >= 0 or newVersion <= current)
            current = newVersion;
        ///deberia retornar version[newVersion-1] ??
    }

    unsigned long getStatesNumber(){
        return version.size();
    }

    ///* Verifica si el nodo con la etiqueta @Param label, existe en el estado @Param state
    bool checkIfExist(string label, int state){
        for(int i=0; i< version[state].size(); i++){
            if (version[state][i].getLabel() == label)
                return true;
        }
        return false;
    }

};

#endif //VERSIONCONTROLLER_CPERSISTENCE_H
