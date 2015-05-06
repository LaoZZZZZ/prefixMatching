/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef REORDERDRIVE_H
#define REORDERDRIVE_H
#include <memory>
#include <fstream>
#include "suffixparam.h"
#include "filesplittor.h"
#include "suffixreadsloader.h"
#include "linearmatching.h"
#include "treematching.h"
#include "treematchingmultipleway.h"
#include "snowball.h"
#include "timer.h"
#include "preprocess.h"
#include "sequenceOutput.h"
using std::fstream;
using std::shared_ptr;
using std::unique_ptr;
namespace prefixMatching{
/**
  * The mainframe work of this program
  * Basically it takes a sequence file
*/
class reorderDrive
{
public:
    reorderDrive(std::shared_ptr<suffixParam>& params);
    void drive(){
        this->_pool.clear();
        std::unique_ptr<matchingAlgorithm> alg;

        if(this->_params->isTest()){
            // this is for testing and time measuring
	    preProcess* pre = new preProcess(this->_params->readFile(),
                                        this->_params->outprefix()+"_original",
                                        this->_params->format());

            std::unique_ptr<preProcess> prepro(pre);
            prepro->split(this->_pool);
            //this->outputOriginal();
            this->_pool.clear();
            if(this->_params->isProcess()){
            	Timer* realtime = new realTimer(cout);
            	Timer* cputime = new cpuTimer(cout);
            	suffixReadsLoader* tmp = new suffixReadsLoader(RAWSEQ,this->_params->outprefix()+"_original_seq");
            	this->_readsLoader.reset(tmp);
            	this->_readsLoader->loadAll(this->_pool);

            	if(this->_params->dataStructure() == TREE)
                	alg.reset(new treeMatching(this->_pool,this->_order));
            	else if(this->_params->dataStructure() == TRIE)
                	alg.reset(new linearMatching(this->_pool,this->_order));
            	else if(this->_params->dataStructure() == TREEMULTIPLE)
                	alg.reset(new treeMatchingMultipleWay(this->_pool,this->_order));
            	else if(this->_params->dataStructure() == BLOCK){
                	alg.reset(new snowBall(this->_pool,this->_order));
            	}
            	else
                	throw 1;
            	alg->solution();

            	delete cputime;
            	delete realtime;
            	realtime = NULL;
            	cputime = NULL;
            	assert(this->_order.size() == this->_pool.size());
            	output();
	     }
        }else{
            Timer* realtime = new realTimer(cout);
            Timer* cputime = new cpuTimer(cout);
            suffixReadsLoader* tmp = NULL;
            tmp = new suffixReadsLoader(this->_params->format(),
                                                           this->_params->readFile());
            if(!tmp)
                throw bad_alloc();
            tmp->loadAll(this->_pool);
            if(this->_params->dataStructure() == TREE)
                alg.reset(new treeMatching(this->_pool,this->_order));
            else if(this->_params->dataStructure() == TRIE)
                alg.reset(new linearMatching(this->_pool,this->_order));
            else if(this->_params->dataStructure() == TREEMULTIPLE)
                alg.reset(new treeMatchingMultipleWay(this->_pool,this->_order));
            else if(this->_params->dataStructure() == BLOCK){
                alg.reset(new snowBall(this->_pool,this->_order));
            }
            else
                throw 1;
            alg->solution();
            delete cputime;
            delete realtime;
            realtime = NULL;
            cputime = NULL;
            assert(this->_order.size() == this->_pool.size());
            output();
        }

    }
    ~reorderDrive(){
    }
private:
    void outputOriginal(){
        string outprefix(this->_params->outprefix());

        string original_prefix = outprefix + "_original";
        std::unique_ptr<fileSplittor> original(new fileSplittor(this->_params->format(),original_prefix));
        original->split(this->_pool);
    }
    void init(){
        suffixReadsLoader* tmp = new suffixReadsLoader(this->_params->format(),this->_params->readFile());
        this->_readsLoader.reset(tmp);
    }
    void output(){
	if(this->_params->isTest()){
	
        	string outprefix(this->_params->outprefix());
        	string processed_prefix = outprefix + "_ordered";
		
        	std::unique_ptr<sequenceOutput> processed(new sequenceOutput(processed_prefix));
        	processed->orderSplit(this->_pool,this->_order);
	}
	else{
        	string outprefix(this->_params->outprefix());
        	string processed_prefix = outprefix + "_ordered";
        	std::unique_ptr<fileSplittor> processed(new fileSplittor(this->_params->format(),processed_prefix));
        	processed->orderSplit(this->_pool,this->_order);
	}
    }
private:
    std::shared_ptr<suffixParam>                      _params;
    std::unique_ptr<suffixReadsLoader>                _readsLoader;
    vector<std::shared_ptr<sequence>>                 _pool;
    vector<int>                                       _order;
    int                                               _unmatched;
};
}

#endif // REORDERDRIVE_H
