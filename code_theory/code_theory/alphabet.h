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

    int_t size() const;

    string operator[](int_t idx) const;
    int_t operator[](string letter) const;


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
