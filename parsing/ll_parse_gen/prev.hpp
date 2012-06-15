#ifndef PREV_HPP
#define PREV_HPP

#include "parser_gen.hpp"

#include <string>

namespace prev
{

typedef utils::letter_nterm<'E', int, int> E;
typedef utils::letter_nterm<'Q', int, int> Q;
typedef utils::letter_nterm<'F', int, int> F;
typedef utils::letter_nterm<'N', int, int> N;
typedef utils::letter_nterm<'M', int, int> M;
typedef utils::letter_nterm<'S', int, int> S;
typedef utils::letter_nterm<'P', int, int> P;
typedef utils::letter_term<'!', int> nott;
typedef utils::letter_term<'^', int> xorr;
typedef utils::letter_term<'&', int> andd;
typedef utils::letter_term<'|', int> orr;
typedef utils::letter_term<'n', int> id;
typedef utils::letter_term<'(', int> op;
typedef utils::letter_term<')', int> cp;

typedef boost::mpl::vector<E, Q, F, N, M, S, P> enterms;
typedef boost::mpl::vector<xorr, nott, id, op, cp, orr, andd> eterms;

struct lexer
{
    typedef parser_gen::get_lexer_var<eterms>::type var;

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
            case '|' :
                id_ = orr();
                break;
            case ')' :
                id_ = cp();
                break;
            case '(' :
                id_ = op();
                break;
            case 'n' :
                id_ = id();
                break;
            case '&' :
                id_ = andd();
                break;
            case '^' :
                id_ = xorr();
                break;
            case '!' :
                id_ = nott();
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

typedef parser_gen::rule<E, parser_gen::vector<F, Q> > r1;
typedef parser_gen::rule<Q, parser_gen::vector<xorr, E> > r2;
typedef parser_gen::rule<Q, parser_gen::vector<> > r3;
typedef parser_gen::rule<F, parser_gen::vector<N, M> > r4;
typedef parser_gen::rule<M, parser_gen::vector<orr, F> > r5;
typedef parser_gen::rule<M, parser_gen::vector<> > r6;
typedef parser_gen::rule<N, parser_gen::vector<S, P> > r7;
typedef parser_gen::rule<P, parser_gen::vector<andd, N> > r8;
typedef parser_gen::rule<P, parser_gen::vector<> > r9;
typedef parser_gen::rule<S, parser_gen::vector<id> > r10;
typedef parser_gen::rule<S, parser_gen::vector<op, E, cp> > r11;
typedef parser_gen::rule<S, parser_gen::vector<nott, S> > r12;

typedef parser_gen::vector<r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12> rules;

typedef parser_gen::create_parser<rules, enterms, eterms, lexer>::type parser;

}

#endif // PREV_HPP
