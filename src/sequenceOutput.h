/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef SEQUENCEOUTPUT_H
#define SEQUENCEOUTPUT_H

#include <string>
#include <vector>
#include <memory>
#include "formats.h"
#include "read.h"
using std::string;
using std::vector;
using std::shared_ptr;


namespace prefixMatching{

/**
  * Given a outprefix and outfile format
  * this class could write given reads into corresponding outfiles
  * in specified order or default order
  *
  * Currently, it supports two different file format
  * One is fasta, which only contains read id and sequence
  * The other is fastq, which contains read id, sequence and quality
*/
class sequenceOutput
{
public:
    explicit sequenceOutput(string& outprefix);
    void split(vector<std::shared_ptr<sequence> >&) ;
    void orderSplit(const std::vector<std::shared_ptr<sequence> >&,const vector<int>&);
    ~sequenceOutput(){

    }
private:

    void init();

    template<typename T>
    void write(std::shared_ptr<OutFileBuf>& handler, T& t){
        if(handler.get()){
            handler->writeString(t);
            handler->write('\n');
        }
    }
    void writeRead(const std::shared_ptr<sequence>& r,bool rc=false){
        if(rc){
            BTDnaString Rc;
            r->reverseComSeq(Rc);
            this->write(this->_bufseq,Rc);
        }
        else{ // original read
            this->write(this->_bufseq,r->fowardSeq());
        }
    }
private:
    string                                      _outprefix;
    std::shared_ptr<OutFileBuf>                 _bufseq;
};
}
#endif // sequenceOutput_H
