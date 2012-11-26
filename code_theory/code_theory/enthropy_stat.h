#ifndef ENTHROPY_STAT_H
#define ENTHROPY_STAT_H

#include <vector>
#include <cmath>
#include "stat_common.h"
#include "probability_counters.h"

namespace coding
{

template<class Iter>
double enthropy(Iter first, Iter beyond)
{
    double res = 0;
    for (; first != beyond; ++first)
    {
        res += *first * (*first == 0 ? 0 : -log2(*first));
    }
    return res;}

template<class Iter>
double enthropy_by_letter(Iter first, Iter beyond, alphabet_t const& alph)
{
    return enthropy(first, beyond) / alph.letter_size();
}

}

#endif // ENTHROPY_STAT_H
