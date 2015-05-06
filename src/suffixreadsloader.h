/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef SUFFIXREADSLOADER_H
#define SUFFIXREADSLOADER_H

#include<vector>
#include <list>
#include <memory>
#include "pattern.h"

using std::list;
using std::vector;
using std::shared_ptr;
namespace prefixMatching{
/**
  * a sequence reads loader
  * based on the file name and format
  * it could supply a way to load the reads from the file
*/
class suffixReadsLoader
{
public:
    suffixReadsLoader(file_format format,const string& file);
    // need more work
    void loadReads(std::vector<std::shared_ptr<sequence>>& cont,size_t& total);
    //void loadReads(std::list<std::shared_ptr<Read>>& cont,size_t& total);
    void loadAll(std::vector<std::shared_ptr<sequence>>& cont);
    bool isDone()const{return this->_isfinished;}
    size_t total()const{return this->_total;}
    ~suffixReadsLoader(){
    }
private:
    void init(file_format,const string&);
    suffixReadsLoader(const suffixReadsLoader&);
    suffixReadsLoader& operator=(const suffixReadsLoader&);
private:
    bool                                        _isfinished;
    size_t                                      _total;
    std::unique_ptr<patternParser>              _parser;
};
}
#endif // SUFFIXREADSLOADER_H
