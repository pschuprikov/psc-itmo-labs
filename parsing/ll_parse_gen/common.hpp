#ifndef COMMON_HPP
#define COMMON_HPP

#include <boost/mpl/insert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/remove.hpp>
#include <boost/variant.hpp>

namespace parser_gen
{

using namespace boost::mpl;

struct terminal_tag;
struct non_terminal_tag;
struct action_tag;

struct t_epsilon
{
    typedef terminal_tag tag;
};

inline std::ostream& operator<<(std::ostream& out, t_epsilon const& eps)
{
    return out << "eps";
}

struct t_eof
{
    typedef terminal_tag t_epsilon;
};

inline std::ostream& operator<<(std::ostream& out, t_eof const& eof)
{
    return out << "eof";
}

template<class Left, class Right>
struct rule
{
    typedef Left left;
    typedef Right right;
};

struct gen_rule_map_inserter
{

    template<class Map, class Rule>
    struct apply
    {
    typedef typename insert<Map, typename pair<typename Rule::left, typename push_back<typename at<Map, typename Rule::left>::type,
        Rule>::type>::type>::type type;
    };
};

template<class NTS, class Rules>
struct gen_rule_map
{
    typedef typename fold<NTS, map<>, insert<_1, typename pair<_2, vector<> >::type> >::type empty_map;
    typedef typename fold<Rules, empty_map, gen_rule_map_inserter>::type type;
};

template<class First, class Follow, class NTS, class TS, class Rules>
struct common_types
{
    typedef First first;
    typedef Follow follow;
    typedef NTS nts;
    typedef TS ts;
    typedef typename remove<TS, t_epsilon>::type clear_ts;
    typedef typename boost::make_variant_over<clear_ts>::type tsvar;
    typedef typename gen_rule_map<NTS, Rules>::type rules_map;
};

}

#endif // COMMON_HPP
