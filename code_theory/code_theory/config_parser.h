#ifndef RUNNING_PARAMS_H
#define RUNNING_PARAMS_H

#include <ostream>
#include <string>
#include <vector>

#include <boost/optional.hpp>

using boost::optional;

namespace running_params
{
    struct data_info
    {
        std::string name;
        bool file;
    };

    enum probability_type { PT_NONE, PT_EXP };


    struct enthropy_info
    {
        size_t letter_size;
        probability_type pr_type;
        bool by_letter;
    };

    std::ostream& operator<<(std::ostream& out, data_info const& ei);

    struct run_info
    {
        std::vector<enthropy_info> feats;
        data_info data;
    };

    std::ostream& operator<<(std::ostream& out, enthropy_info const& ei);

    typedef std::vector<run_info> runs_info;

    optional<runs_info> parse_config(std::string const& file_name);
}

#endif // RUNNING_PARAMS_H
