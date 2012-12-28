#ifndef PROBABILITY_COUNTERS_H
#define PROBABILITY_COUNTERS_H


#include <map>
#include <stdexcept>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include "alphabet.h"

namespace coding
{

namespace probability
{

typedef std::map<alphabet_t::int_t, double> distribution_t;

struct absence_preprocess_exp_t
{
    double operator()(distribution_t& distr, alphabet_t const& alph) const;
};

struct absence_preprocess_no_t
{
    double operator()(distribution_t& distr, alphabet_t const& alph) const
    {
        return 0;
    }
};

typedef vector<alphabet_t::int_t> letter_indicies_t;

struct probability_provider
{
    template<class SeqIter, class Absence>
    probability_provider(SeqIter first, SeqIter beyond, alphabet_t const& alph, Absence const& absence_pp = absence_preprocess_no_t())
    {
        if (first == beyond)
            throw invalid_argument("empty input");

        const alphabet_t::int_t alphabet_size = alph.size();

        size_t total = 0;
        for (; first != beyond; ++first)
        {
            total++;
            distr_[*first]++;
        }

        BOOST_FOREACH(distribution_t::value_type& val, distr_)
        {
            val.second /= total;
            non_zero_.push_back(val.first);
        }

        tail_prob_ = absence_pp(distr_, alph);
    }

    letter_indicies_t const& non_zero() const { return non_zero_; }
    double  operator[](alphabet_t::int_t idx) const { return distr_.find(idx) == distr_.end() ? tail_prob_ : distr_.at(idx); }
    pair<alphabet_t::int_t, double> tail() const { return std::make_pair(alph_.size() - distr_.size(), tail_prob_); }

private:
    alphabet_t            alph_;
    letter_indicies_t non_zero_;
    distribution_t       distr_;
    double           tail_prob_;
};

typedef boost::shared_ptr<probability_provider> probability_provider_ptr;

}

}
#endif // PROBABILITY_COUNTERS_H
