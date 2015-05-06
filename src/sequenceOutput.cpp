/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#include "sequenceOutput.h"


namespace prefixMatching{
sequenceOutput::sequenceOutput(string& outprefix):_outprefix(outprefix)
{
    this->init();
}
void sequenceOutput::init(){
    this->_bufseq.reset(new OutFileBuf(this->_outprefix + "_seq"));

}
/**
  * split the reads into several parts and output them
*/
void sequenceOutput::split(vector<std::shared_ptr<sequence> >& reads) {

    for(const std::shared_ptr<sequence>& r: reads){
        this->writeRead(r);
    }
}

/**
  * given a list of array and a match list
  * output the reads into splited files according to the match list
*/
void sequenceOutput::orderSplit(const std::vector<std::shared_ptr<sequence> >& reads,const std::vector<int>& order){

    size_t i = 0;
    while(i < order.size()){
        bool rev = order[i]%2;

        this->writeRead(reads[order[i]/2],rev);
        i++;
    }
    //delete []buf;
    }
}
