#pragma once
#include "io.h"
#include <stack>
#include "../debug/print.hpp"

namespace stool
{

    namespace lzrr
    {
        /*
        Disjoint-set data structure
        https://en.wikipedia.org/wiki/Disjoint-set_data_structure
        */
        class UnionFind
        {
        private:
            std::vector<uint64_t> findTmpVec;
            std::stack<std::pair<uint64_t, uint64_t>> stack;
            std::vector<uint64_t> array;
            std::vector<uint8_t> rank_array;

        public:
            using GINDEX = uint64_t;

            uint64_t size()
            {
                return this->array.size();
            }
            UnionFind()
            {
            }
            // Construct size-groups whose each size is 1.
            void initialize(uint64_t size)
            {
                this->array.clear();
                this->array.resize(size);
                this->findTmpVec.resize(512);

                this->rank_array.resize(size, 1);

                for (uint64_t i = 0; i < size; i++)
                {
                    this->array[i] = i;
                }
            }
            // Return the group number of the i-th element.
            GINDEX find(uint64_t i)
            {
                uint64_t p = 0;
                while (true)
                {
                    if (this->array[i] == i)
                    {
                        break;
                    }
                    else
                    {
                        this->findTmpVec[p++] = i;
                        i = this->array[i];
                    }
                }

                for (uint64_t x = 0; x < p; x++)
                {
                    this->array[this->findTmpVec[x]] = i;
                }

                return i;
            }
            // Return the group number of the i-th element without updating the data structure.
            GINDEX findWithoutUpdate(uint64_t i)
            {
                while (true)
                {
                    if (this->array[i] == i)
                    {
                        break;
                    }
                    else
                    {
                        i = this->array[i];
                    }
                }
                return i;
            }

            // Unify two groups of i-th and j-th element and return the group number.
            GINDEX unionOperation(GINDEX i, GINDEX j)
            {
                if (this->rank_array[i] >= this->rank_array[j])
                {
                    this->array[j] = i;
                    if (this->rank_array[i] == this->rank_array[j])
                        this->rank_array[i]++;
                }
                else
                {
                    return this->unionOperation(j, i);
                }
                return this->find(i);
            }
            // Unify two groups of i-th and j-th element and return the group number.
            GINDEX unionOperationWithBackup(GINDEX i, GINDEX j)
            {
                this->stack.push(std::pair<uint64_t, uint64_t>(j, this->array[j]));
                this->array[j] = i;
                return this->findWithoutUpdate(i);
            }
            void add()
            {
                this->array.push_back(this->size());
                this->rank_array.push_back(1);
            }
            // Get the data structure when we used the last unionOperationWithBackup operation.
            void back()
            {
                while (this->stack.size() > 0)
                {
                    std::pair<uint64_t, uint64_t> top = this->stack.top();
                    this->array[top.first] = top.second;
                    this->stack.pop();
                }
            }
            std::string toString()
            {
                return stool::lzrr::Printer::toString(this->array, 2);
            }
        };
    }
}
