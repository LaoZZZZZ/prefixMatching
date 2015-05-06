#include "preprocess.h"

namespace prefixMatching{
preProcess::preProcess( std::string readsfile, std::string outprefix,file_format format):
                        _readsfile(readsfile),_outprefix(outprefix),_formats(format),_readparser(NULL)
{
}
void preProcess::split(std::vector<std::shared_ptr<sequence>>& reads){
    this->_readparser.reset(new suffixReadsLoader(this->_formats,this->_readsfile));
    std::shared_ptr<fileSplittor> splittor(new fileSplittor(this->_formats,this->_outprefix));
    reads.clear();
    size_t block = 1000;
    size_t batch = 1000;
    bool finished = false;
    while(!finished){
       this->_readparser->loadReads(reads,batch);
       finished = (batch < block); 
        //this->_readparser->loadAll(reads);
       if(reads.size())
		splittor->split(reads);
	reads.clear();
    }
}
}
