#ifndef RULE_ACTION_HPP
#define RULE_ACTION_HPP

#include <boost/variant.hpp>
#include <boost/mpl/remove.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/fusion/container.hpp>
#include <boost/fusion/include/push_back.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/ref.hpp>
#include "common.hpp"
#include "utils.hpp"
#include "first.hpp"
#include "follow.hpp"

namespace parser_gen
{

using boost::variant;
using boost::static_visitor;
using namespace boost::mpl;
using namespace boost;

template<class P, class Sibs, class RiBeg, class RiEnd, class Func, class Lexer>
struct parser_gen_state
{
    typedef P parent;
    typedef Sibs siblings;
    typedef RiBeg beg;
    typedef RiEnd end;
    typedef Func functor;
    typedef Lexer lexer;
};

template<class Common, class Rule, class Lexer> struct all_rule_parser;

template <class SBeg, class SEnd, class Res>
struct error_parser
        : public error_parser<typename next<SBeg>::type, SEnd, Res>
        , public virtual boost::static_visitor<Res>
{
    typedef error_parser<typename next<SBeg>::type, SEnd, Res> base;

    Res operator()(typename deref<SBeg>::type & term) const
    {
        throw 1;
    }

    using base::operator();
};

template <class SEnd, class Res>
struct error_parser<SEnd, SEnd, Res>
{
    Res operator()() const
    {

    }
};

template<class Common, class State>
struct non_terminal_parser
{
    typedef typename deref<typename State::beg>::type nt;
    typedef typename std::pair<typename nt::inh_attr, typename nt::syn_attr> result_type;

    template<class RI>
    struct get_transition_symbs
    {
        typedef typename deref<RI>::type::right right;
        typedef typename get_first<typename Common::first, typename begin<right>::type, typename end<right>::type>::type first;
        typedef typename if_<typename has_key<first, t_epsilon>::type,
                    typename at<typename Common::follow, nt>::type,
                    typename erase_key<first, t_epsilon>::type
                            >::type type;
    };

    template<class RBeg, class REnd, class Left> struct rule_parser;

    template<class SBeg, class SEnd, class RBeg, class REnd, class Left>
    struct symb_parser
            : public symb_parser<typename next<SBeg>::type,
                                 SEnd, RBeg, REnd,
                                 typename remove<Left,
                                 typename deref<SBeg>::type>::type>
    {
        typedef symb_parser<typename next<SBeg>::type,
                            SEnd, RBeg, REnd,
                            typename remove<Left,
                            typename deref<SBeg>::type>::type> base;

        typedef typename all_rule_parser<Common,
                                         typename deref<RBeg>::type,
                                         typename State::lexer>::type cur_rule_parser;

        symb_parser(typename State::lexer const& lex,
                    typename State::siblings const& sibs,
                    typename State::parent::inh_attr const& inh_parent)
            : base(lex, sibs, inh_parent)
        {}

        result_type operator()(typename deref<SBeg>::type & term) const
        {
            return cur_rule_parser(lex, fusion::vector<>(), 1).result();
        }

        using base::operator();
        using base::lex;
    };

    template<class SEnd, class RBeg, class REnd, class Left>
    struct symb_parser<SEnd, SEnd, RBeg, REnd, Left>
            : public rule_parser<typename next<RBeg>::type, REnd, Left>
    {
        typedef rule_parser<typename next<RBeg>::type, REnd, Left> base;

        symb_parser(typename State::lexer const& lex,
                    typename State::siblings const& sibs,
                    typename State::parent::inh_attr const& inh_parent)
            : base(lex, sibs, inh_parent)
        {}

        using base::operator();
        using base::lex;
    };

    template<class RBeg, class REnd, class Left>
    struct rule_parser
            : public symb_parser<typename begin<typename get_transition_symbs<RBeg>::type>::type,
                                 typename end<typename get_transition_symbs<RBeg>::type>::type,
                                 RBeg, REnd, Left>
            , public virtual boost::static_visitor<result_type>
    {
        typedef symb_parser<typename begin<typename get_transition_symbs<RBeg>::type>::type,
            typename end<typename get_transition_symbs<RBeg>::type>::type,
            RBeg, REnd, Left> base;

        rule_parser(typename State::lexer const& lex,
                    typename State::siblings const& sibs,
                    typename State::parent::inh_attr const& inh_parent)
            : base(lex, sibs, inh_parent)
        {}

        using base::operator();
        using base::lex;
    };

    template<class REnd, class Left>
    struct rule_parser<REnd, REnd, Left>
            : public error_parser<typename begin<Left>::type, typename end<Left>::type, result_type>
            , public virtual boost::static_visitor<result_type>
    {

        rule_parser(typename State::lexer const& lex,
                    typename State::siblings const& sibs,
                    typename State::parent::inh_attr const& inh_parent)
            : lex(lex), sibs(sibs), inh_parent(inh_parent)
        {}

        typename State::lexer const& lex;
        typename State::siblings sibs;
        typename State::parent::inh_attr const& inh_parent;
    };

    typedef typename at<typename Common::rules_map, nt>::type rules;

