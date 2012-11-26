#ifndef ALPHABET_H
#define ALPHABET_H

#include <string>
#include <stdexcept>

namespace coding
{

using namespace std;

struct alphabet_t
{
    typedef string::traits_type traits_t;
    typedef traits_t::int_type int_t;

    alphabet_t(size_t letter_size = 1)
        : letter_size_(letter_size)
    {}

    static size_t simple_symbols()
    {
        return sizeof(char) << 8;
    }

    size_t letter_size() const
    {
        return letter_size_;
    }

    int_t size() const
    {
        int_t res = 1;
        for (size_t i = 0; i < letter_size_; i++)
            res *= simple_symbols();
        return res;
    }

    string operator[](int_t idx) const
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

    int_t operator[](string letter) const
    {
        if (letter.length() != letter_size_)
            throw std::invalid_argument("wrong letter size");
        int_t res = 0;
        for (size_t i = 0; i < letter_size_; i++)
            res  = res * simple_symbols() + traits_t::to_int_type(letter[i]);
        return res;
    }


private:
    size_t letter_size_;
};

inline bool operator==(alphabet_t const& lhs, alphabet_t const& rhs)
{
    return lhs.letter_size() == rhs.letter_size();
}

inline bool operator!=(alphabet_t const& lhs, alphabet_t const& rhs)
{
    return !(lhs == rhs);
}

inline bool operator<(alphabet_t const& lhs, alphabet_t const& rhs)
{
    return lhs.letter_size() < rhs.letter_size();
}

}

#endif // ALPHABET_H
