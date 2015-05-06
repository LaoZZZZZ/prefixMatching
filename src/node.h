/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef NODE_H
#define NODE_H
#include <string>
#include <cassert>
#include <stdio.h>
#include <string.h>
#include <iostream>
using std::string;
namespace prefixMatching{
class Node
{
public:
    Node(int id);

    /**
      setter function
      */
    void  setBranch(size_t b){this->_isBranch = b;}
    void  setID(int id){this->_id = id;}
    void  setDepth(size_t d){this->_depth = d;}
    void  setNext(Node* n){this->_next = n;}
    void  setParent(Node* p){this->_parent = p;}
    void  setChild(Node* p){
            memcpy(&(this->_child),p,4*sizeof(Node*));
            for(size_t i = 0; i <= 3; i++){
                if(this->_child)
                    this->_isBranch++;
            }
        }
    void removeChild(int ind){
            assert(ind < 4);
            if(this->_child[ind]){
                this->_child[ind] = NULL;
                if(this->_isBranch > 0)
                    this->_isBranch--;
            }
    }
    void setChild(Node*p,int ind){
        if(!this->_child[ind] && p)
            this->_isBranch++;
        this->_child[ind] = p;

    }
    /**
      getter function
      */
    size_t              depth()const{return this->_depth;}
    const Node*         parent()const{return this->_parent;}
    Node*         parent(){return this->_parent;}

    const Node*         operator[](size_t ind)const{assert(ind<4);return this->_child[ind];}
    Node*         operator[](size_t ind){assert(ind<4);return this->_child[ind];}
    const Node*         child(size_t ind)const{assert(ind<4);return this->_child[ind];}
    Node*         child(size_t ind){assert(ind<4);return this->_child[ind];}
    const Node*         next()const{return this->_next;}
    Node*         next(){return this->_next;}
    ~Node(){
        this->_parent = NULL;
        this->_next = NULL;
    }
    bool isBranch()const{return this->_isBranch >=2 ;}
    int branch()const{return this->_isBranch;}
    int id()const{return this->_id;}
public:
    Node(Node& cp){
        this->copy(cp);
    }
    Node& operator=(Node& cp){
        this->copy(cp);
        return *this;
    }
    void copy(Node& cp){
        this->_id = cp.id();
        this->_depth = cp.depth();
        this->_parent = cp.parent();
        for(size_t i = 0; i <=3; i++){
            this->_child[i] = cp.child(i);
        }
        this->_next = cp.next();
        this->_isBranch = cp.isBranch();
    }
private:
    int                             _id;
    size_t                          _depth;      // my depth, root is 0
    Node*                           _parent;    // parent pointer
    Node*                           _child[4]; // children pointer
    Node*                           _next;     // my next
    int                             _isBranch;
};
}
#endif // NODE_H
