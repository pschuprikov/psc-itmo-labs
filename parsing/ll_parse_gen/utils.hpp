#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <iostream>

#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/none_t.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/adapted/mpl.hpp>

#include "common.hpp"


namespace utils
{

using namespace boost;
using namespace boost::mpl;

template<class TBeg, class TEnd, class TVar>
struct gen_vector
{   
    typedef typename deref<TBeg>::type cur;
    typedef std::vector<TVar> result_type;
    typedef gen_vector<typename next<TBeg>::type, TEnd, TVar> next_vec;
    static result_type const& result()
    {
        static result_type vec;
        static bool initialized = false;
        if (!initialized)
        {
            vec.push_back(cur());
            std::copy(next_vec::result().begin(), next_vec::result().end(), std::back_inserter(vec));
            initialized = true;
        }
        return vec;
    }
};

template<class TEnd, class TVar>
struct gen_vector<TEnd, TEnd, TVar>
{
    typedef std::vector<TVar> result_type;
    static result_type const& result()
    {
        static result_type vec;
        return vec;
    }
};

template <class TBeg, class TEnd, class AS, class TVar, class NTVar>
struct gen_map_vector
{
    typedef std::vector<std::pair<NTVar, std::vector<TVar> > > result_type;
    typedef typename deref<TBeg>::type cur;
    typedef typename at<AS, cur>::type cur_set;
    typedef gen_map_vector<typename next<TBeg>::type, TEnd, AS, TVar, NTVar> next_vec;
    static result_type const& result()
    {
        static result_type vec;
        static bool initialized = false;
        if (!initialized)
        {
            vec.push_back(std::make_pair(cur(), gen_vector<typename begin<cur_set>::type,
                                                          typename end<cur_set>::type,
                                                           TVar>::result()));
            std::copy(next_vec::result().begin(), next_vec::result().end(), std::back_inserter(vec));
            initialized = true;
        }
        return vec;
    }
};

template <class TEnd, class AS, class TVar, class NTVar>
struct gen_map_vector<TEnd, TEnd, AS, TVar, NTVar>
{
    typedef std::vector<std::pair<NTVar, std::vector<TVar> > > result_type;
    static result_type const& result()
    {
        static result_type vec;
        return vec;
    }
};


template <class S1, class S2>
struct merge_sets
{
    typedef typename fold<S2, S1, typename lambda<insert<_1, _2> >::type>::type type;
};


template<class Map, class  Rules, class Iterator, bool Ready = false, int cur = 0, int max = 1000>
struct iterate_while_changes;

template <class Map, class Rules, class  Iterator, int cur, int max>
struct iterate_while_changes<Map, Rules, Iterator, true, cur, max>
{
    typedef Map type;
};

template <class Map, class Rules, class Iterator, int cur, int max>
struct iterate_while_changes<Map, Rules, Iterator, false, cur, max>
{
    typedef typename fold<Rules, Map, Iterator>::type new_fst;
    typedef typename equal<Map, new_fst>::type is_ready;
    typedef typename iterate_while_changes<new_fst, Rules, Iterator, is_ready::value, cur + 1, max>::type type;
};

template <class Map, class Rules, class Iterator, int max>
struct iterate_while_changes<Map, Rules, Iterator, false, max, max>
{
    typedef Map type;
};

template <char Letter, class Inh, class Syn>
struct letter_nterm
{
    typedef parser_gen::non_terminal_tag tag;
    typedef Inh inh_attr;
    typedef Syn syn_attr;
    letter_nterm(inh_attr inh, syn_attr syn)
        : syn(syn), inh(inh)
    {}

    inh_attr inh;
    syn_attr syn;
};

template <char Letter, class Inh, class Syn>
std::ostream& operator<<(std::ostream& out, letter_nterm<Letter, Inh, Syn> const& nt)
{
    return out << Letter;
}

template <char Letter, class Syn>
struct letter_term
{
    typedef parser_gen::terminal_tag tag;
    typedef Syn syn_attr;
    typedef boost::none_t inh_attr;

    letter_term(syn_attr syn = syn_attr())
        : syn(syn)
    {}

    syn_attr syn;
};

template <char Letter, class Syn>
std::ostream& operator<<(std::ostream& out, letter_term<Letter, Syn> const& nt)
{
    return out << Letter;
}

template<class Map>
void print_map(std::string const& name, Map const& map)
{
    std::cout << name << ":" << std::endl;
    for (int i = 0; i < map.size(); i++)
    {
        std::cout << map[i].first << ": ";
        for (int j = 0; j < map[i].second.size(); j++)
        {
            std::cout << map[i].second[j] << " ";
        }
        std::cout << std::endl;
    }
}

}

#endif // UTILS_HPP
