#include "matchingalgorithm.h"

namespace prefixMatching{
matchingAlgorithm::matchingAlgorithm(const vector<std::shared_ptr<sequence> >& reads,vector<int>& order):_readsPool(reads),_order(order)
{
    this->_unmatched = 0;
}
void matchingAlgorithm::OutputOrder(){

    size_t i = 0;
    while(i < this->_match.size()-1){
        int revCom = i+1;
        int target = i;
        if(this->_match[target] >= 0 && this->_match[revCom] >= 0){

            if(this->_match[i] != i && this->_match[revCom] == revCom){
                target = i;
            }else if(this->_match[i] == i && this->_match[revCom] != revCom){
                target = revCom;
            }else if(this->_match[i] != i && this->_match[revCom] != revCom){
                std::cerr<<" duplicate mathcing for "<<i<<'\t'<<this->_match[i] <<endl;
                std::cerr<<" duplicate mathcing for "<<revCom<<'\t'<<this->_match[revCom]<<endl;

            }else{
               // not matched
               this->_unmatched++;
               target = -1;
            }
           // matched
            if(target >= 0){
                if( target < this->_match[target]){
                    this->_order.push_back(target);
                    this->_order.push_back(this->_match[target]);
                }
            }else{
                this->_order.push_back(i);
            }

        }
        i += 2;
    }
}

}
