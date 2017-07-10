//
// Created by jhoelccari on 16/05/17.
//

#ifndef VERSIONCONTROLLER_HASHTABLE_H
#define VERSIONCONTROLLER_HASHTABLE_H

#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <list>

using namespace std;

template <class Tr>
class CNodeHash{
public:
    typedef typename Tr::GNode N;
    typedef typename Tr::GEdge E;
    string id;
    N* node;
    E* edge;

    CNodeHash(){}
    CNodeHash(string i,N* n, E* e){
        id = i;
        node = n;
        edge = e;
    }

    N *getNode() const {
        return node;
    }

    void setNode(N *node) {
        CNodeHash::node = node;
    }

    E *getEdge() const {
        return edge;
    }

    void setEdge(E *edge) {
        CNodeHash::edge = edge;
    }

    const string &getId() const {
        return id;
    }
};


template <class Tr>
class ListAdaptor
{
    typedef typename Tr::HNode T;
public:
    vector<T> l;
    //typedef typename list<T> self;
    //int aux = 0;
    bool find(string x,T& node)
    {
        for(int i=0; i< l.size(); i++){
            if(l[i].id==x) {
                node = l[i];
                return 1;
            }
        }
        return 0;
    }

    bool insert(T x)
    {
        //if(find(x)) return 0;
        l.push_back(x);
        return 1;
    }

    bool remove(T x)
    {
        int pos;
        if(!find(x,pos))return 0;
        typename vector<T>::iterator it = l.begin();
        l.erase(it+pos);
        return 1;
    }

    void imprimir()
    {
        typename vector<T>::iterator it = l.begin();
        for(int i=0; i<l.size();i++, it++){
            cout<< *it << " ";
        }
    }
    bool empty(){
        return l.empty();
    }
};

/*
template <class Tr>
class ListAdaptor
{
    typedef typename Tr::HNode T;
public:
    list<T> l;
    //typedef typename list<T> self;

    bool find(T x)
    {
        list<string>::iterator it = l.begin();
        for(int i=0; i<l.size();i++, it++){
            if( *it ==x)
                return 1;
        }
        return 0;
    }

    bool insert(T x)
    {
        //if(find(x)) return 0;
        l.push_back(x);
        return 1;
    }

    bool remove(T x)
    {
        if(!find(x))return 0;
        l.remove(x);
        return 1;
    }

    void imprimir()
    {
        typename list<T>::iterator it = l.begin();
        for(int i=0; i<l.size();i++, it++){
            cout<< *it << " ";
        }
    }
    bool empty(){
        return l.empty();
    }

    T operator[](int n){
        typename list<T>::iterator it = l.begin();
        for(int i=0; i<n; i++)
            it++;
        return *(it);
    }
};
*/
template <class Tr>
class DispersionFunction
{
    //typedef typename Tr::String T;
    typedef string T;
public:
///metodo de compresion, ascii
    vector<int> binario(int num)
    {
        vector<int>binarios;
        int base=2;
        while(num>=2){
            binarios.insert(binarios.begin(),num%base);
            num=num>>1;
        }
        binarios.insert(binarios.begin(),num);

        return binarios;
    }

    int convert_to_decimal(vector<int> numero)///GENERA EL ALEATORIO EN DECIMAL
    {
        int num_decimal;
        long base=2;
        num_decimal=0;
        long tam=numero.size()-1;
        long i;
        for( i=0; i<=tam;i++){
            if(numero[i]==1){
                num_decimal+=pow(base,tam-i);
            }
        }

        //si es multiplo de 5 lo paso
        if(num_decimal%5==0){
            num_decimal=num_decimal+2;
        }
        return num_decimal;
    }


    int operator()(T num)
    {
        int aux=0;
        vector<vector<int>> contenedor;
        for(int i=0;i<num.size();i++)
        {
            aux=num[i];//caracter ascii
            //  cout<<"numero: "<<aux<<endl;
            contenedor.push_back(binario(aux));
            while(contenedor[i].size()<8)
            {
                contenedor[i].insert(contenedor[i].begin(),0);
            }
        }

        //hacer el xor de
        int resultado=0;
        vector<int> adecimal;

        for(int u=0;u<contenedor[0].size();u++){
            resultado=0;
            for(int w=0;w<contenedor.size();w++){
                // cout<<"DE: "<<w<<" palabra "<<u<<" posicion "<<contenedor[w][u]<<endl;
                resultado= resultado ^(contenedor[w][u]);

            }
            // cout<<resultado;
            adecimal.push_back(resultado);
        }
        aux=convert_to_decimal(adecimal);

        //  cout<<"este es aux: "<<aux<<endl;

        return aux;

    }

};

//template<class T,class F, class E>
template <class Tr>
class CHashTable
{
    typedef typename Tr::LAdaptor E;
    typedef typename Tr::DFunction F;
    typedef typename Tr::HNode T;
public:
    vector<E> m_ht;
    F m_f;
    int m_size;

    CHashTable(int size)
    {
        m_size=size;
        m_ht.resize(m_size);
    }

    bool Insert(T x)
    {
        int id=m_f(x.id)%m_size;
        cout<< x.getId() <<" -> "<<id<< " Insertion Successful "<<endl;
        return m_ht[id].insert(x);
    }

    bool remove(T x)
    {
        int id=m_f(x.id)%m_size;
        //cout<< x <<" -> "<<id<< " Delete Successful "<<endl;
        return m_ht[id].remove(x);
    }

    bool find(string x, T& node)
    {

        int id=m_f(x)%m_size;
        //cout <<"En hash " <<id << endl;
        return m_ht[id].find(x,node);
    }

    void imprimir()
    {
        cout << " Contents of Hash Table\n";
        for(int i=0; i<m_size ; i++){
            cout << "[ "<< i << " ] " ;
            if(!m_ht[i].empty())
                m_ht[i].imprimir();
            cout << endl;
        }
        cout << endl;
    }

};

#endif //VERSIONCONTROLLER_HASHTABLE_H
