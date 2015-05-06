/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef SUFFIXPARAM_H
#define SUFFIXPARAM_H
#include <string>
#include "formats.h"
using std::string;
namespace prefixMatching{
class suffixParam
{
public:
    suffixParam(const std::string& ,const std::string& ,file_format,structure,bool,bool);
    file_format     format()const{return this->_format;}
    string          readFile()const{return this->_readsfile;}
    string          outprefix()const{return this->_outprefix;}
    structure       dataStructure()const{return this->_structure;}
    // whether this run is a test
    // if it is a test running, then
    bool            isTest(void)const{return this->_test;}
    bool	    isProcess(void)const{return this->_process;}
private:
    string                          _readsfile;
    string                          _outprefix;
    file_format                     _format;
    structure                       _structure;
    bool                            _test;
    bool			    _process;
};
}
#endif // SUFFIXPARAM_H
