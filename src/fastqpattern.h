/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */

#ifndef FASTQPATTERN_H
#define FASTQPATTERN_H

#include "patternparser.h"
namespace prefixMatching{
class fastqPattern : public patternParser
{
public:
    fastqPattern(string file);
private:
    void parseImp(std::shared_ptr<sequence>&,bool&,bool&);
private:
    bool                    first_;
};
}
#endif // FASTQPATTERN_H
