#include "enthropy_stat.h"

namespace
{

inline double weighted_information(double p)
{
    return p * (p == 0 ? 0 : -log2(p));
}

}

double coding::enthropy(probability::probability_provider const& prb)
{
    using namespace probability;
    double res = 0;
    BOOST_FOREACH(alphabet_t::int_t idx, prb.non_zero())
    {
        res += weighted_information(prb[idx]);
    }

    res += prb.tail().first * weighted_information(prb.tail().second);

    return res;
}

double coding::enthropy_by_letter(probability::probability_provider const& prb, alphabet_t const& alph)
{
    return enthropy(prb) / alph.letter_size();
}
