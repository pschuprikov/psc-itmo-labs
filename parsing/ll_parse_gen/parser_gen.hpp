#ifndef PARSER_GEN_H
#define PARSER_GEN_H

#include <boost/mpl/front.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/container.hpp>
#include <exception>

#include "utils.hpp"
#include "first.hpp"
#include "follow.hpp"
#include "rule_action.hpp"

namespace parser_gen
{

using namespace boost::mpl;
using namespace boost;

template<class NT>
struct create_start_rule
{
    struct dummy
    {
        typedef non_terminal_tag tag;
        typedef NT syn_attr;
        typedef typename NT::inh_attr inh_attr;

        dummy(inh_attr, syn_attr syn)
            : result(syn)
        {}

        syn_attr result;
    };

    struct dummy_functor_finish
    {
        typedef action_tag tag;
        typename dummy::syn_attr operator()(typename dummy::inh_attr const& me, fusion::vector<NT> const& start) const
        {
            return fusion::at_c<0>(start);
        }
    };

    struct dummy_functor_start
    {
        typedef action_tag tag;
        typename NT::inh_attr operator()(typename dummy::inh_attr const& me, fusion::vector<> const& start) const
        {
            return me;
        }
    };

    typedef parser_gen::rule<dummy, vector<dummy_functor_start, NT, dummy_functor_finish> > type;
};

template<class TS>
struct get_lexer_var
{
    typedef typename make_variant_over<typename push_back<TS, parser_gen::t_eof>::type>::type type;
};

template<class Rules, class NTS, class TS, class Lexer>
struct create_parser
{
    typedef typename get_lexer_var<TS>::type lexer_variant;
    typedef typename make_variant_over<typename push_back<typename lexer_variant::types, parser_gen::t_epsilon>::type>::type tvar;
    typedef typename front<NTS>::type start_nt;
    typedef create_start_rule<start_nt> start_rule_creator;
    typedef typename start_rule_creator::type start_rule;
    typedef typename make_variant_over<typename push_back<NTS, typename start_rule_creator::dummy>::type>::type ntvar;
    typedef typename push_back<Rules, start_rule>::type rules;
    typedef typename parser_gen::first<rules, typename ntvar::types>::type first;
    typedef typename parser_gen::follow<rules, typename ntvar::types, first>::type follow;
    typedef parser_gen::common_types<first, follow, typename ntvar::types, typename tvar::types, rules> common;
    typedef typename parser_gen::all_rule_parser<common, start_rule, Lexer>::type parser_type;

    struct parser
    {
        start_nt operator()(typename start_nt::inh_attr const& inh, Lexer const& lexer)
        {
            return parser_type(lexer, fusion::vector<>(), inh).result().result;
        }
    };

    typedef parser type;
};


}

#endif // PARSER_GEN_H
