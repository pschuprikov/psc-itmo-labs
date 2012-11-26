#include <fstream>
#include <vector>

#include "data_providers.h"

namespace coding
{

string_slide_iterator::value_type const& string_slide_iterator::operator*()
{
    return cur_elem_;
}

string_slide_iterator::pointer string_slide_iterator::operator->()
{
    return &cur_elem_;
}

string_slide_iterator& string_slide_iterator::operator++()
{
    if (pos_ == string::npos)
        return *this;

    pos_++;

    if (pos_ + alph_.letter_size() > str_.length())
        pos_ = string::npos;

    read_some();

    return *this;
}

void string_slide_iterator::read_some()
{
    if (pos_ == std::string::npos)
        return;
    cur_elem_ = alph_[str_.substr(pos_, alph_.letter_size())];
}

string_slide_iterator const string_slide_iterator::operator++(int)
{
    string_slide_iterator tmp = *this;
    operator++();
    return tmp;
}

}

coding::string_slide_iterator coding::string_slide_from_file(std::string const& file_name, coding::alphabet_t const& alph)
{
    using namespace std;
    ifstream fstr(file_name.c_str(), std::ios_base::binary);
    fstr.seekg(0, ios_base::end);
    size_t const len = fstr.tellg();

    if (!len)
        return string_slide_iterator();

    vector<char> buffer(len);
    fstr.seekg(0, ios_base::beg);
    fstr.read(&buffer[0], ios_base::beg);
    return string_slide_iterator(std::string(&buffer[0], len), alph);

}

