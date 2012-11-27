#ifndef ENTHROPY_STAT_H
#define ENTHROPY_STAT_H

#include <vector>
#include <cmath>

#include "alphabet.h"
#include "probability_counters.h"

namespace coding
{

template<class PrIter>
double enthropy(PrIter first, PrIter beyond)
{
    double res = 0;
    for (; first != beyond; ++first)
    {
        res += *first * (*first == 0 ? 0 : -log2(*first));
    }
    return res;
}

template<class PrIter>
double enthropy_by_letter(PrIter first, PrIter beyond, alphabet_t const& alph)
{
    return enthropy(first, beyond) / alph.letter_size();
}

}

#endif // ENTHROPY_STAT_H
