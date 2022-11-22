#pragma once

#include "../common/dependency_array.hpp"
#include <cassert>
#include "../common/union_find.hpp"
#include <unordered_set>
#include "debug/randstr.hpp"

void transformVector(vector<uint64_t> &item, vector<uint64_t> &sa, vector<uint64_t> &output);
void transformVector2(vector<uint64_t> &item, vector<uint64_t> &sa, vector<uint64_t> &output);