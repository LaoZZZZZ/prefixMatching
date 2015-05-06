#include <iostream>

#include "reorderdrive.h"
#include "treenode.h"
using namespace std;
using namespace prefixMatching;
/**
  * main process
*/
void drive(string readsfile,file_format format,string outprefix,structure suffix,bool test,bool process){

    vector<string> fq;

    fq.push_back(readsfile);

    std::shared_ptr<suffixParam> param(new suffixParam(readsfile,outprefix,format,suffix,test,process));

    std::unique_ptr<reorderDrive> order(new reorderDrive(param));

    order->drive();
}

/**
*/
int main(int argc,char* argv[])
{
    assert(argc >= 6);
    string readsfile(argv[1]);// = "/home/laozzzzz/suffix_matching/reads/1000.fq";


    file_format format(static_cast<file_format>(atoi(argv[2])));// = FASTQ;


    string outprefix(argv[3]);// = "/home/laozzzzz/suffix_matching_memory/result/test";

    structure s(static_cast<structure>(atoi(argv[4])));
    bool test(static_cast<bool>(atoi(argv[5])));
    bool process = true;
    if(argc >=7 )
    	process = static_cast<bool>(atoi(argv[6]));
    /*
    string readsfile = "/home/laozzzzz/suffix_matching/reads/quater.fq";
    file_format format =  FASTQ;
    structure s = BLOCK ;
    bool test = true;
    // structure s = TREEMULTIPLE;
    string outprefix = "/home/laozzzzz/suffix_matching_memory/result/test";
    */
    drive(readsfile,format,outprefix,s,test,process);
    return 0;
}

