/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef PREPROCESS_H
#define PREPROCESS_H
#include "suffix_commons.h"
#include "formats.h"
#include "suffixreadsloader.h"
#include "filesplittor.h"
namespace prefixMatching{
/**
  * preProcess the reads file, split the
  * read file into three part if neccessary
*/
class preProcess
{
public:
    preProcess( std::string readsfile, std::string outprefix,file_format format);
    void split(std::vector<std::shared_ptr<sequence>>&);
    ~preProcess(){
        cout<<"Totally processed "<<this->_readparser->total()<<" reads in total!"<<endl;
        cout.flush();
    }
private:
    std::string                             _readsfile;
    std::string                             _outprefix;
    file_format                             _formats;
    std::shared_ptr<suffixReadsLoader>      _readparser;
};
}
#endif // PREPROCESS_H
