#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "config_parser.h"

namespace processing
{

void process(std::ostream& file_name, running_params::runs_info const& runs);

}

#endif // PROCESSOR_H
