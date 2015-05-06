#include "rawpattern.h"

namespace prefixMatching{
rawPattern::rawPattern(std::string file):patternParser(file)
{
}
void rawPattern::parseImp(std::shared_ptr<sequence> & read, bool &succ, bool &done){
    if(this->fb_->eof()){
        succ = false;
        done = true;
    }else{
        char* buf = new char[rawPattern::MAXLEN];
        memset(buf,0,rawPattern::MAXLEN);
        Read* r = new Read();
        int len = this->fb_->gets(buf,rawPattern::MAXLEN);
        BTDnaString& sbuf = r->fowardSeq();
        sbuf.installChars(buf,len);
        read.reset(r);
        succ = true;
        done = this->fb_->eof();
        delete buf;
    }
}
}
