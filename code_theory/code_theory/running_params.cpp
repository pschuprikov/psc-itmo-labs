#include <iostream>
#include <fstream>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted.hpp>

#include <boost/spirit/include/phoenix.hpp>

#include "running_params.h"

using namespace boost;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

BOOST_FUSION_ADAPT_STRUCT(
        running_params::data_info,
        (bool, file)
        (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
        running_params::enthropy_info,
        (size_t, letter_size)
        (running_params::probability_type, pr_type)
        (bool, by_letter)
)

BOOST_FUSION_ADAPT_STRUCT(
        running_params::run_info,
        (running_params::data_info, data)
        (std::vector<running_params::enthropy_info>, feats)
)

namespace running_params
{
template<class Iterator>
struct runs_info_parser : qi::grammar<Iterator, runs_info(), ascii::space_type>
{

    runs_info_parser() : runs_info_parser::base_type(start_r, "qu")
    {
        using qi::lit;
        using qi::lexeme;
        using qi::int_;
        using qi::char_;
        using qi::on_error;
        using qi::fail;

        using namespace qi::labels;

        using phoenix::val;
        using phoenix::construct;

        quoted_string_r %= lexeme['"' >> +(char_ - '"') >> '"'];
        data_r %= lit("data") >> lit("{") >> source_type_s >> quoted_string_r >> lit("}");
        enthropy_r %= lit("enthropy") >> lit("{") >> int_ >> probability_type_s >> enthropy_type_s >> lit("}");
        run_info_r %= lit("run") >> lit("{") >> data_r >> *(enthropy_r) >> lit("}");
        start_r %= *run_info_r;

        on_error<fail>
        (
            start_r
          , std::cerr
                << val("Error! Expecting ")
                << _4                               // what failed?
                << val(" here: \"")
                << construct<std::string>(_3, _2)   // iterators to error-pos, end
                << val("\"")
                << std::endl
        );
    }

    struct enthropy_type_s_ : qi::symbols<char, bool>
    {
        enthropy_type_s_()
        {
            add
                    ("letter", true)
                    ("usual", false)
                    ;
        }
    } enthropy_type_s;

    struct probability_type_s_ : qi::symbols<char, probability_type>
    {
        probability_type_s_()
        {
            add
                    ("none", PT_NONE)
                    ("exp", PT_EXP)
            ;
        }
    } probability_type_s;

    struct source_type_s_ : qi::symbols<char, bool>
    {
        source_type_s_()
        {
            add
                    ("file", true)
                    ("raw", false)
            ;
        }
    } source_type_s;



    qi::rule<Iterator, data_info(), ascii::space_type> data_r;
    qi::rule<Iterator, enthropy_info(), ascii::space_type> enthropy_r;
    qi::rule<Iterator, run_info(), ascii::space_type> run_info_r;
    qi::rule<Iterator, std::string(), ascii::space_type> quoted_string_r;
    qi::rule<Iterator, runs_info(), ascii::space_type> start_r;
};

}


std::ostream& running_params::operator<<(std::ostream& out, enthropy_info const& ei)
{
    out << "entrhropy: letter_size = " << ei.letter_size;
    switch(ei.pr_type)
    {
    case PT_NONE: out << " tail = none "; break;
    case PT_EXP: out << " tail = exp "; break;
    }
    out << "by_letter = " << ei.by_letter;
    return out;
}

std::ostream& running_params::operator<<(std::ostream& out, data_info const& ei)
{
    out << "data: file = " << ei.file << " " << " name = " << ei.name;
    return out;
}

running_params::runs_info running_params::parse_config(std::string const& config)
{
    runs_info res;

    typedef std::string::const_iterator iter_type;

    runs_info_parser<iter_type> runs_p;

    iter_type beg = config.begin();
    iter_type end = config.end();

    qi::phrase_parse(beg, end, runs_p, ascii::space, res);

    return res;
}
