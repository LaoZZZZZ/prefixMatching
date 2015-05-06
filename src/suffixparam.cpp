/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#include "suffixparam.h"


namespace prefixMatching{
suffixParam::suffixParam(const std::string& readfile,const std::string& outprefix,
                         file_format format=FASTQ, structure d = TRIE,bool test = true,bool process=true):_readsfile(readfile),_outprefix(outprefix),
                        _format(format),_structure(d),_test(test),_process(process)
{
}
}
