/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef LINEARMATCHING_H
#define LINEARMATCHING_H
#include <memory>
#include <algorithm>
#include "suffixtrie.h"
#include "matchingalgorithm.h"
using std::shared_ptr;
namespace prefixMatching{
class linearMatching : public matchingAlgorithm
{
public:
    linearMatching(const vector<std::shared_ptr<sequence>>& ,vector<int>&);

    ~linearMatching(){
    }
protected:

    /**
      * main body of the algorithm
      */
    void constructSolution();
    void        init();
    //might be used
    void        buildPriority(Node*);

    void    collapse_node(Node* node,int id){
        if(node){
            do{
                node->setID(id);
                node = node->parent();
            }while(node && !node->isBranch() && node->id() >= 0);
         }
    }
    /**
      * matching the branching node
      * propogate the single child to its nearest parent branching node
      * if necessary
      */
    void match(Node*);
    /**
      * match two nodes
      */

    Node* matchPair(Node* pair1,Node* pair2){
        int id1 = pair1->id();
        int id2 = pair2->id();
	// if they are reverse complement
        if(isReverseComplement(id1,id2)){
	    if(this->isMatched(id1))
            	return NULL;
	    else
		return pair1;
        }
	// if pair1 is matched already
	if(this->isMatched(id1)){
		if(this->isMatched(id2))
			return NULL;
		else
			return pair2; 
	}
	else{ // if pair1 is not matched yet
		if(this->isMatched(id2)) //if pair2 is matched
			return pair1;
		// if neither pair1 and pair2 are matched
        	this->_match[id1-1] = id2-1;
        	this->_match[id2-1] = id1-1;
        	this->_totalMatched++;
        	return NULL;
	}
    }

    /**
      * remove the non-branching internal node until
      * the nearest branching node
      */
    void removeBranch(Node* head){
        if(head){
            do{
                Node* p = head->parent();
                if(!p){
                    this->_trie->destroy(head);
                    head = NULL;
                }
                // its parent is non branching node
                else if(p && p->branch() == 1){
                    this->_trie->destroy(head);
                    head = p;
                }else{ // at least has two branches

                    for(size_t i = 0; i < 4; i++){
                        if(p->child(i) == head){
                            p->removeChild(i);
                            this->_trie->destroy(head);
                            head = NULL;
                            break;
                        }
                    }
                }
            }while(head);
    }
    }
    linearMatching(const linearMatching&);
    linearMatching& operator=(const linearMatching&);
private:
    std::unique_ptr<suffixTrie>                     _trie;
    int                                             _totalMatched;
};
}
#endif // LINEARMATCHING_H
