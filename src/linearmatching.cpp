/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#include "linearmatching.h"
namespace prefixMatching{
linearMatching::linearMatching(const vector<std::shared_ptr<sequence> >& r,vector<int>& m):matchingAlgorithm(r,m)
{
    this->init();
}
void linearMatching::init(){
    //int tstring = this->_trie->size();
    this->_trie.reset(new suffixTrie(this->_match));
    for(const std::shared_ptr<sequence>& r: this->_readsPool){
        this->_trie->insert(r->fowardSeq());
        BTDnaString rc;
        r->reverseComSeq(rc);
        this->_trie->insert(rc);
    }
}
/**
  * main procedure for the prefix matching
*/
void  linearMatching::constructSolution(){
    const vector<Node*>& a = this->_trie->priority();
    // start from the deepest node, traverse the priority queue
    if(a.empty())
        return ;
    Node* head = NULL;
    int depth = a.size()-1;
    // traverse the priority queue from the bottom
    while(depth >= 0){
        head = a[depth];
        while(head){
             this->match(head);
             head = head->next();
        }
        depth--;
    }
    this->OutputOrder();

}
void linearMatching::match(Node* head){
    // find the first pair of string
    vector<int> unmatched;
    int indx(0);
    for(; indx < 4; indx++){
        if(head->child(indx) && !this->isMatched(head->child(indx)->id())){
            unmatched.push_back(indx);
        }
    }
    // there is only one valid pair
    if(unmatched.empty()){
        head->setID(-2);
    }
    else if(unmatched.size() == 2){
	    Node* res =  this->matchPair(head->child(unmatched[0]),head->child(unmatched[1]));
            if(res)
	    	this->collapse_node(head,res->id());
	    else
		this->collapse_node(head,-2);

    }else if(unmatched.size() == 1){ // need to progogate the
        this->collapse_node(head,head->child(unmatched[0])->id());

    }else if(unmatched.size() == 3){ // need to propagate
	Node* unode = this->matchPair(head->child(unmatched[0]),head->child(unmatched[1]));	
        if(!unode){
	    int id = -2;
            if(!this->isReverseComplement(head->child(unmatched[0])->id(),head->child(unmatched[2])->id())
                && !this->isReverseComplement(head->child(unmatched[1])->id(),head->child(unmatched[2])->id()))
                id = head->child(unmatched[2])->id();
            this->collapse_node(head,id);
        }else{
            // 0,1 are reverse complement
	    unode = this->matchPair(unode,head->child(unmatched[2]));
            assert(unode==NULL);
            this->collapse_node(head,-2);
            //head->setID(-2);
        }
    }else{
        // two complete pair
	Node* unode = this->matchPair(head->child(unmatched[0]),head->child(unmatched[1]));
        // 0,1 are not reverse complement
        if(!unode){
		 // if node 2 is reverse complement of node 0 or 1
		 if(this->isReverseComplement(head->child(unmatched[0])->id(),head->child(unmatched[2])->id())
                    || this->isReverseComplement(head->child(unmatched[1])->id(),head->child(unmatched[2])->id())){
			// node 3 is also the reverse complement of node 0 or 1
			if(this->isReverseComplement(head->child(unmatched[0])->id(),head->child(unmatched[3])->id())
                    || this->isReverseComplement(head->child(unmatched[1])->id(),head->child(unmatched[3])->id())){
				this->collapse_node(head,-2);
			}
			else
				this->collapse_node(head,head->child(unmatched[3])->id());
		}
	        else{//node 2 is not reverse complement of node 0 or 1
			// if node 3 is reverse complement of node 0 or 1
			if(this->isReverseComplement(head->child(unmatched[0])->id(),head->child(unmatched[3])->id())
                    || this->isReverseComplement(head->child(unmatched[1])->id(),head->child(unmatched[3])->id())){
				this->collapse_node(head,unmatched[2]);
			}
			else{
				unode = this->matchPair(head->child(unmatched[2]),head->child(unmatched[3]));
				if(unode)
					this->collapse_node(head,unode->id());
				else
					this->collapse_node(head,-2);
			}

		}
		
	}
	else{
	    // unmatched node must be matched with node 2 
            assert(NULL == this->matchPair(unode,head->child(unmatched[2])));
	    if(this->isReverseComplement(head->child(unmatched[2])->id(),head->child(unmatched[3])->id()))
		this->collapse_node(head,-2);
	    else
		this->collapse_node(head,head->child(unmatched[3])->id());
		
        }
      }
}
}
