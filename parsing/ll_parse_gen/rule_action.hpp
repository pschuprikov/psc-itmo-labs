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
        throw parse_error("paser failed");
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

template <class State, class Functor>
struct functor_dispatcher
{
    typedef Functor type;
};

template <class State>
struct functor_dispatcher<State, boost::none_t>

{
    typedef struct
    {
        typedef typename deref<typename State::beg>::type nt;
        typename nt::inh_attr operator()(inh_child<typename State::parent, nt> par, typename State::siblings const& sibs) const
        {
            return typename nt::inh_attr();
        }
    } type;
};

template<class Common, class State>
struct non_terminal_parser
{
    typedef typename deref<typename State::beg>::type nt;
    typedef nt result_type;
    typedef typename functor_dispatcher<State, typename State::functor>::type functor_type;

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

        result_type operator()(typename deref<SBeg>::type &) const
        {
            return cur_rule_parser(lex, fusion::vector<>(), functor(
                                       inh_child<typename State::parent, nt>(inh_parent), sibs)).result();
        }

        using base::operator();
        using base::lex;
        using base::sibs;
        using base::inh_parent;
        using base::functor;
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
        using base::sibs;
        using base::inh_parent;
        using base::functor;
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
        using base::sibs;
        using base::inh_parent;
        using base::functor;
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
        typename State::siblings const& sibs;
        functor_type functor;
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
    typedef term result_type;
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
            : lex(lex)
        {}

        result_type operator()(term t) const
        {
            lex.next_token();
            return t;
        }

        typename State::lexer const& lex;
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

template<class State, class Functor>
struct finish_functor_dispatcher
{
    typedef Functor type;
};

template<class State>
struct finish_functor_dispatcher<State, boost::none_t>
{
    typedef struct default_functor
    {
        typename State::parent::syn_attr operator()(inh<typename State::parent> const& me,
                                                    typename State::siblings const& children) const
        {
            return typename State::parent::syn_attr();
        }
    } type;
};

template<class Common, class State>
struct rule_finish
{


    rule_finish(typename State::lexer const& lex,
                typename State::siblings const& children,
                typename State::parent::inh_attr const& me)
        : lex(lex), children(children), me(me)
    {}

    typedef typename State::parent result_type;

    result_type result()
    {
        return result_type(me, functor(inh<typename State::parent>(me), children));
    }

    typename finish_functor_dispatcher< State, typename State::functor >::type functor;
    typename State::lexer const& lex;
    typename State::siblings const& children;
    typename State::parent::inh_attr const& me;
};

template<class Common, class State> struct gen_rule_parser;

template<class Common, class State, class tag>
struct gen_rule_parser_dispatcher
{
    typedef gen_rule_parser<Common, State> type;
};

template<class Common, class State, class RiBeg, class RiEnd>
struct gen_rule_sentinel;

template<class Common, class State>
struct gen_rule_parser_dispatcher<Common, State, action_tag>
{
    typedef parser_gen_state<typename State::parent,
                             typename State::siblings,
                             typename next<typename State::beg>::type,
                             typename State::end,
                             typename deref<typename State::beg>::type,
                             typename State::lexer
                            > new_state;
    typedef typename gen_rule_sentinel<Common, new_state, typename new_state::beg,
        typename new_state::end>::type type;
};



template<class Common, class State, class RiBeg, class RiEnd>
struct gen_rule_sentinel
{
    typedef typename gen_rule_parser_dispatcher<Common, State, typename deref<RiBeg>::type::tag>::type type;
};

template<class Common, class State, class RiEnd>
struct gen_rule_sentinel<Common, State, RiEnd, RiEnd>
{
    typedef rule_finish<Common, State> type;
};

template<class Common, class State>
struct gen_rule_parser
{
    typedef typename choose_parser<Common, State,
        typename deref<typename State::beg>::type::tag>::type node_parser;
    typedef typename fusion::result_of::as_vector<typename fusion::result_of::push_back<typename State::siblings const,
                                                  typename node_parser::result_type>::type>::type new_sibs;
    typedef parser_gen_state<typename State::parent, new_sibs, typename next<typename State::beg>::type,
        typename State::end, boost::none_t, typename State::lexer> new_state;
    typedef typename gen_rule_sentinel<Common, new_state, typename new_state::beg,
        typename State::end>::type next_parser;

    gen_rule_parser(typename State::lexer const& lex,
                    typename State::siblings const& sibs,
                    typename State::parent::inh_attr const& inh_parent)
        : lex(lex), sibs(sibs), inh_parent(inh_parent)
    {}

    typedef typename State::parent result_type;
    result_type result()
    {
        typename node_parser::result_type res = boost::apply_visitor(node_parser(lex, sibs, inh_parent), lex.cur_token());
        return next_parser(lex, fusion::as_vector(fusion::push_back(sibs, res)), inh_parent).result();
    }

    typename State::lexer const& lex;
    typename State::siblings const& sibs;
    typename State::parent::inh_attr const& inh_parent;
};

template<class Common, class Rule, class Lexer>
struct all_rule_parser
{
    typedef parser_gen_state<typename Rule::left,
                             fusion::vector<>,
                             typename begin<typename Rule::right>::type,
                             typename end<typename Rule::right>::type,
                             boost::none_t,
                             Lexer
                            > state;
    typedef typename gen_rule_sentinel<Common, state, typename state::beg,
        typename state::end>::type type;
};

}
#endif // RULE_ACTION_HPP
