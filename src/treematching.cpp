#include "treematching.h"

namespace prefixMatching{
treeMatching::treeMatching(std::vector<std::shared_ptr<sequence> >& r, vector<int>& m):matchingAlgorithm(r,m)
{
    this->init();
}
void treeMatching::init(){
    this->_order.clear();
    this->_suffix.reset(new suffixTree(this->_readsPool));
    size_t total = 0;
    for(const std::shared_ptr<sequence>& r: this->_readsPool){
        this->_suffix->insert(r->fowardSeq());
        this->_match.push_back(total);
        BTDnaString rc;
        r->reverseComSeq(rc);
        this->_suffix->insert(rc);
        total+=1;
        this->_match.push_back(total);
        total+=1;
    }
}
void treeMatching::constructSolution(){
    const vector<treeNode*>& a = this->_suffix->priority();
    // start from the deepest node, traverse the priority queue
    if(a.empty())
        return ;
    treeNode* head = NULL;
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
void treeMatching::match(treeNode* node){
    // if the node is leaf, then check if there are identical sequence
    // if yes, then group those identical sequence together in the ultimate order
    if(node->isLeaf()){
        list<int>& ids = node->ids();

        vector<int> tmp;
        for(const int& id : ids){
            if(!this->isMatched(id)){
                tmp.push_back(id);
                this->_match[id-1] = -1;
            }
        }
        if(tmp.size() > 1){
            for(const int& id: tmp){
                this->_order.push_back(id-1);
            }
            ids.clear();
            ids.push_back(-1);
        }
        else if(tmp.size()==1){ // there is only one id unmatched
            ids.front() = tmp[0];
            this->_match[tmp[0]-1] = tmp[0] - 1;
        }
        else{
            ids.clear();
            ids.push_back(-1);
        }


    }else{ // if the node is internal node, then find all possible matching within this node
        vector<int> unmatched;
        int indx(0);
        std::unordered_map<int,int> unmatchedDict;
        // collect all unmatched sequence
        for(; indx <= 4; indx++){
            if(node->child(indx)){
                int id = node->child(indx)->id();
                if(!this->isMatched(id) && unmatchedDict.count(this->reverseComplement(id)) == 0 ){
                    unmatched.push_back(indx);
                    unmatchedDict.insert(std::make_pair(id,indx));
                }
            }
        }

        // there is only one valid pair
        if(unmatched.empty()){
            node->setID(-2);
        }else if(unmatched.size() == 1){
            if(node->isRoot()){ // read the top of the suffix tree no need to propogate
                this->_order.push_back(node->child(unmatched[0])->id()-1);
                this->_match[this->reverseComplement(node->child(unmatched[0])->id())-1] = -1;
            }
            else
                this->collapse_node(node,node->child(unmatched[0])->id());
        }else if(unmatched.size() == 2){
            bool success = false;
            treeNode* res =  this->matchPair(node->child(unmatched[0]),node->child(unmatched[1]),success);
            assert(success && !res);
        }else if(unmatched.size() == 3){
            bool success = false;
            treeNode* unode = this->matchPair(node->child(unmatched[0]),node->child(unmatched[1]),success);
            assert(success && !unode);
            this->collapse_node(node,node->child(unmatched[2])->id());
        }else if(unmatched.size() == 4){
            bool success = false;
            treeNode* unode = this->matchPair(node->child(unmatched[0]),node->child(unmatched[1]),success);
            assert(success && !unode);

            success = false;
            unode = this->matchPair(node->child(unmatched[2]),node->child(unmatched[3]),success);
            assert(success && !unode);
        }else{
            assert(unmatched.size() == 5);
            bool success = false;
            treeNode* unode = this->matchPair(node->child(unmatched[0]),node->child(unmatched[1]),success);
            assert(success && !unode);

            success = false;
            unode = this->matchPair(node->child(unmatched[2]),node->child(unmatched[3]),success);
            assert(success && !unode);
            this->collapse_node(node,node->child(unmatched[4])->id());
        }
        /*
        else if(unmatched.size() == 2){
            bool success = false;
            treeNode* res =  this->matchPair(node->child(unmatched[0]),node->child(unmatched[1]),success);
                if(res)
                    this->collapse_node(node,res->id());
                else{
                    this->collapse_node(node,-2);
                    if(success){
                        this->_order.push_back(node->child(unmatched[0])->id()-1);
                        this->_order.push_back(node->child(unmatched[1])->id()-1);
                    }

                }

        }else if(unmatched.size() == 1){ // need to progogate the
            if(node->isRoot()) // read the top of the suffix tree no need to propogate
                this->_order.push_back(node->child(unmatched[0])->id()-1);
            else
                this->collapse_node(node,node->child(unmatched[0])->id());

        }else if(unmatched.size() == 3){ // need to propagate
            bool success = false;
            treeNode* unode = this->matchPair(node->child(unmatched[0]),node->child(unmatched[1]),success);
            if(!unode){
                if(success){
                    this->_order.push_back(node->child(unmatched[0])->id()-1);
                    this->_order.push_back(node->child(unmatched[1])->id()-1);
                }
                int id = -2;
                if(!this->isReverseComplement(node->child(unmatched[0])->id(),node->child(unmatched[2])->id())
                        && !this->isReverseComplement(node->child(unmatched[1])->id(),node->child(unmatched[2])->id()))
                        id = node->child(unmatched[2])->id();
                this->collapse_node(node,id);
            }else{
                // 0,1 are reverse complement
                success = false;
                int id = unode->id();
                unode = this->matchPair(unode,node->child(unmatched[2]),success);
                this->_order.push_back(id-1);
                this->_order.push_back(node->child(unmatched[2])->id()-1);
                assert(unode==NULL && success);
                this->collapse_node(node,-2);
                //node->setID(-2);
            }
        }else if(unmatched.size() == 4){
            // two complete pair
            bool success = false;
            treeNode* unode = this->matchPair(node->child(unmatched[0]),node->child(unmatched[1]),success);
            // 0,1 are not reverse complement
            if(!unode){
                if(success){
                    this->_order.push_back(node->child(unmatched[0])->id()-1);
                    this->_order.push_back(node->child(unmatched[1])->id()-1);
                }
             // if node 2 is reverse complement of node 0 or 1
             if(this->isReverseComplement(node->child(unmatched[0])->id(),node->child(unmatched[2])->id())
                        || this->isReverseComplement(node->child(unmatched[1])->id(),node->child(unmatched[2])->id())){
                // node 3 is also the reverse complement of node 0 or 1
                    if(this->isReverseComplement(node->child(unmatched[0])->id(),node->child(unmatched[3])->id())
                            || this->isReverseComplement(node->child(unmatched[1])->id(),node->child(unmatched[3])->id())){
                        this->collapse_node(node,-2);
                    }else
                    this->collapse_node(node,node->child(unmatched[3])->id());
                }
            else{//node 2 is not reverse complement of node 0 or 1
                // if node 3 is reverse complement of node 0 or 1
                if(this->isReverseComplement(node->child(unmatched[0])->id(),node->child(unmatched[3])->id())
                        || this->isReverseComplement(node->child(unmatched[1])->id(),node->child(unmatched[3])->id())){
                    this->collapse_node(node,unmatched[2]);
                }
                else{ // neigher node 2 nor node 3 is reverse complement of 0 or 1
                    success = false;
                    unode = this->matchPair(node->child(unmatched[2]),node->child(unmatched[3]),success);
                    if(unode)
                        this->collapse_node(node,unode->id());
                    else{
                        if(success){
                            this->_order.push_back(node->child(unmatched[2])->id()-1);
                            this->_order.push_back(node->child(unmatched[3])->id()-1);
                        }
                        this->collapse_node(node,-2);
                    }
                }

            }

        }
        else{
            // unmatched node must be matched with node 2
            success = false;
            int id = unode->id();
            assert(NULL == this->matchPair(unode,node->child(unmatched[2]),success));
            if(success){
                this->_order.push_back(id-1);
                this->_order.push_back(node->child(unmatched[2])->id()-1);
            }
            if(this->isReverseComplement(node->child(unmatched[2])->id(),node->child(unmatched[3])->id()))
                this->collapse_node(node,-2);
            else
                this->collapse_node(node,node->child(unmatched[3])->id());

            }
          }
        else{ // there are five branches
            // two complete pair
            bool success = false;
            treeNode* unode = this->matchPair(node->child(unmatched[0]),node->child(unmatched[1]),success);
            // 0,1 are not reverse complement
            if(!unode){
                if(success){
                    this->_order.push_back(node->child(unmatched[0])->id()-1);
                    this->_order.push_back(node->child(unmatched[1])->id()-1);
                }
             // if node 2 is reverse complement of node 0 or 1
             if(this->isReverseComplement(node->child(unmatched[0])->id(),node->child(unmatched[2])->id())
                        || this->isReverseComplement(node->child(unmatched[1])->id(),node->child(unmatched[2])->id())){
                // node 3 is also the reverse complement of node 0 or 1
                    if(this->isReverseComplement(node->child(unmatched[0])->id(),node->child(unmatched[3])->id())
                            || this->isReverseComplement(node->child(unmatched[1])->id(),node->child(unmatched[3])->id())){
                        this->collapse_node(node,-2);
                    }else
                    this->collapse_node(node,node->child(unmatched[3])->id());
                }
            else{//node 2 is not reverse complement of node 0 or 1
                // if node 3 is reverse complement of node 0 or 1
                if(this->isReverseComplement(node->child(unmatched[0])->id(),node->child(unmatched[3])->id())
                        || this->isReverseComplement(node->child(unmatched[1])->id(),node->child(unmatched[3])->id())){
                    this->collapse_node(node,unmatched[2]);
                }
                else{ // neigher node 2 nor node 3 is reverse complement of 0 or 1
                    success = false;
                    unode = this->matchPair(node->child(unmatched[2]),node->child(unmatched[3]),success);
                    if(unode)
                        this->collapse_node(node,unode->id());
                    else{
                        if(success){
                            this->_order.push_back(node->child(unmatched[2])->id()-1);
                            this->_order.push_back(node->child(unmatched[3])->id()-1);
                        }
                        this->collapse_node(node,-2);
                    }
                }

            }

        }
        else{
            // unmatched node must be matched with node 2
            success = false;
            int id = unode->id();
            assert(NULL == this->matchPair(unode,node->child(unmatched[2]),success));
            if(success){
                this->_order.push_back(id-1);
                this->_order.push_back(node->child(unmatched[2])->id()-1);
            }
            if(this->isReverseComplement(node->child(unmatched[2])->id(),node->child(unmatched[3])->id()))
                this->collapse_node(node,-2);
            else
                this->collapse_node(node,node->child(unmatched[3])->id());

            }
          }
        */
    }
}
}
