#ifndef SNOWBALL_H
#define SNOWBALL_H
#include "suffix_commons.h"
#include "read.h"
#include "suffixtree.h"
//#include "sequenceblock.h"
//#include "treematching.h"
#include "treematchingmultipleway.h"
namespace prefixMatching{
class snowBall : public treeMatchingMultipleWay
{
public:
    snowBall(std::vector<std::shared_ptr<sequence> >& r, vector<int>& m);
private:
    void constructSolution();
    void match(linkNode *);
    void finalizeOrder();
private:
    list<int>                                       _tmporder;
};
}
#endif // SNOWBALL_H
