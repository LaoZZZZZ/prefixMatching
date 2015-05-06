#include "suffixtree.h"

namespace prefixMatching{
suffixTree::suffixTree(const vector<std::shared_ptr<sequence>>& r):_readPool(r),_total(0)
{
    _root = new internalNode(0);
    this->_priority.push_back(_root);
}
void suffixTree::insert(const Sequence& seq){
    this->_total += 1;
    this->insertImp(_root,seq,this->_total,0,seq.length());
}
UINT16  suffixTree::matching(const Sequence& seq1,int id1,UINT16 start1,UINT16 end1,
              const Sequence& seq2, UINT16 start2,UINT16 end2){

    int matched = 0;

    // reverse sequence
    if(id1%2 == 0){
        int length = seq1.length()-1;
        while(start1 < end1 && start2 < end2 && this->reverseComp(seq1[length - start1])== seq2[start2]){
            matched += 1;
            start1++;
            start2++;
        }

    }else{
        while(start1 < end1 && start2 < end2 && seq1[start1] == seq2[start2]){
            matched++;
            start1++;
            start2++;
        }
    }
    return matched;

}
void suffixTree::insertImp(treeNode* node,const Sequence& seq,int id,UINT16 start,UINT16 end){

    treeNode* child = node->child(seq[start]);
    if(child){
        //const Sequence& seq = this->_readPool[child->id()/2]->fowardSeq();
        int cid = child->id();
        UINT16 start1 = child->start();
        UINT16 end1 = child->end();
        Sequence& seq1 = this->_readPool[(cid-1)/2]->fowardSeq();
        size_t len1 = seq1.length();
        UINT16 matched = this->matching(seq1,cid,start1,end1,
                                     seq,start,end);
        if(matched + start >= seq.length()){ //reach the leaf node, everything is matched
            assert(child->isLeaf());
            child->setID(id);
        }
        else if(matched + start < end1){ // break at the middle of the range
            assert(matched >= 1);
            bool forward = cid%2;
            treeNode* tmp = new internalNode(cid);
            tmp->setParent(node);
            node->setChild(tmp,seq[start]);
            tmp->setStart(start);
            tmp->setEnd(start + matched);

            int c = 0;
            if(forward)
                c = this->_readPool[(cid-1)/2]->fowardSeq()[start1 + matched];
            else
                c = this->reverseComp(this->_readPool[(cid-1)/2]->fowardSeq()[len1 - start1 - matched - 1]);
            tmp->setChild(child,c);
            child->setStart(start1 + matched);
            child->setParent(tmp);
            //tmp->setChild(child,seq[start+matched]);
            treeNode* leaf = new leafNode(id);
            leaf->setParent(tmp);
            leaf->setStart(start + matched);
            leaf->setEnd(end);
            c = seq[start + matched];
            tmp->setChild(leaf,c);
            if(this->_priority.size() < end){
                while(this->_priority.size() < end){
                    this->_priority.push_back(NULL);
                }
            }

            node = this->_priority[end-1];
            leaf->setNext(node);
            this->_priority[end-1] = leaf;

            //update the
            node = this->_priority[tmp->end() - 1];
            tmp->setNext(node);
            this->_priority[tmp->end()-1] = tmp;
        }
        else{ // perfect match at this range, move to next child
            assert(matched+start == child->end());
            assert(matched+start < end);
            this->insertImp(child,seq,id,matched+start,end);
        }

    }else{
        treeNode* tmp = new leafNode(id);
        tmp->setParent(node);
        tmp->setStart(start);
        tmp->setEnd(end);
        node->setChild(tmp,seq[start]);
        if(this->_priority.size() < end){
            while(this->_priority.size() < end){
                this->_priority.push_back(NULL);
            }

        }
        if(node->branch() == 2 && node->id() != 0){
            treeNode* head = this->_priority[node->end() - 1];
            node->setNext(head);
            this->_priority[node->end()-1] = node;
        }
        node = this->_priority[end-1];
        tmp->setNext(node);
        this->_priority[end-1] = tmp;

    }
}

}
