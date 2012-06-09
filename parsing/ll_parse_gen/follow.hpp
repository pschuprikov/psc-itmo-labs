#ifndef FOLLOW_HPP
#define FOLLOW_HPP

#include "first.hpp"

namespace parser_gen
{
template<class Left, class Fst, class Follow, class Tag, class RBeg, class REnd>
struct left_rule_follow_iterate_dispatcher;

template<class Left, class Fst, class Follow, class RBeg, class REnd>
struct left_rule_follow_iterate_dispatcher<Left, Fst, Follow, terminal_tag, RBeg, REnd>
{
    typedef Follow type;
};

template<class Left, class Fst, class Follow, class RBeg, class REnd>
struct left_rule_follow_iterate_dispatcher<Left, Fst, Follow, non_terminal_tag, RBeg, REnd>
{
    typedef typename deref<RBeg>::type nt;
    typedef typename get_first<Fst, typename next<RBeg>::type, REnd>::type first;
    typedef typename if_<typename has_key<first, t_epsilon>::type,
                         typename utils::merge_sets<typename at<Follow, Left>::type,
                                            typename erase_key<first, t_epsilon>::type >::type,
                         first
                        >::type add_follow;
    typedef typename utils::merge_sets<typename at<Follow, nt>::type, add_follow>::type new_follow;
    typedef typename insert<Follow, typename pair<nt, new_follow>::type>::type type;
};

template<class Left, class Fst, class Follow, class RBeg, class REnd>
struct left_rule_follow_iterate_dispatcher<Left, Fst, Follow, action_tag, RBeg, REnd>
{
    typedef Follow type;
};

template <class Left, class Fst, class follow, class RBeg, class REnd>
struct left_rule_follow_iterate
{
    typedef typename left_rule_follow_iterate_dispatcher<Left, Fst, follow,
        typename deref<RBeg>::type::tag, RBeg, REnd>::type res;
    typedef typename left_rule_follow_iterate<Left, Fst, res, typename next<RBeg>::type, REnd>::type type;
};

template <class Left, class Fst, class follow, class REnd>
struct left_rule_follow_iterate<Left, Fst, follow, REnd, REnd>
{
    typedef follow type;
};

template <class Fst>
struct rule_follow_iterate
{
    template <class Follow, class Rule>
    struct apply
    {
        typedef typename left_rule_follow_iterate<typename Rule::left, Fst, Follow,
            typename begin<typename Rule::right>::type,
            typename end<typename Rule::right>::type>::type
                type;
    };
};

template<class Rules, class NTTypes, class Fst>
struct follow
{
    typedef typename fold<NTTypes, map<>, insert<_1, typename pair<_2, set<> >::type> >::type empty_follow;
    typedef typename insert<empty_follow, typename pair<typename front<NTTypes>::type, set<t_eof> >::type >::type start_follow;
    typedef typename utils::iterate_while_changes<start_follow, Rules, rule_follow_iterate<Fst>, false, 0, 5 >::type type;
};

}

#endif // FOLLOW_HPP
