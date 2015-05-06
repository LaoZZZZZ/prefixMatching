#include "linksuffixtree.h"

namespace prefixMatching{
linkSuffixTree::linkSuffixTree(const vector<std::shared_ptr<sequence>>& r):_readPool(r),_root(0),_leafHead(0),_total(0)
{
    this->_root = new linkInternalNode(0);
    this->_leafHead = new linkLeafNode(0);
    this->_priority.push_back(this->_root);

}

UINT16  linkSuffixTree::matching(const Sequence& seq1,int id1,UINT16 start1,UINT16 end1,
              const Sequence& seq2, UINT16 start2,UINT16 end2){

    int matched = 0;
    // reverse sequence
    if(id1%2 == 0){
        int length = seq1.length()-1;
        while(start1 < end1 && start2 < end2 && this->reverseComp(seq1[length-start1]) == seq2[start2]){
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
// need more work
void linkSuffixTree::insertImp(linkNode* node,const Sequence& seq,int id,UINT16 start,UINT16 end){

    linkNode* child = node->child(seq[start]);
    if(child){
        //const Sequence& seq = this->_readPool[child->id()/2]->fowardSeq();
        int cid = child->id();
        UINT16 start1 = child->start();
        UINT16 end1 = child->end();
        Sequence& seq1 = this->_readPool[(cid-1)/2]->fowardSeq();
        size_t len1 = seq1.length();
        UINT16 matched = this->matching(seq1,cid,start1,end1,
                                     seq,start,end);
        assert(matched);
        if(matched + start >= seq.length()){ //reach the leaf node, everything is matched
            assert(child->isLeaf());
            child->addID(id);
        }
        else if(matched + start < end1){ //  at the middle of the range
            //assert(matched >= 1);
            bool forward = cid%2;
            // create internal node
            linkNode* tmp = new linkInternalNode(cid);
            tmp->setStart(start);
            tmp->setEnd(start + matched);


            // rewire this child to the tmp child
            int c = 0;
            if(forward){
                c = this->_readPool[(cid-1)/2]->fowardSeq()[start1 + matched];
            }
            else{
                c = this->reverseComp(this->_readPool[(cid-1)/2]->fowardSeq()[len1 - start1 - matched - 1]);
            }
            tmp->setChild(child,c);

            // update the range of this child
            child->setStart(start1 + matched);

            // create the new leaf node
            linkNode* leaf = new linkLeafNode(id);
            leaf->setStart(start + matched);
            leaf->setEnd(end);
            int cn = seq[start + matched];
            tmp->setChild(leaf,cn);
            assert(cn != c);
            // update the link list
            if(cn < c){
                // find the right after leafnode
                linkNode* right = this->findMostLeftChild(child);
                assert(right && right->Prev());
                right->Prev()->setNext(leaf);
                leaf->setPrev(right->Prev());
                leaf->setNext(right);
                right->setPrev(leaf);

            }
            else{

                // find the right before leaf node
                linkNode* left = this->findMostRightChild(child);
                assert(left);
                linkNode* next = left->next();
                left->setNext(leaf);
                leaf->setPrev(left);
                leaf->setNext(next);
                if(next)
                    next->setPrev(leaf);
            }
            node->setChild(tmp,seq[start]);
            //update the priority
            while(this->_priority.size() < tmp->end()){
                this->_priority.push_back(NULL);
            }
            linkNode* head = this->_priority[tmp->end()-1];
            tmp->setNext(head);
            this->_priority[tmp->end()-1] = tmp;
        }
        else{ // perfect match at this range, move to next child
            assert(matched+start1 == child->end());
            assert(matched+start < end);
            this->insertImp(child,seq,id,matched+start,end);
        }

    }else{ // there is no child that start with the input sequence

        // create new leaf node
        linkNode* tmp = new linkLeafNode(id);
        tmp->setStart(start);
        tmp->setEnd(end);
        //update the node child
        node->setChild(tmp,seq[start]);
        // if the first child
        if(!(this->_leafHead->next())){
            this->_leafHead->setNext(tmp);
            tmp->setPrev(this->_leafHead);
        }else{
            // update the linklist
            int c = seq[start];
            linkNode* prev = NULL;
            for(int i = c-1; i >= 0; i--){
                prev = node->child(i);
                if(prev)
                    break;
            }
            // search the right before leaf node on the same branch
            if(prev){
                linkNode* left = this->findMostRightChild(prev);
                linkNode* next = left->next();
                left->setNext(tmp);
                tmp->setPrev(left);
                tmp->setNext(next);
                if(next)
                    next->setPrev(tmp);
            }else{
                for(int i = c+1; i <= 4; i++){
                    prev = node->child(i);
                    if(prev)
                        break;
                }
                assert(prev);
                linkNode* after = this->findMostLeftChild(prev);
                assert(after && after->Prev());
                after->Prev()->setNext(tmp);
                tmp->setPrev(after->Prev());
                after->setPrev(tmp);
                tmp->setNext(after);
            }
        }

    }

}

}
