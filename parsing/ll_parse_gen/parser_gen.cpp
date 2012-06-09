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

struct ololo
{
    typedef parser_gen::action_tag tag;
};

struct FunctorBegin
{
    typedef parser_gen::action_tag tag;
    N::inh_attr operator()(F::inh_attr const& parent, fusion::vector<>) const
    {
        std::cerr << "functor begin says hi\n";
        return N::inh_attr();
    }
};

struct FunctorMiddle
{
    typedef parser_gen::action_tag tag;
    M::inh_attr operator()(F::inh_attr const& parent, fusion::vector<std::pair<N::inh_attr, N::syn_attr> >) const
    {
        std::cerr << "functor middle says hi\n";
        return M::inh_attr();
    }
};

struct FunctorEnd
{
    typedef parser_gen::action_tag tag;
    F::syn_attr operator()(F::inh_attr const& parent, fusion::vector<std::pair<N::inh_attr, N::syn_attr>,
                                                                     std::pair<M::inh_attr, M::syn_attr> >) const
    {
        std::cerr << "functor end says hi\n";
        return F::inh_attr();
    }
};

struct DepthCounter
{
    typedef parser_gen::action_tag tag;
    F::syn_attr operator()(F::inh_attr const&, fusion::vector<std::pair<op::inh_attr, op::syn_attr>,
                                                              std::pair<F::inh_attr, F::syn_attr>,
                                                              std::pair<cp::inh_attr, cp::syn_attr>,
                                                              std::pair<F::inh_attr, F::syn_attr> > children )
    {
        return std::max(fusion::at_c<1>(children).second + 1, fusion::at_c<3>(children).second);
    }
};

struct FinishCounter
{
    typedef parser_gen::action_tag tag;
    E::syn_attr operator()(E::inh_attr const&, fusion::vector<std::pair<F::inh_attr, F::syn_attr> > children )
    {
        return fusion::at_c<0>(children).second;
    }
};

int main()
{

    typedef variant<E, Q, F, N, M, S, P> enterms;
    typedef variant<xorr, nott, id, op, cp, orr, andd, parser_gen::t_epsilon, parser_gen::t_eof> eterms;

    typedef parser_gen::rule<E, vector<F, Q> > r1;
    typedef parser_gen::rule<Q, vector<xorr, E> > r2;
    typedef parser_gen::rule<Q, vector<> > r3;
    typedef parser_gen::rule<F, vector<FunctorBegin, N, FunctorMiddle, M, FunctorEnd> > r4;
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
    typedef parser_gen::common_types<efirst, efollow, enterms::types, eterms::types, rules> common;
    typedef parser_gen::all_rule_parser<common, r1, lexer>::type parser;

    typedef parser_gen::rule<E, vector<F, FinishCounter> > rr1;
    typedef parser_gen::rule<F, vector<op, F, cp, F, DepthCounter> > rr2;
    typedef parser_gen::rule<F, vector<> > rr3;
    typedef vector<rr1, rr2, rr3> rrules;

    typedef parser_gen::first<rrules, enterms::types>::type rfirst;
    typedef parser_gen::follow<rrules, enterms::types, rfirst>::type rfollow;
    typedef parser_gen::common_types<rfirst, rfollow, enterms::types, eterms::types, rrules> rcommon;

    typedef parser_gen::all_rule_parser<rcommon, rr1, lexer>::type rparser;


    std::string str;
    while (true)
    {
        std::cin >> str;
        lexer lex(str);
        rparser parser_inst(lex, fusion::vector<>(), 0);
        try {
            std::pair<E::inh_attr, E::syn_attr> res = parser_inst.result();
            std::cout << res.second << std::endl;
        }
        catch(int a)
        {
            std::cerr << "NO\n";
        }
    }
}
