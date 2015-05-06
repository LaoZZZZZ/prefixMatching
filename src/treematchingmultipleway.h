#ifndef TREEMATCHINGMULTIPLEWAY_H
#define TREEMATCHINGMULTIPLEWAY_H
#include "matchingalgorithm.h"
#include "linksuffixtree.h"
namespace prefixMatching{
class treeMatchingMultipleWay : public matchingAlgorithm
{
public:
    treeMatchingMultipleWay(std::vector<std::shared_ptr<sequence> >& r, vector<int>& m);
    virtual ~treeMatchingMultipleWay(){}
private:
    virtual void constructSolution();
    void init();
protected:
    std::shared_ptr<linkSuffixTree>                             _suffix;
};
}
#endif // TREEMATCHINGMULTIPLEWAY_H
