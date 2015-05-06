#ifndef SEQUENCEBLOCK_H
#define SEQUENCEBLOCK_H
#include "suffix_commons.h"
namespace prefixMatching{
class sequenceBlock
{
public:
    sequenceBlock(const list<int>& id);
    sequenceBlock(int id){this->_order.push_back(id);}
    const int& head()const{return this->_order.front();}
    const int& tail()const{return this->_order.back();}
    void mergeOnLeft(const sequenceBlock* b){
        this->_order.insert(this->_order.begin(),b->ids().begin(),b->ids().end());
    }

    void mergeOnright(const sequenceBlock* b){
        this->_order.insert(this->_order.end(),b->ids().begin(),b->ids().end());
    }
    void addOnLeft(int id){
        this->_order.push_front(id);
    }

    void addOnRight(int id){
        this->_order.push_back(id);
    }
    const list<int>& ids()const{return this->_order;}
    list<int>& ids() {return this->_order;}

private:
    list<int>                   _order;
};
}
#endif // SEQUENCEBLOCK_H
