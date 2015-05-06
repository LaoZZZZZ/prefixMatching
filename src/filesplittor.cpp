/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#include "filesplittor.h"


namespace prefixMatching{
fileSplittor::fileSplittor(file_format s,string& outprefix):_format(s),_outprefix(outprefix)
{
    this->init();
}
void fileSplittor::init(){
    this->_bufseq.reset(new OutFileBuf(this->_outprefix + "_seq"));
    this->_bufid.reset(new OutFileBuf(this->_outprefix + "_id"));
    if(this->_format == FASTQ)
        this->_bufqual.reset(new OutFileBuf(this->_outprefix + "_qual"));

}
/**
  * split the reads into several parts and output them
*/
void fileSplittor::split(vector<std::shared_ptr<sequence> >& reads) {

    for(const std::shared_ptr<sequence>& r: reads){
        this->writeRead(r);
        //r->id().clear();
        //r->quality().clear();
    }
}

/**
  * given a list of array and a match list
  * output the reads into splited files according to the match list
*/
void fileSplittor::orderSplit(vector<std::shared_ptr<sequence> >& reads,vector<int>& order){

    size_t i = 0;
    //char* buf = new char[40];
    //memset(buf,0,40*sizeof(char));
    while(i < order.size()){
        bool rev = order[i]%2;
        //this->write(_bufid,order[i]);
        //sprintf(buf,"%d",order[i]);

        //this->_bufid->writeChars(buf);
        //this->_bufid->write('\n');
        this->writeRead(reads[order[i]/2],rev);
        i++;
        //memset(buf,0,40*sizeof(char));
    }
    //delete []buf;
    }
}
