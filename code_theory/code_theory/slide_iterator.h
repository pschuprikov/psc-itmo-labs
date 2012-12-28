#ifndef DATA_PROVIDERS_H
#define DATA_PROVIDERS_H

#include <string>

#include <boost/iterator/iterator_facade.hpp>

#include "alphabet.h"

namespace coding
{
    using std::string;

    struct string_slide_iterator
            : boost::iterator_facade<string_slide_iterator, alphabet_t::int_t const, boost::forward_traversal_tag>
    {
        string_slide_iterator()
            : str_(), pos_(std::string::npos), cur_elem_(-1), alph_()
        {}

        string_slide_iterator(string const& str, alphabet_t const& alph)
            : str_ (str )
            , pos_(str.length() >= alph.letter_size() ? 0 : std::string::npos)
            , cur_elem_(-1), alph_(alph)
        {
            read_some();
        }

    private:
        friend class boost::iterator_core_access;

        void increment()
        {
            if (pos_ != string::npos)
            {
                pos_ = pos_ + alph_.letter_size() < str_.length() ? pos_ + 1 : string::npos;
            }

            read_some();
        }

        bool equal(string_slide_iterator const& rhs) const
        {
            if (rhs.pos_ != -1 && pos_ != -1 && rhs.alph_ != alph_)
                throw std::invalid_argument("incompatible string slide step");
            return pos_ == rhs.pos_;
        }

        string_slide_iterator::reference dereference() const { return cur_elem_; }

        void read_some()
        {
            if (pos_ == std::string::npos)
                return;
            cur_elem_ = alph_[str_.substr(pos_, alph_.letter_size())];
        }

    private:

        string str_;

        size_t pos_;
        string_slide_iterator::value_type cur_elem_;

        alphabet_t alph_;
    };
}

#endif // DATA_PROVIDERS_H
