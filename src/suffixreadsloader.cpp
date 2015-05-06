/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#include "suffixreadsloader.h"

namespace prefixMatching{
suffixReadsLoader::suffixReadsLoader(file_format format,const string& file):_isfinished(false),_total(0)
{
    this->init(format,file);
}
void suffixReadsLoader::init(file_format format,const string& file){
    if(format == FASTQ){
        this->_parser.reset(new fastqPattern(file));
    }else if(format == FASTA){
        this->_parser.reset(new fastaPattern(file));
    }else if(format == RAWSEQ){
        this->_parser.reset(new rawPattern(file));
    }else{
        throw runtime_error("Currently not supported file format!\n");
    }
}
void suffixReadsLoader::loadReads(std::vector<std::shared_ptr<sequence>>& cont,size_t& total){
    if(this->_isfinished)
        return;
    else{
        size_t count = 0;
        while(count < total){
            std::shared_ptr<sequence> tmp;
            bool success(false);
            this->_parser->parse(tmp,success,this->_isfinished);
            if(success){
                cont.push_back(tmp);
                count++;
            }
            if(this->_isfinished)
                break;

        }
        total = count;
        this->_total += count;
    }
}
void suffixReadsLoader::loadAll(std::vector<std::shared_ptr<sequence>>& cont){
    size_t count = 0;
    while(!this->_isfinished){
        std::shared_ptr<sequence> tmp;
        bool success(false);
        this->_parser->parse(tmp,success,this->_isfinished);
        if(success){
            cont.push_back(tmp);
            count++;
        }
    }
    this->_total += count;
}
}
