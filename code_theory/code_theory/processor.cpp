#include <iostream>
#include <fstream>
#include <map>

#include "processor.h"
#include "enthropy_stat.h"
#include "data_providers.h"

namespace processing
{

using namespace ::coding;

namespace
{

using namespace std;
using namespace probability;

struct cached_string_data
{
    cached_string_data()
    {}

    cached_string_data(std::string const& str, alphabet_t const& alph)
        : alph_(alph)
        , gen_str_(string_slide_iterator(str, alph), string_slide_iterator())
    {}

    probability_provider const& operator[](running_params::probability_type pt) const
    {
        if (probs_.find(pt) == probs_.end())
        {
            switch (pt)
            {
            case running_params::PT_NONE : probs_[pt] = probability_provider_ptr(
                            new probability_provider(gen_str_.begin(), gen_str_.end(), alph_, absence_preprocess_no_t())
            );
                break;
            case running_params::PT_EXP  : probs_[pt] = probability_provider_ptr(
                            new probability_provider(gen_str_.begin(), gen_str_.end(), alph_, absence_preprocess_exp_t())
                );
                break;
            }
        }

        return *probs_[pt];
    }

private:
    typedef vector<alphabet_t::int_t> general_string_t;
    typedef map<running_params::probability_type, coding::probability::probability_provider_ptr> probs_map_t;

private:
    alphabet_t alph_;
    general_string_t gen_str_;
    mutable probs_map_t probs_;
};

struct cached_data
{
    cached_data(std::string const& str)
        : str_(str)
    {}

    cached_string_data const& operator[](alphabet_t const& alph) const
    {
        if (data_.find(alph) == data_.end())
        {
            data_[alph] = cached_string_data(str_, alph);
        }
        return data_[alph];
    }

private:
    typedef map<alphabet_t, cached_string_data> alphabet_map_t;

private:
    std::string str_;
    mutable alphabet_map_t data_;
};

struct enthropy_processor
{
    enthropy_processor(ostream& out, cached_data const& cd)
        : out_(out)
        , data_(cd)
    {}

    void operator()(running_params::enthropy_info const& ei) const
    {
        out_ << "processing:\n";
        out_ << ei << "\n";

        alphabet_t alph(ei.letter_size);
        double res;

        if (ei.by_letter)
            res = enthropy_by_letter(data_[alph][ei.pr_type], alph);
        else
            res = enthropy(data_[alph][ei.pr_type]);

        out_ << "result: " << res << "\n";
    }

private:
    ostream& out_;
    cached_data const& data_;
};

struct run_processor
{
    run_processor(ostream& out)
        : out_(out)
    {}

    void operator()(running_params::run_info const& run_info) const
    {
        out_ << "processing:\n";
        out_ << run_info.data << "\n";

        std::string str;
        if(run_info.data.file)
        {
            ifstream ist(run_info.data.name.c_str(), ios_base::binary);
            ist.seekg(0, ios_base::end);
            size_t const len = ist.tellg();

            vector<char> buffer(len);
            ist.seekg(0, ios_base::beg);
            ist.read(&buffer[0], len);
            str.assign(buffer.begin(), buffer.end());
        }
        else
        {
            str = run_info.data.name;
        }

        cached_data data(str);
        std::for_each(run_info.feats.begin(), run_info.feats.end(), enthropy_processor(out_, data));
    }

private:
    ostream& out_;
};

}

}

void processing::process(std::ostream& out, running_params::runs_info const& runs)
{
    std::for_each(runs.begin(), runs.end(), processing::run_processor(out));
}
