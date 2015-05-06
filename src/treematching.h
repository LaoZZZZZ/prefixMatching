#ifndef TREEMATCHING_H
#define TREEMATCHING_H
#include <unordered_map>
#include "suffix_commons.h"
#include "read.h"
#include "suffixtree.h"
#include "matchingalgorithm.h"
using std::unordered_map;
namespace prefixMatching{
class treeMatching : public matchingAlgorithm
{
public:
    treeMatching(std::vector<std::shared_ptr<sequence> >& r, vector<int>& m);
protected:
    void init();
private:
    virtual void constructSolution();
    void match(treeNode*);
    void    collapse_node(treeNode* node,int id){
        if(node){
            do{
                node->setID(id);
                node = node->parent();
            }while(node && !node->isBranch() && node->id() >= 0);
         }
    }

    treeNode* matchPair(treeNode* pair1,treeNode* pair2,bool& success){
        int id1 = pair1->id();
        int id2 = pair2->id();
    // if they are reverse complement
        if(isReverseComplement(id1,id2)){
            if(this->isMatched(id1)){
                return NULL;
                success = false; // not matched
            }
            else
                return pair1;
        }
        // if pair1 is matched already
        if(this->isMatched(id1)){
            if(this->isMatched(id2)){
                return NULL;  // not matched
                success = false;
            }
            else
                return pair2;
        }
        else{ // if pair1 is not matched yet
            if(this->isMatched(id2)) //if pair2 is matched
                return pair1;
            // if neither pair1 and pair2 are matched
                this->_match[id1-1] = id2-1;
                this->_match[id2-1] = id1-1;
                success= true;

                return NULL;
        }
    }
protected:
    std::shared_ptr<suffixTree>                     _suffix;
};
}
#endif // TREEMATCHING_H
