#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include "suffix_commons.h"
#include "read.h"
#include "treenode.h"
namespace prefixMatching{
class suffixTree
{
public:
    typedef BTDnaString Sequence;
public:
    suffixTree(const vector<std::shared_ptr<sequence>>& r);
    void insert(const Sequence&);
    size_t      size()const{return this->_total;}
    const vector<treeNode*>&  priority()const{ return this->_priority;}
    ~suffixTree(){
        if(_root){
            destroy(_root);
        }
    }
private:
    suffixTree(const suffixTree&);
    suffixTree& operator=(const suffixTree&);
private:
    inline int reverseComp(int base){
        return base == 4? base:(3-base);
    }
    void insertImp(treeNode*,const Sequence&,int,UINT16,UINT16);
    UINT16  matching(const Sequence&,int,UINT16,UINT16,
                  const Sequence&, UINT16,UINT16);
    inline int reverseComID(int id){
        if(id%2)
            return id + 1;
        else
            return id - 1;
    }
    void destroy(treeNode* node){
        if(node){
            if(node->isLeaf()){
                delete node;
                node= NULL;
            }
            else{
                for(int i = 0; i <= 4; i++){
                    destroy(node->child(i));
                    node->setChild(NULL,i);
                }
                delete node;
                node = NULL;
            }

        }
    }
private:
    vector<treeNode*>                                                 _priority;
    const vector<std::shared_ptr<sequence>>&                              _readPool;
    size_t                                                            _total;
    treeNode*                                                         _root;
};
}
#endif // SUFFIXTREE_H
