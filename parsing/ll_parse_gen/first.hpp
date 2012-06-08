#ifndef FIRST_HPP
#define FIRST_HPP

#include <boost/mpl/set.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/map.hpp>

#include "common.hpp"
#include "utils.hpp"

namespace parser_gen
{

using namespace boost::mpl;

template<class Fst, class RBeg, class REnd>
struct get_first;

template <class Fst, class Tag, class RBeg, class REnd>
struct get_first_dispatcher;

template <class Fst, class RBeg, class REnd>
struct get_first_dispatcher<Fst, terminal_tag, RBeg, REnd>
{
    typedef set<typename deref<RBeg>::type> type;
};

template <class Fst, class RBeg, class REnd>
struct get_first_dispatcher<Fst, non_terminal_tag, RBeg, REnd>
{
    typedef typename deref<RBeg>::type nt;
    typedef typename at<Fst, nt>::type nt_set;
    typedef typename if_<typename has_key<nt_set, t_epsilon>::type,
                         typename utils::merge_sets<typename get_first<Fst, typename next<RBeg>::type, REnd>::type,
                                            typename erase_key<nt_set, t_epsilon>::type
                                           >::type,
                         nt_set>::type type;
};

template <class Fst, class RBeg, class REnd>
struct get_first_dispatcher<Fst, action_tag, RBeg, REnd>
{
    typedef typename get_first<Fst, typename next<RBeg>::type, REnd>::type type;
};

template<class Fst, class RBeg, class REnd>
struct get_first
{
    typedef typename get_first_dispatcher<Fst, typename deref<RBeg>::type::tag, RBeg, REnd>::type type;
};

template <class Fst, class RBeg>
struct get_first<Fst, RBeg, RBeg>
{
    typedef set<t_epsilon> type;
};


struct rule_first_iterate
{
    template<class Fst, class Rule>
    struct apply
    {
        typedef typename get_first<Fst, typename begin<typename Rule::right>::type,
                                       typename end<typename Rule::right>::type>::type add_first;
        typedef typename at<Fst, typename Rule::left>::type term_set;
        typedef typename utils::merge_sets<term_set, add_first>::type new_first;
        typedef typename insert< Fst, typename pair<typename Rule::left, new_first>::type >::type type;
    };
};

template<class Rules, class NTTypes>
struct first
{
    typedef typename fold<NTTypes, map<>, insert<_1, typename pair<_2, set<> >::type> >::type empty_first;
    typedef typename utils::iterate_while_changes<empty_first, Rules, rule_first_iterate>::type type;
};

}

#endif // FIRST_HPP
