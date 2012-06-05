#include <iostream>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/variant.hpp>
#include <string>

#include "parser_gen.h"

using boost::variant;
using boost::mpl::vector;
using boost::mpl::push_back;
using boost::mpl::begin;
using boost::mpl::end;

template<int def>
struct default_int
{
    default_int()
        : val(def)
    {}

    operator int() const
    {
        return val;
    }

    int val;

    typedef parser_gen::terminal_tag tag;
};

struct A
{
    typedef parser_gen::non_terminal_tag tag;
};

struct B
{
    typedef parser_gen::non_terminal_tag tag;
};

int main()
{
    typedef variant<default_int<2>, default_int<3> > SVar;
    typedef SVar::types vec_res;

    typedef vector<vector<vector<default_int<2>, default_int<2> >,
                          vector<default_int<2>, default_int<3> >
                         >,
                   vector<vector<default_int<3>, default_int<2> >,
                          vector<default_int<3>, default_int<3> >
                         > > vec;

    typedef parser_gen::NonTerminalParserIndexer<typename begin<vec>::type,
                                                typename end<vec>::type,
                                                SVar,
                                                vec_res,
                                                vec_res> parser;
    parser p;
    SVar v1 = default_int<2>();
    SVar v2 = default_int<2>();
    std::vector<SVar> res = boost::apply_visitor(p, v1, v2);
    for (int i = 0; i < res.size(); i++)
    {
        std::cout << res[i] << std::endl;
    }

    typedef vector<A, B> nterms;
    typedef parser_gen::Rule<A, vector<A, default_int<1> > > rule1;
    parser_gen::First<vector<rule1>, nterms> pg;
}
