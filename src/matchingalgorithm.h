#ifndef MATCHINGALGORITHM_H
#define MATCHINGALGORITHM_H
#include <unordered_map>
#include "suffix_commons.h"
#include "read.h"
using std::unordered_map;
namespace prefixMatching{
class matchingAlgorithm
{
public:
    matchingAlgorithm(const vector<std::shared_ptr<sequence> >&,vector<int>& );
    void solution(){this->constructSolution();}
    virtual ~matchingAlgorithm(){
        std::cout<<"There are totally "<<this->_unmatched<<" reads that are not matched!!!"<<std::endl;
    }
private:
    virtual void constructSolution(){}

protected:
    void OutputOrder();
    inline bool isReverseComplement(int id1,int id2){
        assert(id1 > 0);
        assert(id2 > 0);
        return (id1%2 && id2 - id1 == 1) ||
               (id2 %2 && id1 - id2 == 1);
    }
    inline int reverseComplement(int id){
        assert(id > 0);
        if(id%2)
            return id+1;
        else
            return id-1;
    }
    bool isMatched(int ind){
        if(ind < 0) return true;
        if(ind%2){
            return this->_match[ind]!= ind || this->_match[ind-1]!= ind-1;
        }
        else{
            return this->_match[ind-1]!= ind-1 || this->_match[ind-2]!= ind-2;
        }
    }
protected:
    const vector<std::shared_ptr<sequence> >&             _readsPool;
    vector<int>&                                          _order;
    vector<int>                                           _match;
    int                                                   _unmatched;

};
}
#endif // MATCHINGALGORITHM_H
