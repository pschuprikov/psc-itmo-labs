#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "running_params.h"

namespace processing
{

void process(std::string const& file_name, running_params::runs_info const& runs);

}

#endif // PROCESSOR_H
