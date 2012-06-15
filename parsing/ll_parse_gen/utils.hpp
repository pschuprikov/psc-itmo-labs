#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <iostream>

#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/none_t.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/adapted/mpl.hpp>

#include "common.hpp"


namespace utils
{

using namespace boost;
using namespace boost::mpl;

template <class S1, class S2>
struct merge_sets
{
    typedef typename fold<S2, S1, typename lambda<insert<_1, _2> >::type>::type type;
};


template<class Map, class  Rules, class Iterator, bool Ready = false, int cur = 0, int max = 1000>
struct iterate_while_changes;

template <class Map, class Rules, class  Iterator, int cur, int max>
struct iterate_while_changes<Map, Rules, Iterator, true, cur, max>
{
    typedef Map type;
};

template <class Map, class Rules, class Iterator, int cur, int max>
struct iterate_while_changes<Map, Rules, Iterator, false, cur, max>
{
    typedef typename fold<Rules, Map, Iterator>::type new_fst;
    typedef typename equal<Map, new_fst>::type is_ready;
    typedef typename iterate_while_changes<new_fst, Rules, Iterator, is_ready::value, cur + 1, max>::type type;
};

template <class Map, class Rules, class Iterator, int max>
struct iterate_while_changes<Map, Rules, Iterator, false, max, max>
{
    typedef Map type;
};

template <char Letter, class Inh, class Syn>
struct letter_nterm
{
    typedef parser_gen::non_terminal_tag tag;
    typedef Inh inh_attr;
    typedef Syn syn_attr;
    letter_nterm(inh_attr inh, syn_attr syn)
        : syn(syn), inh(inh)
    {}

    inh_attr inh;
    syn_attr syn;
};

template <char Letter, class Inh, class Syn>
std::ostream& operator<<(std::ostream& out, letter_nterm<Letter, Inh, Syn> const& nt)
{
    return out << Letter;
}

template <char Letter, class Syn>
struct letter_term
{
    typedef parser_gen::terminal_tag tag;
    typedef Syn syn_attr;
    typedef boost::none_t inh_attr;

    letter_term(syn_attr syn = syn_attr())
        : syn(syn)
    {}

    syn_attr syn;
};

template <char Letter, class Syn>
std::ostream& operator<<(std::ostream& out, letter_term<Letter, Syn> const& nt)
{
    return out << Letter;
}

}

#endif // UTILS_HPP
