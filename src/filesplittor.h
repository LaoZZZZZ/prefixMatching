/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef FILESPLITTOR_H
#define FILESPLITTOR_H

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
class fileSplittor
{
public:
    explicit fileSplittor(file_format s,string& outprefix);
    void split(vector<std::shared_ptr<sequence> >&) ;
    void orderSplit(vector<std::shared_ptr<sequence> >& ,vector<int>&);
    ~fileSplittor(){

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
        this->_bufid->write('@');
        this->write(this->_bufid,r->id());
        // reverse complement
        if(rc){
            BTDnaString Rc;
            r->reverseComSeq(Rc);
            this->write(this->_bufseq,Rc);
            r->quality().reverse();
            this->write(this->_bufqual,r->quality());
        }
        else{ // original read
            this->write(this->_bufseq,r->fowardSeq());
            this->write(this->_bufqual,r->quality());
        }
    }
private:

    file_format                                 _format;
    string                                      _outprefix;
    std::shared_ptr<OutFileBuf>                 _bufid;
    std::shared_ptr<OutFileBuf>                 _bufseq;
    std::shared_ptr<OutFileBuf>                 _bufqual;
};
}
#endif // FILESPLITTOR_H
