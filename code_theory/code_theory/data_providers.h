#ifndef DATA_PROVIDERS_H
#define DATA_PROVIDERS_H

#include <stdexcept>
#include <string>

#include "alphabet.h"

namespace coding
{
    using std::string;

    struct string_slide_iterator
            : std::iterator<forward_iterator_tag, alphabet_t::int_t>
    {
        string_slide_iterator()
            : str_(), pos_(std::string::npos), cur_elem_(-1), alph_()
        {
        }

        string_slide_iterator(string const& str, alphabet_t const& alph)
            : str_ (str )
            , pos_(str.length() >= alph.letter_size() ? 0 : std::string::npos)
            , cur_elem_(-1), alph_(alph)
        {
            read_some();
        }

        string_slide_iterator::value_type const& operator*();
        string_slide_iterator::pointer operator->();

        string_slide_iterator& operator++();
        string_slide_iterator const operator++(int);

        bool operator==(string_slide_iterator const& rhs)
        {
            if (rhs.pos_ != -1 && pos_ != -1 && rhs.alph_ != alph_)
                throw std::invalid_argument("incompatible string slide step");
            return pos_ == rhs.pos_;
        }

        bool operator!=(string_slide_iterator const& rhs)
        {
            return !(*this == rhs);
        }

    private:
        void read_some();

    private:

        string str_;

        size_t pos_;
        string_slide_iterator::value_type cur_elem_;

        alphabet_t alph_;
    };

    string_slide_iterator string_slide_from_file(string const& file_name, alphabet_t const& alph);
}

#endif // DATA_PROVIDERS_H
