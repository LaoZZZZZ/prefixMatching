#ifndef RAWPATTERN_H
#define RAWPATTERN_H
#include "patternparser.h"
namespace prefixMatching{
class rawPattern : public patternParser
{
public:
    rawPattern(std::string file);
private:
    void parseImp(std::shared_ptr<sequence>&,bool&,bool&);
private:
    bool                          first_;
    static const int              MAXLEN = 1000;
};
}
#endif // RAWPATTERN_H
