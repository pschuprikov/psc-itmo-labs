#include "alphabet.h"

namespace coding
{

using namespace std;


alphabet_t::int_t alphabet_t::size() const
{
    int_t res = 1;
    for (size_t i = 0; i < letter_size_; i++)
        res *= simple_symbols();
    return res;
}

string alphabet_t::operator[](int_t idx) const
{
    if (idx >= size())
        throw std::out_of_range("letter idx out of bounds");
    string res;
    for (size_t i = 0; i < letter_size_; i++)
    {
        res += traits_t::to_char_type(idx);
        idx /= simple_symbols();
    }
    return res;
}

alphabet_t::int_t alphabet_t::operator[](string letter) const
{
    if (letter.length() != letter_size_)
        throw std::invalid_argument("wrong letter size");
    int_t res = 0;
    for (size_t i = 0; i < letter_size_; i++)
        res  = res * simple_symbols() + traits_t::to_int_type(letter[i]);
    return res;
}

}
