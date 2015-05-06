#include "treematchingmultipleway.h"

namespace prefixMatching{
treeMatchingMultipleWay::treeMatchingMultipleWay(std::vector<std::shared_ptr<sequence> >& r, vector<int>& m):matchingAlgorithm(r,m)
{
    this->init();
}
void treeMatchingMultipleWay::init(){
    this->_order.clear();

    this->_suffix.reset(new linkSuffixTree(this->_readsPool));
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
void treeMatchingMultipleWay::constructSolution(){

    linkNode* leaves = this->_suffix->leaf();
    // start from the deepest node, traverse the priority queue
    if(!leaves)
        return ;
    linkNode* head = leaves;
    assert(head);
    // remove those sequence that its reverse complementary is identical with other sequence
    while(head){
        list<int>& ids = head->ids();
        if(ids.size() > 1){
            for(const int& id: ids){
                if(this->_match[id-1] != -1)
                    this->_match[this->reverseComplement(id)-1] = -1;
            }
        }
        head = head->next();
    }
    head = leaves;
    while(head){
        list<int>& ids = head->ids();
        for(const int& id : ids){
            if(this->_match[id-1] != -1){
                this->_order.push_back(id-1);
                this->_match[this->reverseComplement(id)-1] = -1;
            }
        }
        head = head->next();
    }

}
}
