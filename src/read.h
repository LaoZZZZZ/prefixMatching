/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */

#ifndef READ_H_
#define READ_H_

#include <stdint.h>
#include <sys/time.h>
#include "sequence.h"

namespace prefixMatching {

typedef UINT64 TReadId;
typedef size_t TReadOff;
typedef int64_t TAlScore;


/**
 * A buffer for keeping all relevant information about a single read.
 */
class Read :public sequence {
public:
	Read() { reset(); }

	Read(const char *nm, const char *seq, const char *ql) { init(nm, seq, ql); }
    ~Read(){
        this->reset();
    }
    BTString& id(){return this->name;}
    BTString& quality(){return this->qual;}
private:
    void reset() {
        this->clear();
        qual.clear();
        name.clear();
    }
    /**
     * Simple init function, used for testing.
     */
    void init(
        const char *nm,
        const char *seq,
        const char *ql)
    {
        reset();
        this->set(seq);
        qual.install(ql);
        assert(this->length() == qual.length());
        if(nm != NULL) name.install(nm);
    }

private:

    BTString                    qual;             // quality values
    BTString                    name;      // read name

};


}
#endif /*READ_H_*/
