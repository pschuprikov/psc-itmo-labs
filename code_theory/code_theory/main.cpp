#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "utils.h"
#include "config_parser.h"
#include "processor.h"

using namespace std;

int main(int argc, char const * argv[])
{
    string config;
    boost::filesystem::path working_dir = "./";

    if (argc > 1)
    {
        working_dir = argv[1];
        boost::filesystem::path config_path = working_dir / "process.cfg";
        config = utils::read_file(config_path);
    }
    else
    {
           config =      "run {                     "
                         "    data {                "
                         "        raw \"abcdefgh\" "
                         "    }                     "
                         "                          "
                         "    enthropy {            "
                         "        1 none usual      "
                         "    }                     "
                         "}                         ";
    }


    optional<running_params::runs_info> info = running_params::parse_config(config);

    if (!info)
        std::cerr << "Parse error\n exitting...\n";
    else
    {
        boost::filesystem::path save_path = working_dir / "result.txt";
        boost::filesystem::ofstream ost(save_path);
        processing::process(ost, *info);
    }

    return 0;
}

