#pragma once

#include "../common/dependency_array.hpp"
#include <cassert>
#include "../common/union_find.hpp"
#include <unordered_set>
#include "./randstr.hpp"

void transformVector(std::vector<uint64_t> &item, std::vector<uint64_t> &sa, std::vector<uint64_t> &output);
void transformVector2(std::vector<uint64_t> &item, std::vector<uint64_t> &sa, std::vector<uint64_t> &output);