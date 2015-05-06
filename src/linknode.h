#ifndef LINKNODE_H
#define LINKNODE_H
#include "suffix_commons.h"
#include <list>
using std::list;
namespace prefixMatching{
class linkNode
{
public:
    linkNode():_start(0),_end(0),_next(0){}
    ///////////////////////////////////////////////////
    /// \brief setter funcitons
    ///////////////////////////////////

    void setStart(UINT16 s){this->_start = s;}
    void setEnd(UINT16 e){this->_end = e;}
    bool  isRoot()const{return this->_start == 0 && this->_end == 0;}
    ///////////////////////////////////////////////////////////////
    /// virtual functions
    //////////////////////////////////////////////////////////////
    void  setNext(linkNode* n){this->_next = n;}
    virtual void setPrev(linkNode* ){throw 1;}
    linkNode* next(){ return this->_next;}
    virtual linkNode* Prev(){throw 1;}
    virtual void  setChild(linkNode*,int){
        throw 1;
    }
    virtual void  addID(int){ throw 1;}
    virtual const linkNode*         child(size_t)const{return NULL;}
    virtual linkNode*         child(size_t ind){return NULL;}
    virtual bool isLeaf()const{return false;}
    virtual list<int>&  ids(){ throw 1;}
    virtual int         id()const{throw 1;}
    ////////////////////////////////////////////////////////////////
    ////////////////////getter function ////////////////////////////
    ///////////////////////////////////////////////////////////////
    ///
    UINT16  start()const{return this->_start;}
    UINT16  end()const{return this->_end;}
    const linkNode* next()const{return this->_next;}


private:
    // the range is [_start,end), closed at left but open at right end
    UINT16                                  _start;
    UINT16                                  _end;
    linkNode*                               _next;

};
class linkInternalNode : public linkNode{
public:
    linkInternalNode(int id):linkNode(),_id(id){
        memset(this->_child,0,sizeof(linkNode*)*5);
    }
    int id()const{return this->_id;}
    linkInternalNode():linkNode(){
        memset(this->_child,0,sizeof(linkNode*)*5);
    }
    virtual const linkNode*         child(size_t ind)const{
        assert(ind<=4);
        return this->_child[ind];
    }
    virtual linkNode*         child(size_t ind){
        assert(ind<=4);
        return this->_child[ind];
    }
    void addID(int id){this->_id = id;}
    virtual void setChild(linkNode* c,int ind){
        assert(ind <=4);
        this->_child[ind] = c;
    }
private:
    linkNode*                                _child[5];
    int                                      _id;

};
class linkLeafNode : public linkNode{
public:
    linkLeafNode(int id):linkNode(),_prev(0){
        this->_ids.push_back(id);
    }
    int id()const{return this->_ids.front();}
    virtual void setPrev(linkNode* n){this->_prev = n;}
    virtual linkNode* Prev(){return _prev;}
    virtual void  addID(int id){this->_ids.push_back(id);}
    virtual bool isLeaf()const{return true;}
    virtual std::list<int>&  ids(){ return _ids;}
private:
    linkNode*                   _prev;
    list<int>                   _ids;
};
}
#endif // LINKNODE_H
