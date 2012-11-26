#include <iostream>
#include "running_params.h"
#include "processor.h"

using namespace std;

int main()
{
    string params_test = "run {                     "
                         "    data {                "
                         "        file \"/home/pasha/code_teory/CALGARY/PROGP\" "
                         "    }                     "
                         "                          "
                         "    enthropy {            "
                         "          2 none usual    "
                         "    }                     "
                         "}                         ";

    running_params::runs_info info = running_params::parse_config(params_test);
    processing::process("result.txt", info);

    return 0;
}

