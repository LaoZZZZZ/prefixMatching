/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef PATTERNPARSER_H
#define PATTERNPARSER_H

#include <memory>
#include <string>
#include <fstream>
#include <stdio.h>
#include "filebuf.h"
#include "read.h"
using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::ifstream;
namespace prefixMatching{
/**
  * a general pattern that parse the sequence file
  * Its subclass is designed to handle a specific file format,like fastq,fasta..
*/
class patternParser
{
public:
    patternParser(string file):_file(file),_nakeHandler(NULL){
        _nakeHandler = fopen(this->_file.c_str(),"r");
        if(!(_nakeHandler)){
            throw runtime_error(string("can not open file ") + _file);
        }
        fb_.reset(new FileBuf(this->_nakeHandler));
    }
    void parse(std::shared_ptr<sequence>& r,bool& success,bool& done){
        this->parseImp(r,success,done);
    }
    virtual ~patternParser(){
        fclose(this->_nakeHandler);
    }
private:
    virtual void parseImp(std::shared_ptr<sequence>&,bool&,bool&){throw 1;}
    patternParser(const patternParser&);
    patternParser& operator=(const patternParser&);
protected:
    std::unique_ptr<FileBuf>                fb_;
private:
    string                                  _file;
    FILE*                                   _nakeHandler;
};
}
#endif // PATTERNPARSER_H
