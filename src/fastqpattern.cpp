/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#include "fastqpattern.h"
namespace prefixMatching{

fastqPattern::fastqPattern(string file):patternParser(file),first_(true)
{
}
void fastqPattern::parseImp(std::shared_ptr<sequence>& read,bool& success,bool& done){

    int c;

    Read* r = new Read();
  //  success = false;
    //done = false;
    // Pick off the first at
    if(this->first_) {
        c = this->fb_->get();
        if(c != '@') {
            c = this->fb_->getPastNewline();
            if(c < 0) {
                delete r; r = NULL;success = false; done = true;
            }
        }
        if(c != '@') {
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        assert_eq('@', c);
        first_ = false;
    }
    // Read to the end of the id line, sticking everything after the '@'
    // into *name
    BTString& name = r->id();
    //BTString name;
    while(true) {

        c = this->fb_->get();
        if(c == '@')
            continue;
        if(c < 0) {
            delete r; r= NULL; success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        if(c == '\n' || c == '\r') {
            // Break at end of line, after consuming all \r's, \n's
            while(c == '\n' || c == '\r') {
                c = this->fb_->get();
                if(c < 0) {
                    delete r; r = NULL; success = false; done = true;
                    std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
                    throw 1;
                }
            }
            break;
        }
        name.append(c);
    }
    // fb_ now points just past the first character of a
    // sequence line, and c holds the first character
    BTDnaString& sbuf = r->fowardSeq();
    while(true){
        c = toupper(c);
        if(c < 0){
            delete r; r= NULL; success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        else{
            if(c == '\n' || c == '\r') {
                // Break at end of line, after consuming all \r's, \n's
                while(c == '\n' || c == '\r') {
                    c = this->fb_->get();
                    if(c < 0) {
                        delete r; r = NULL; success = false; done = true;
                        std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
                        throw 1;
                    }
                }
                break;
            }
            sbuf.append(asc2dna[c]);
            c = this->fb_->get();
        }
    }
    assert_eq('+', c);
    // skip the option sequence at '+' line
    c = this->fb_->getPastNewline();
    if(c <0){
        delete r; r = NULL; success = false; done = true;
        std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
        throw 1;
    }
    BTString& qual = r->quality();
    //BTString qual;
    while(true){
        qual.append(c);
        c = this->fb_->get();
        if(c == '\n' || c == '\r')
            break;
    }
    assert(qual.length() == sbuf.length());
    read.reset(r);
    success = true;
    done = this->fb_->eof();
}

}
