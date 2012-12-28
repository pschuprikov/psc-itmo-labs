#include <boost/foreach.hpp>

#include "probability_counters.h"

namespace coding
{

namespace probability
{

    double absence_preprocess_exp_t::operator()(distribution_t& distr, alphabet_t const& alph) const
    {
        double total_prob;

        BOOST_FOREACH(distribution_t::value_type const& val, distr)
        {
            total_prob += val.second;
        }

        alphabet_t::int_t zeroes = alph.size() - distr.size();

        double zero_prob = 1. / alph.size();

        double total_zero_prob = zeroes * zero_prob;

        double norm_mult = zeroes != alph.size() ? (1 - total_zero_prob) / total_prob : 1.;

        BOOST_FOREACH(distribution_t::value_type& val, distr)
        {
            val.second = val.second * norm_mult;
        }

        return zero_prob;
    }
}

}
