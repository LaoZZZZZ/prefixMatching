/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#include "reorderdrive.h"

namespace prefixMatching{
reorderDrive::reorderDrive(std::shared_ptr<suffixParam>& params):_params(params)
{
    this->init();
}
}
