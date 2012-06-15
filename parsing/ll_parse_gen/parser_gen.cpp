#include <iostream>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/container.hpp>
#include <string>

#include "parenth.hpp"
#include "prev.hpp"
#include "calc.hpp"


int main()
{
    calc::parser parser;

    std::string str;
    while (true)
    {
        std::cin >> str;
        calc::lexer lex(str);
        try {
            calc::F res = parser(0, lex);
            std::cout << res.syn << std::endl;
        }
        catch(parser_gen::parse_error const& err)
        {
            std::cerr << "NO on " << lex.cur_token() << "\n";
        }
    }
}
