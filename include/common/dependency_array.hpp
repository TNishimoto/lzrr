#pragma once
#include <cassert>
#include <unordered_map>
#include "./constants.hpp"
#include "lz_factor.hpp"
#include "union_find.hpp"

namespace stool
{

    namespace lzrr
    {
        /*
        This is the data structure to emulate dependency array of the input text.
        */
        class DependencyArrayManager
        {
        private:
            // uf.find(i) == uf.find(j) if DA[i] == DA[j].
            UnionFind uf;
            // DA[i] = dependencyArrayForGroups[uf.find(i)]
            std::vector<uint64_t> dependencyArrayForGroups;
            // If T[i] is a factor not having a reference then T[i] = factorChars[i].
            std::unordered_map<uint64_t, char> factorChars;
            std::vector<uint64_t> smallDA;
            std::stack<uint64_t> smallStack;

            bool isUsingDependencyArrayForGroups()
            {
                return this->dependencyArrayForGroups.size() > 0;
            }
            // Return DA[i].
            TINDEX access(TINDEX i)
            {
                if (!this->isUsingDependencyArrayForGroups())
                    throw -1;

                uint64_t index = this->uf.find(i);
                uint64_t p = this->dependencyArrayForGroups[index];
                return p;
            }

        public:
            // Initialize this data structure.
            void initialize(uint64_t size, bool usingDependIndexes)
            {
                this->uf.initialize(size);

                if (usingDependIndexes)
                {
                    this->dependencyArrayForGroups.resize(size);
                    for (uint64_t i = 0; i < this->size(); i++)
                    {
                        this->dependencyArrayForGroups[i] = i;
                    }
                    this->smallDA.resize(256);
                }
            }

            // Return the size of the input text.
            uint64_t size()
            {
                return this->uf.size();
            }
            // Return T[i].
            char getChar(TINDEX i)
            {
                if (!this->isUsingDependencyArrayForGroups())
                    throw -1;

                uint64_t index = this->uf.find(i);
                uint64_t p = this->dependencyArrayForGroups[index];

                if (i == p)
                {
                    if (factorChars.find(i) == factorChars.end())
                    {
                        std::cout << "NOTFACTOR!" << std::endl;
                        throw -1;
                    }
                    else
                    {
                        return factorChars[p];
                    }
                }
                else
                {
                    return factorChars[p];
                }
            }

            // Update DA by f.
            uint64_t update(MSFactor &f)
            {
                if (f.isChar())
                {
                    // UnionFind::GINDEX p = this->uf.find(f.index);
                    this->uf.find(f.index);

                    if (this->isUsingDependencyArrayForGroups())
                    {
                        // this->dependIndexes[p] = FACTORCHAR;
                        this->factorChars[f.index] = f.getChar();
                    }
                }
                else
                {
                    for (uint64_t x = 0; x < f.length; x++)
                    {
                        UnionFind::GINDEX i = this->uf.find(f.index + x);
                        UnionFind::GINDEX j = this->uf.find(f.reference + x);

                        if (i == j)
                        {
                            std::cout << "same group" << std::endl;
                            throw -1;
                        }

                        UnionFind::GINDEX p = this->uf.unionOperation(i, j);
                        if (this->isUsingDependencyArrayForGroups())
                        {
                            this->dependencyArrayForGroups[p] = this->dependencyArrayForGroups[j];
                        }
                    }
                }
                return 0;
            }
            // Return the LCPWR(f.index, f.reference).
            uint64_t getLCPWR(MSFactor &f)
            {
                if (this->isUsingDependencyArrayForGroups())
                    return this->getLCPWR_Y(f);
                // uint64_t len = f.length;
                uint64_t x = 0;

                // uint64_t minimalIndex = 0;
                while (x < f.length)
                {
                    uint64_t i = f.index + x;
                    uint64_t j = f.reference + x;
                    uint64_t i2 = this->uf.findWithoutUpdate(i);
                    uint64_t j2 = this->uf.findWithoutUpdate(j);

                    if (i2 == j2)
                    {
                        break;
                    }
                    else
                    {
                        this->uf.unionOperationWithBackup(i2, j2);
                        x++;
                    }
                }
                this->uf.back();
                return x;
            }
            // Return the LCPWR(f.index, f.reference).
            uint64_t getLCPWR_Y(MSFactor &f)
            {
                uint64_t len = f.getLength();
                if (len > this->smallDA.size())
                {
                    uint64_t newLen = len * 2 <= this->size() ? len * 2 : this->size();
                    this->smallDA.resize(newLen);
                }

                for (uint64_t i = 0; i < len; i++)
                {

                    uint64_t iRefGlobalIndex = f.reference + i;
                    this->smallDA[i] = f.index <= iRefGlobalIndex && iRefGlobalIndex <= f.index + i ? this->smallDA[iRefGlobalIndex - f.index] : this->access(iRefGlobalIndex);
                    uint64_t iDepIndex = this->smallDA[i];

                    uint64_t refLocalIndex = i;
                    while (f.index <= iDepIndex && iDepIndex <= f.index + i)
                    {
                        if (iDepIndex == f.index + i)
                        {
                            return i;
                        }
                        else
                        {
                            this->smallStack.push(refLocalIndex);
                            uint64_t nextRefIndex = iDepIndex - f.index;
                            this->smallDA[refLocalIndex] = this->smallDA[nextRefIndex];
                            refLocalIndex = nextRefIndex;
                            iDepIndex = this->smallDA[nextRefIndex];
                        }
                    }
                    while (this->smallStack.size() > 0)
                    {
                        uint64_t top = this->smallStack.top();
                        this->smallDA[top] = iDepIndex;
                        this->smallStack.pop();
                    }
                }
                return len;
            }
            // Return the FakeLCPWR(f.index, f.reference).
            uint64_t getFakeLCPWR(MSFactor &f)
            {
                // uint64_t len = f.length;
                uint64_t x = 0;

                // uint64_t minimalIndex = 0;
                while (x < f.length)
                {
                    uint64_t i = f.index + x;
                    uint64_t j = f.reference + x;
                    uint64_t i2 = this->uf.find(i);
                    uint64_t j2 = this->uf.find(j);

                    if (i2 == j2)
                    {
                        break;
                    }
                    else
                    {
                        x++;
                    }
                }
                return x;
            }
        };
    }
}