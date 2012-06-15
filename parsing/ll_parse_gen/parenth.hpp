#ifndef PARENTH_HPP
#define PARENTH_HPP

#include "parser_gen.hpp"

namespace parenth
{

typedef utils::letter_nterm<'F', int, int> F;
typedef utils::letter_term<'(', int> op;
typedef utils::letter_term<')', int> cp;

typedef parser_gen::vector<F> rnterms;
typedef parser_gen::vector<op, cp> rterms;

struct lexer
{
    typedef parser_gen::get_lexer_var<rterms>::type var;
    lexer(std::string const& str)
        : str(str), cur(0)
    {

    }

    var& cur_token() const
    {
        if (cur >= str.size())
            id_ = parser_gen::t_eof();
        else
        {
            switch (str[cur])
            {
            case ')' :
                id_ = cp();
                break;
            case '(' :
                id_ = op();
                break;
            }
        }
        return id_;
    }

    void next_token() const
    {
        cur++;
    }

    std::string const& str;
    mutable int cur;
    mutable var id_;
};

struct DepthCounter
{
    typedef parser_gen::action_tag tag;
    F::syn_attr operator()(F::inh_attr const&, parser_gen::fusion::vector<op, F, cp, F> const& children )
    {
        return std::max(parser_gen::fusion::at_c<1>(children).syn + 1, parser_gen::fusion::at_c<3>(children).syn);
    }
};

typedef parser_gen::rule<F, parser_gen::vector<op, F, cp, F, DepthCounter> > rr1;
typedef parser_gen::rule<F, parser_gen::vector<> > rr2;
typedef parser_gen::vector<rr1, rr2> rrules;

typedef parser_gen::create_parser<rrules, rnterms, rterms, lexer>::type parser;

}

#endif // PARENTH_HPP
