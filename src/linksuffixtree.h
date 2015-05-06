#ifndef LINKSUFFIXTREE_H
#define LINKSUFFIXTREE_H
#include "linknode.h"
#include "read.h"

namespace prefixMatching{
class linkSuffixTree
{
    typedef BTDnaString Sequence;
public:
    linkSuffixTree(const vector<std::shared_ptr<sequence>>& r);
    void insert(const Sequence& seq){
        this->_total += 1;
        this->insertImp(_root,seq,this->_total,0,seq.length());
    }
    const linkNode*  leaf()const{return (this->_leafHead->next());}
    linkNode*  leaf(){
        linkNode* n = this->_leafHead->next();
        return n;}
    ~linkSuffixTree(){
        if(_root){
            destroy(_root);
        }
    }
    void insertImp(linkNode*,const Sequence&,int,UINT16,UINT16);
    UINT16  matching(const Sequence&,int,UINT16,UINT16,
                  const Sequence&, UINT16,UINT16);
    const vector<linkNode*>&        priority()const{return this->_priority;}

    vector<linkNode*>&        priority(){return this->_priority;}
private:
    inline int reverseComp(int base){
        return base == 4? base:(3-base);
    }
    void destroy(linkNode* node){
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
    linkNode* findMostRightChild(linkNode* head){
        if(head){
            if(head->isLeaf()) return head;
            for(int i = 4; i >=0; i--){
                if(head->child(i) && head->child(i)->isLeaf()){
                    return head->child(i);
                }
                else if(head->child(i) && !head->child(i)->isLeaf())
                    return findMostRightChild(head->child(i));
            }
        }
        return NULL;
    }

    linkNode* findMostLeftChild(linkNode* head){
        if(head){
            if(head->isLeaf()) return head;
            for(int i = 0; i <=4; i++){
                if(head->child(i) && head->child(i)->isLeaf()){
                    return head->child(i);
                }
                else if(head->child(i) && !head->child(i)->isLeaf())
                    return findMostLeftChild(head->child(i));
            }
        }
        return NULL;
    }
private:
    const vector<std::shared_ptr<sequence>>&                          _readPool;
    linkNode*                                                         _root;
    linkNode*                                                         _leafHead;
    int                                                               _total;
    vector<linkNode*>                                                 _priority;
};
}
#endif // LINKSUFFIXTREE_H
