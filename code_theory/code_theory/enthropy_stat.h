#ifndef ENTHROPY_STAT_H
#define ENTHROPY_STAT_H

#include <vector>
#include <cmath>

#include "alphabet.h"
#include "probability_counters.h"

namespace coding
{

double enthropy(probability::probability_provider const& prb)
{
    using namespace probability;
    double res = 0;
    BOOST_FOREACH(alphabet_t::int_t idx, prb.non_zero())
    {
        double pr = prb[idx];
        res += pr * (pr == 0 ? 0 : -log2(pr));
    }

    res += prb.tail().first * prb.tail().second * (-log2(prb.tail().second));

    return res;
}

double enthropy_by_letter(probability::probability_provider const& prb, alphabet_t const& alph)
{
    return enthropy(prb) / alph.letter_size();
}

}

#endif // ENTHROPY_STAT_H
