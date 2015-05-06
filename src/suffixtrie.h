/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef SUFFIXTRIE_H
#define SUFFIXTRIE_H
#include <string>
#include <vector>
#include "sstring.h"
#include "node.h"
using std::string;
using std::vector;
namespace prefixMatching {


    class suffixTrie
    {
        typedef BTDnaString Sequence;
    public:
        suffixTrie(vector<int>&);
        void insert(const Sequence&);
        ///const Sequence& operator[](size_t id){return this->_pool[id];}
        const Node*       root()const{return this->_root;}
        Node*             root(){return this->_root;}

        size_t      size()const{return this->_total;}
        ~suffixTrie();
        void destroy(Node*);
        size_t maxDepth()const{return this->_maxDepth;}
        const vector<Node*>&  priority()const{ return this->_priority;}
        // output the trie
        void dump(std::ostream&);
        vector<int>     initMatching()const{return this->_initMatching;}
        void countLeaf(Node* n,int& total){
            if(n->branch()== 0 && n->id() >= 0)
                total++;
            else{
                for(size_t i = 0; i < 4; i++){
                    if(n->child(i))
                        countLeaf(n->child(i),total);
                }
            }
        }
        Node* findLeafNode(const Sequence& str)const{
            Node* cur = this->_root;
            for(size_t i = 0; i < str.length(); i++){
                cur = cur->child(str[i]);
            }
            return cur;
        }
        vector<int> findPath(Node* n)const{
            vector<int> p;
            while(n){
                p.push_back(n->id());
                n = n->parent();
            }
            return p;

        }
    private:
        void init();
	void rewire(Node* parent,int id){
		bool found = false;
		for(int i = 0; i < 4; i++){
			if(parent->child(i) && parent->child(i)->id() != id && !this->isMatched(parent->child(i)->id())){
				parent->setID(parent->child(i)->id());
				found = true;
				break;
			}
		}
		if(!found)
			parent->setID(-2);	
	}
	inline int reverseComID(int id){
		if(id%2)
			return id + 1;
		else
			return id - 1;
	}
	bool isMatched(int id){
		if(id < 0) return true;
		else{
			return this->_initMatching[id-1] != id-1 
			|| this->_initMatching[this->reverseComID(id)-1] !=this->reverseComID(id)-1;
		}	
	}
    private:
        int                                         _total;
        Node*                                       _root;
        size_t                                      _maxDepth;
        vector<Node*>                               _priority;
        vector<int>&                                 _initMatching;
    };

}


#endif // SUFFIXTRIE_H
