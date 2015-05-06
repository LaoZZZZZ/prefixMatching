#include "snowball.h"

namespace prefixMatching{
snowBall::snowBall(std::vector<std::shared_ptr<sequence> >& r, vector<int>& m):treeMatchingMultipleWay(r,m)
{
    //this->init();
    this->_tmporder.clear();
}
void snowBall::finalizeOrder(){
    this->_order.clear();
    for(const int& id : this->_tmporder){
        if(this->_match[id-1] != -1){
            this->_order.push_back(id-1);
        }
    }
}

void snowBall::constructSolution(){
    linkNode* leaves = this->_suffix->leaf();
    linkNode* head = leaves;
    while(head){
        list<int>& ids = head->ids();
        for(const int& id : ids){
            this->_tmporder.push_back(id);
        }
        head = head->next();
    }
    //cout<<this->_tmporder.size()<<endl;
    const vector<linkNode*>& a = this->_suffix->priority();
    // start from the deepest node, traverse the priority queue
    if(a.empty())
        return ;

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
    this->finalizeOrder();
}
void snowBall::match(linkNode * node){


    if(node->isLeaf()){
        list<int>&  ids = node->ids();
        for(const int& id:ids){
            if(!this->isMatched(id)){
                this->_match[this->reverseComplement(id)-1] = -1;
            }
        }
    }
    // if this node has not been matched
    if(-1 != node->id()){
        linkNode* child = NULL;
        for(size_t i = 0; i <= 4; i++){
            child = node->child(i);
            if(child){
                this->match(child);
            }
        }
        node->addID(-1);
    }
}
}
