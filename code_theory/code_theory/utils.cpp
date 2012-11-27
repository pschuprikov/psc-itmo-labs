#include <boost/filesystem/fstream.hpp>

#include "utils.h"

std::string utils::read_file(boost::filesystem::path p, std::ios_base::openmode mode)
{
    using namespace std;
    using boost::filesystem::ifstream;
    ifstream ist(p, mode);
    ist.seekg(0, ios_base::end);
    int const len = ist.tellg();
    ist.seekg(0, ios_base::beg);
    if (len > 0)
    {
        vector<char> buffer(len);
        ist.read(&buffer[0], len);
        return string(buffer.begin(), buffer.end());
    }
    else
    {
        return string();
    }
}
