/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#include "suffixtrie.h"

namespace prefixMatching{
suffixTrie::suffixTrie(vector<int>& match):_total(0),_root(new Node(-1)),_maxDepth(0),_initMatching(match)
{
}
suffixTrie::~suffixTrie(){
    this->destroy(this->_root);
}
// recursively delete the trie from the bottom to top
// might need to come up a iterative solution for optimization
void suffixTrie::destroy(Node* root){
    if(root){
        for(int i = 0; i <= 3; i++){
            if(root->child(i))
                this->destroy(root->child(i));
        }
        Node* parent = root->parent();
        if(parent){
            for(size_t i = 0; i < 4; i++){
                if(parent->child(i) == root){
                    parent->removeChild(i);
                    break;
                }
            }
        }
        delete root;
        root = NULL;
    }

}

void suffixTrie::init(){
    this->_root = new Node(-1);
}
void suffixTrie::insert(const Sequence& str){
    this->_total++;
    // if this is the reverse complement of the original string
    // and the original one matched. no need to insert this string
   /*
   if(this->_total == 191896){
	std::cout<<this->_initMatching[this->_total-2]<<endl;
	std::cout<<!(this->_total%2) << (this->_total-2 != this->_initMatching[this->_total-2])<<std::endl;
	}
    */
    if(!(this->_total%2) && this->_total-2 != this->_initMatching[this->_total-2]){
        this->_initMatching.push_back(this->_total-1);
        return;
    }

    assert(this->_root);
    Node* cur = this->_root;

    for(size_t id = 0; id < str.length();id++){

        assert(str[id] < 4);
        // if the current node has children
        if(cur->child(str[id])){
            // identical sequence come out
            if(id == str.length() - 1){
                // the sequence id that match the currently inserted sequence
                int chid = cur->child(str[id])->id();
                assert(chid != -1);
                if(chid == -2){
                    cur->child(str[id])->setID(this->_total);
                    this->_initMatching.push_back(this->_total-1);
                }
                else{
                    assert(chid != -1);
                    int rev = 0;
                    if(chid%2)
                        rev = chid + 1;
                    else
                        rev = chid - 1;
                  ///they are not reverse complementary to each other
                    if(this->_total != rev){
                        if((this->_initMatching[chid-1] == chid-1) &&
                           (this->_initMatching[rev-1] == rev-1)){
                        // forward and its reverse is not matched
                            this->_initMatching[chid-1] = this->_total -1;
                            this->_initMatching.push_back(chid-1);
                            cur->child(str[id])->setID(-2);
                            while(cur && cur->branch() == 1 && cur->id() != -1){
                                cur->setID(this->_total);
                                cur = cur->parent();
                            }
			    // rename the id of the branch node
			    if(cur && cur->id() != -1 &&  cur->id() == chid){
				this->rewire(cur,chid);
			    } 
                        }
                        else{
                            cur->child(str[id])->setID(this->_total);

                            // find the shallowest non branching node
                            while(cur && cur->branch() == 1 && cur->id() != -1){
                                cur->setID(this->_total);
                                cur = cur->parent();
                            }
			    if(cur && cur->id() != -1 && cur->id() == chid){
				cur->setID(this->_total);
			    }
                            this->_initMatching.push_back(this->_total-1);

                        }
                    }
                    else{
                        this->_initMatching.push_back(this->_total-1);
                    }
                }


            }
            else
                cur = cur->child(str[id]);
        }
        else{
            Node* tmp = new Node(this->_total);
            tmp->setParent(cur);
            tmp->setDepth(cur->depth() + 1);
            cur->setChild(tmp,str[id]);
            if(cur->branch() == 2){
                size_t d = cur->depth() + 1;
                while(this->_priority.size() < d ){
                    this->_priority.push_back(NULL);
                }
                d--;
                Node* head = this->_priority[d];
                cur->setNext(head);
                this->_priority[d] = cur;
            }
            cur = tmp;
            if(cur->depth() > this->_maxDepth)
                this->_maxDepth = cur->depth();

            if(id == str.length() - 1){
                this->_initMatching.push_back(this->_total-1);
            }

        }

    }
    cur=NULL;
}


}
