#pragma once

namespace stool{
using TINDEX = uint64_t;
using SINDEX = uint64_t;

extern uint64_t changedRepCount;
extern uint64_t rlcpCounter;

extern uint64_t intervalTotalCount;

const uint64_t FACTORCHAR = UINT64_MAX - 1;
const uint64_t LOOPDETECT = UINT64_MAX - 2;

const uint64_t NOTFACTOR = UINT64_MAX;
const uint64_t FACTORIZED = UINT64_MAX;
}