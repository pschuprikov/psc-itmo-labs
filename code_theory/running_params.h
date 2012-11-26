#ifndef RUNNING_PARAMS_H
#define RUNNING_PARAMS_H

#include <string>
#include <vector>

namespace running_params
{
    struct data_info
    {
    public:

        data_info(string name, bool file)
            : name_(name)
            , file_(file)
        {}

        std::string name() const { return name_; }
        bool is_file() const { return file_; }

    private:
        std::string name_;
        bool file_;
    };

    enum probability_type { PT_NONE, PT_EXP };


    struct enthropy_info
    {
        enthropy_info(size_t letter_size = 1, bool by_letter = false, probability_type pr_type = PT_NONE)
            : letter_size_(letter_count)
            , by_letter_(by_letter)
            , pr_type_(pr_info)
        {}

        size_t letter_size() const { return letter_size_; }
        bool by_letter() const { return by_letter_; }
        probability_type pr_type() const { return pr_type_; }

    private:
        probability_type pr_type_;
        size_t letter_size_;
        bool by_letter_;
    };

    struct run_info
    {
        run_info(data_info const& data, std::vector<enthropy_info> const& feats);

        std::vector<enthropy_info> const& feats() const { return feats_; }
        data_info const& data() const { return data_; }

    private:
        std::vector<enthropy_info> feats_;
        data_info data_;
    };

    typedef vector<run_info> runs_info;

    runs_info parse_config(std::string file_name);
}

#endif // RUNNING_PARAMS_H