    typedef rule_parser<typename begin<rules>::type,
                        typename end<rules>::type,
                        typename Common::clear_ts> type;
};

template<class Common, class State>
struct terminal_parser
{
    typedef typename deref<typename State::beg>::type term;
    typedef typename std::pair<typename term::inh_attr, typename term::syn_attr> result_type;
    typedef typename remove<typename Common::clear_ts, term>::type left;

    struct parser
        : public error_parser<typename begin<left>::type, typename end<left>::type, result_type>
        , public virtual boost::static_visitor<result_type>
    {
        typedef error_parser<typename begin<left>::type, typename end<left>::type, result_type> base;

        using base::operator();

        parser(typename State::lexer const& lex,
               typename State::siblings const& sibs,
               typename State::parent::inh_attr const& inh_parent)
            : lex(lex), sibs(sibs), inh_parent(inh_parent)
        {}

        result_type operator()(term & t) const
        {
            lex.next_token();
            return result_type();
        }

        term term_;
        typename State::lexer const& lex;
        typename State::siblings sibs;
        typename State::parent::inh_attr const& inh_parent;
    };

    typedef parser type;
};

template<class Common, class State, class Tag>
struct choose_parser;

template<class Common, class State>
struct choose_parser<Common, State, non_terminal_tag>
{
    typedef typename non_terminal_parser<Common, State>::type type;
};

template<class Common, class State>
struct choose_parser<Common, State, terminal_tag>
{
    typedef typename terminal_parser<Common, State>::type type;
};

template<class Common, class State>
struct rule_finish
{
    rule_finish(typename State::lexer const& lex,
                typename State::siblings children,
                typename State::parent::inh_attr const& me)
        : lex(lex), children(children), me(me)
    {}

    typedef std::pair<typename State::parent::inh_attr, typename State::parent::syn_attr> result_type;

    result_type result()
    {
        return result_type();
    }

    typename State::lexer const& lex;
    typename State::siblings children;
    typename State::parent::inh_attr const& me;
};

template<class Common, class State> struct gen_rule_parser;

template<class Common, class State, class Sibs, class RiBeg, class RiEnd>
struct gen_rule_sentinel
{
    typedef parser_gen_state<typename State::parent,
                             Sibs,
                             typename next<RiBeg>::type,
                             RiEnd,
                             typename State::functor,
                             typename State::lexer
                            > new_state;
    typedef gen_rule_parser<Common, new_state> type;
};

template<class Common, class State, class Sibs, class RiEnd>
struct gen_rule_sentinel<Common, State, Sibs, RiEnd, RiEnd>
{
    typedef parser_gen_state<typename State::parent,
                             Sibs,
                             RiEnd,
                             RiEnd,
                             typename State::functor,
                             typename State::lexer
                            > new_state;
    typedef rule_finish<Common, new_state> type;
};

template<class Common, class State, class types>
struct choose_gen_rule_parser
{
    typedef gen_rule_parser<Common, State> type;
};

template<class Common, class State>
struct choose_gen_rule_parser<Common, State, vector<> >
{
    struct epsilon_parser
    {
        epsilon_parser(typename State::lexer const& lex,
                        typename State::siblings sibs,
                        typename State::parent::inh_attr const& inh_parent)
            : lex(lex), sibs(sibs), inh_parent(inh_parent)
        {}

        typedef std::pair<typename State::parent::inh_attr, typename State::parent::syn_attr> result_type;

        result_type result()
        {
            return std::make_pair(inh_parent, 0);
        }

        typename State::lexer const& lex;
        typename State::siblings sibs;
        typename State::parent::inh_attr const& inh_parent;
    };

    typedef epsilon_parser type;
};

template<class Common, class State>
struct gen_rule_parser
{
    typedef typename choose_parser<Common, State,
        typename deref<typename State::beg>::type::tag>::type node_parser;
    typedef typename fusion::result_of::as_vector<typename fusion::result_of::push_back<typename State::siblings const,
                                                  typename node_parser::result_type const&>::type>::type new_sibs;
    typedef typename gen_rule_sentinel<Common, State, new_sibs, typename State::beg,
        typename State::end>::type next_parser;

    gen_rule_parser(typename State::lexer const& lex,
                    typename State::siblings sibs,
                    typename State::parent::inh_attr const& inh_parent)
        : lex(lex), sibs(sibs), inh_parent(inh_parent)
    {}

    typedef std::pair<typename State::parent::inh_attr, typename State::parent::syn_attr> result_type;
    result_type result()
    {
        typename node_parser::result_type res = boost::apply_visitor(node_parser(lex, sibs, inh_parent), lex.cur_token());
        return next_parser(lex, fusion::as_vector(fusion::push_back(sibs, cref(res))), inh_parent).result();
    }

    typename State::lexer const& lex;
    typename State::siblings sibs;
    typename State::parent::inh_attr const& inh_parent;
};

template<class Common, class Rule, class Lexer>
struct all_rule_parser
{
    typedef parser_gen_state<typename Rule::left,
                             fusion::vector<>,
                             typename begin<typename Rule::right>::type,
                             typename begin<typename Rule::right>::type,
                             boost::none_t,
                             Lexer
                            > state;
    typedef typename choose_gen_rule_parser<Common, state, typename Rule::right>::type type;
};

}
#endif // RULE_ACTION_HPP
