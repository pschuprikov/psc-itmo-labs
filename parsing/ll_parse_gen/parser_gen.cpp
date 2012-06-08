#include <iostream>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/container.hpp>
#include <string>

#include "parser_gen.hpp"
#include "rule_action.hpp"

using namespace boost;
using boost::variant;
using boost::mpl::vector;
using boost::mpl::push_back;
using boost::mpl::begin;
using boost::mpl::end;
using boost::mpl::set;
using boost::mpl::equal;
using boost::mpl::insert;

typedef utils::letter_nterm<'E'> E;
typedef utils::letter_nterm<'Q'> Q;
typedef utils::letter_nterm<'F'> F;
typedef utils::letter_nterm<'N'> N;
typedef utils::letter_nterm<'M'> M;
typedef utils::letter_nterm<'S'> S;
typedef utils::letter_nterm<'P'> P;
typedef utils::letter_term<'!'> nott;
typedef utils::letter_term<'^'> xorr;
typedef utils::letter_term<'&'> andd;
typedef utils::letter_term<'|'> orr;
typedef utils::letter_term<'n'> id;
typedef utils::letter_term<'('> op;
typedef utils::letter_term<')'> cp;

struct lexer
{
    lexer(std::string const& str)
        : str(str), cur(0)
    {

    }

    variant<xorr, nott, id, op, cp, andd, orr, parser_gen::t_eof>& cur_token() const
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
    mutable variant<xorr, nott, id, op, cp, andd, orr, parser_gen::t_eof> id_;
};

int main()
{

    typedef variant<E, Q, F, N, M, S, P> enterms;
    typedef variant<xorr, nott, id, op, cp, orr, andd, parser_gen::t_epsilon, parser_gen::t_eof> eterms;

    typedef parser_gen::rule<E, vector<F, Q> > r1;
    typedef parser_gen::rule<Q, vector<xorr, E> > r2;
    typedef parser_gen::rule<Q, vector<> > r3;
    typedef parser_gen::rule<F, vector<N, M> > r4;
    typedef parser_gen::rule<M, vector<orr, F> > r5;
    typedef parser_gen::rule<M, vector<> > r6;
    typedef parser_gen::rule<N, vector<S, P> > r7;
    typedef parser_gen::rule<P, vector<andd, N> > r8;
    typedef parser_gen::rule<P, vector<> > r9;
    typedef parser_gen::rule<S, vector<id> > r10;
    typedef parser_gen::rule<S, vector<op, E, cp> > r11;
    typedef parser_gen::rule<S, vector<nott, S> > r12;
    typedef vector<r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12> rules;
    typedef parser_gen::first<rules, enterms::types>::type efirst;
    typedef parser_gen::follow<rules, enterms::types, efirst>::type efollow;
    typedef utils::gen_map_vector<begin<enterms::types>::type, end<enterms::types>::type, efirst, eterms, enterms>
            efirst_vec_rep_t;
    efirst efirst_vec;
    typedef utils::gen_map_vector<begin<enterms::types>::type, end<enterms::types>::type, efollow, eterms, enterms>
           efollow_vec_rep_t;
    efollow efollow_vec;

    utils::print_map("first", efirst_vec_rep_t::result());
    utils::print_map("follow", efollow_vec_rep_t::result());
    typedef parser_gen::common_types<efirst, efollow, enterms::types, eterms::types, rules> common;

    typedef parser_gen::all_rule_parser<common, r1, lexer>::type parser;
    std::string str;
    while (true)
    {
        std::cin >> str;
        lexer lex(str);
        parser parser_inst(lex, fusion::vector<>(), 0);
        try {
            parser_inst.result();
        }
        catch(int a)
        {
            std::cerr << "NO\n";
        }
    }
}
