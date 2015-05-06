#ifndef TREENODE_H
#define TREENODE_H
#include "suffix_commons.h"
#include <list>
using std::list;
namespace prefixMatching {
class treeNode{
public:
    treeNode():_parent(NULL),_next(NULL),_start(0),_end(0){}
    ///////////////////////////////////////////////////
    /// \brief setter funcitons
    ///////////////////////////////////

    void  setNext(treeNode* n){this->_next = n;}
    void  setParent(treeNode* p){this->_parent = p;}
    void setStart(UINT16 s){this->_start = s;}
    void setEnd(UINT16 e){this->_end = e;}

    ///////////////////////////////////////////////////////////////
    /// virtual functions
    /////////////////////////////////
    virtual void  setChild(treeNode*){
        return ;
    }
    virtual void setChild(treeNode*,int){
        return ;
    }
    virtual void  setBranch(size_t){}
    virtual int id()const{return -1;}
    virtual bool isBranch()const{return false;}
    virtual int branch()const{return 1;}
    virtual void  setID(int){}
    virtual const treeNode*         child(size_t)const{return NULL;}
    virtual treeNode*         child(size_t ind){return NULL;}
    virtual bool isLeaf()const{return true;}
    virtual list<int>&  ids(){ throw 1;}
    ////////////////////////////////////////////////////////////////
    ////////////////////getter function ////////////////////////////
    ///////////////////////////////////////////////////////////////
    ///
    bool    isRoot()const{return this->_parent == NULL;}
    UINT16  start()const{return this->_start;}
    UINT16  end()const{return this->_end;}
    const treeNode* parent()const{return this->_parent;}
    treeNode* parent(){return this->_parent;}
    const treeNode* next()const{return this->_next;}
    treeNode* next(){return this->_next;}
    UINT16      depth()const{return this->_start;}
private:
    treeNode*                               _parent;
    treeNode*                               _next;
    // the range is [_start,end), closed at left but open at right end
    UINT16                                  _start;
    UINT16                                  _end;

};
class internalNode : public treeNode
{
public:
//    internalNode();
    internalNode(int id):_id(id),_branch(0){
        memset(this->_child,0,5*sizeof(treeNode*));
    }

    ///////////////////////////////////////////////////////////////
    /// virtual functions
    /////////////////////////////////

    bool isLeaf()const{return false;}
    virtual void  setBranch(size_t b){this->_branch = b;}
    virtual void setChild(treeNode*p,int ind){
        if(!this->_child[ind] && p)
            this->_branch++;
        this->_child[ind] = p;
    }
    void  setChild(treeNode* p){
            memcpy(&(this->_child),p,5*sizeof(treeNode*));
            for(size_t i = 0; i <= 4; i++){
                if(this->_child)
                    this->_branch++;
            }
        }
    virtual bool isBranch()const{return this->_branch >= 2;}
    virtual int branch()const{return this->_branch;}
    virtual int id()const{return this->_id;}

    virtual void  setID(int id){ this->_id = id;}

    /**
      getter function
      */

    const treeNode*         operator[](size_t ind)const{assert(ind<=4);return this->_child[ind];}
    treeNode*         operator[](size_t ind){assert(ind<=4);return this->_child[ind];}
    const treeNode*         child(size_t ind)const{assert(ind<=4);return this->_child[ind];}
    treeNode*         child(size_t ind){assert(ind<=4);return this->_child[ind];}
private:
    int                                     _id;  // sequence id
    treeNode*                               _child[5];
    UINT8                                   _branch;
};

class leafNode : public treeNode{
public:
    leafNode(int id):treeNode(){
        this->_ids.push_back(id);
    }
    virtual list<int>&  ids(){return _ids;}

    virtual int id()const{
        return this->_ids.front();
    }
    virtual void  setID(int i){
        this->_ids.push_back(i);
    }

private:
    list<int>                 _ids;
};
}

#endif // TREENODE_H
