#ifndef CALC_HPP
#define CALC_HPP

#include "parser_gen.hpp"

namespace calc
{

    typedef utils::letter_nterm<'F', int, int> F;
    typedef utils::letter_nterm<'N', int, int> N;
    typedef utils::letter_nterm<'M', int, int> M;
    typedef utils::letter_nterm<'S', int, int> S;
    typedef utils::letter_nterm<'P', int, int> P;
    typedef utils::letter_term<'+', int> plus;
    typedef utils::letter_term<'-', int> minus;
    typedef utils::letter_term<'*', int> times;
    typedef utils::letter_term<'/', int> div;
    typedef utils::letter_term<'n', int> id;
    typedef utils::letter_term<'(', int> op;
    typedef utils::letter_term<')', int> cp;

    typedef boost::mpl::vector<F, N, M, S, P> enterms;
    typedef boost::mpl::vector<plus, minus, id, op, cp, div, times> eterms;

struct lexer
{
    typedef parser_gen::get_lexer_var<eterms>::type var;
    lexer(std::string const& str)
        : str(str), cur(0)
    {
        next_token();
    }

    var& cur_token() const
    {
        return id_;
    }

    void next_token() const
    {
        while (cur < str.size() && isspace(str[cur]))
            cur++;
        if (cur >= str.size())
            id_ = parser_gen::t_eof();
        else
        {
            if (isdigit(str[cur]))
            {
                int acc = 0;
                while (isdigit(str[cur]))
                {
                    acc = acc * 10 + (str[cur] - '0');
                    cur++;
                }
                id_ = id(acc);
            }
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
                case '+' :
                    id_ = plus();
                    break;
                case '-' :
                    id_ = minus();
                    break;
                case '/' :
                    id_ = div();
                    break;
                case '*' :
                    id_ = times();
                    break;
                }
                cur++;
            }
        }
    }

    std::string const& str;
    mutable int cur;
    mutable var id_;
};

struct promote
{
    typedef parser_gen::action_tag tag;
    S::syn_attr operator()(S::inh_attr const&, parser_gen::fusion::vector<op, F, cp> const& child) const
    {
        return parser_gen::fusion::at_c<1>(child).syn;
    }
};

struct copy_id
{
    typedef parser_gen::action_tag tag;
    S::syn_attr operator()(S::inh_attr const&, parser_gen::fusion::vector<id> const& child) const
    {
        return parser_gen::fusion::at_c<0>(child).syn;
    }
};

struct div_f
{
    typedef parser_gen::action_tag tag;
    P::syn_attr operator()(P::inh_attr const& p, parser_gen::fusion::vector<div, S> const& child) const
    {
        return p / parser_gen::fusion::at_c<1>(child).syn;
    }
};

struct mult_f
{
    typedef parser_gen::action_tag tag;
    P::syn_attr operator()(P::inh_attr const& p, parser_gen::fusion::vector<times, S> const& child) const
    {
        return p * parser_gen::fusion::at_c<1>(child).syn;
    }
};

struct plus_f
{
    typedef parser_gen::action_tag tag;
    M::syn_attr operator()(M::inh_attr const& p, parser_gen::fusion::vector<plus, N> const& child) const
    {
        return p + parser_gen::fusion::at_c<1>(child).syn;
    }
};

struct minus_f
{
    typedef parser_gen::action_tag tag;
    M::syn_attr operator()(M::inh_attr const& p, parser_gen::fusion::vector<minus, N> const& child) const
    {
        return p - parser_gen::fusion::at_c<1>(child).syn;
    }
};

struct get_down
{
    typedef parser_gen::action_tag tag;

    template<class P, class C>
    typename P::result_type operator()(P p, C child) const
    {
        return parser_gen::fusion::at_c<0>(child).syn;
    }
};

struct ret_par
{
    typedef parser_gen::action_tag tag;

    template<class P, class C>
    typename P::result_type operator()(P p, C child) const
    {
        return p;
    }
};

struct ret_syn
{
    typedef parser_gen::action_tag tag;

    template<class P, class C>
    typename P::result_type operator()(P p, C child) const
    {

        return parser_gen::fusion::at_c<parser_gen::fusion::result_of::size<C>::value - 1>(child).syn;
    }
};

typedef parser_gen::rule<F, parser_gen::vector<N, get_down, M, ret_syn> > r1;
typedef parser_gen::rule<M, parser_gen::vector<plus, N, plus_f, M, ret_syn> > r2;
typedef parser_gen::rule<M, parser_gen::vector<minus, N, minus_f, M, ret_syn> > r3;
typedef parser_gen::rule<M, parser_gen::vector<ret_par> > r4;
typedef parser_gen::rule<N, parser_gen::vector<S, get_down, P, ret_syn> > r5;
typedef parser_gen::rule<P, parser_gen::vector<times, S, mult_f, P, ret_syn> > r6;
typedef parser_gen::rule<P, parser_gen::vector<div, S, div_f, P, ret_syn> > r7;
typedef parser_gen::rule<P, parser_gen::vector<ret_par> > r8;
typedef parser_gen::rule<S, parser_gen::vector<id, copy_id> > r9;
typedef parser_gen::rule<S, parser_gen::vector<op, F, cp, promote> > r10;

typedef parser_gen::vector<r1, r2, r3, r4, r5, r6, r7, r8, r9, r10> rules;

typedef parser_gen::create_parser<rules, enterms, eterms, lexer>::type parser;

}

#endif // CALC_HPP
