#ifndef PROBABILITY_COUNTERS_H
#define PROBABILITY_COUNTERS_H

#include <algorithm>
#include <numeric>
#include <vector>
#include <stdexcept>

#include "alphabet.h"

namespace coding
{

namespace probability
{

struct absence_preprocess_exp_t
{
    template<class Container>
    void operator() (Container& probs, alphabet_t const& alph) const
    {
        double total_prob = std::accumulate(probs.begin(), probs.end(), 0.);
        size_t zeroes = std::count_if(probs.begin(), probs.end(), std::bind1st(std::equal_to<double>(), 0));

        double zero_prob = 1. / alph.size();
        double total_zero_prob = zeroes * zero_prob;

        double norm_mult = zeroes != alph.size() ? (1 - total_zero_prob) / total_prob : 1.;
        for (typename Container::iterator p_it = probs.begin(); p_it != probs.end(); ++p_it)
        {
            *p_it == *p_it == 0 ? zero_prob : *p_it * norm_mult;
        }
    }
};

struct absence_preprocess_no_t
{
    template<class Container>
    void operator()(Container& probs, alphabet_t const& alph) const
    {}
};

typedef std::vector<double> distribution_t;


template<class SeqIter, class Absence>
void fill_probability(SeqIter first, SeqIter beyond, distribution_t & distr, alphabet_t const& alph, Absence const& absence_pp = absence_preprocess_no_t())
{
    if (first == beyond)
        throw invalid_argument("empty input");

    const int alphabet_size = alph.size();

    vector<size_t> hits(alphabet_size);
    size_t total = 0;
    for (; first != beyond; ++first)
    {
        total++;
        hits.at(*first)++;
    }

    distr.assign(hits.begin(), hits.end());
    std::transform(distr.begin(), distr.end(), distr.begin(), std::bind2nd(std::divides<double>(), total));
    absence_pp(distr, alph);
}

}

}

#endif // PROBABILITY_COUNTERS_H
