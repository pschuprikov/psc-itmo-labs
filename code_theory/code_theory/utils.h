#ifndef UTILS_H
#define UTILS_H

#include <ios>
#include <string>
#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>

namespace utils
{

std::string read_file(boost::filesystem::path p, std::ios_base::openmode mode = std::ios_base::in);

}

#endif // UTILS_H
