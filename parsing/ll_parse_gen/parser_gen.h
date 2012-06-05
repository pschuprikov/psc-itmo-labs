#ifndef PARSER_GEN_H
#define PARSER_GEN_H

#include <boost/tuple/tuple.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/type_traits.hpp>


#include <vector>

namespace parser_gen
{

using boost::is_same;
using boost::mpl::_1;
using boost::mpl::_2;
using boost::mpl::int_;
using boost::mpl::size;
using boost::mpl::empty;
using boost::mpl::deref;
using boost::mpl::insert;
using boost::mpl::at;
using boost::mpl::next;
using boost::mpl::and_;
using boost::mpl::not_;
using boost::mpl::bool_;
using boost::mpl::if_;
using boost::mpl::equal;
using boost::mpl::equal_to;
using boost::mpl::pop_front;
using boost::mpl::front;
using boost::mpl::has_key;
using boost::mpl::end;
using boost::mpl::set;
using boost::mpl::map;
using boost::mpl::begin;
using boost::mpl::advance;
using boost::mpl::fold;
using boost::mpl::pair;

struct terminal_tag {};
struct non_terminal_tag {};

template<class SA, class IA>
struct NonTerminal
{
public:
    typedef SA synthetic_attrs_t;
    typedef IA inherited_attrs_t;

public:
   synthetic_attrs_t synthetic;
   inherited_attrs_t inherited;
};

template<class SA>
struct Terminal
{
    typedef SA synthetic_attrs_t;

    synthetic_attrs_t synthetic;
};

template <class TBeg, class TEnd, class SVar>
struct VariantVectorDispatcher
{
    static std::vector<SVar>& result()
    {
        static std::vector<SVar> vec = VariantVectorDispatcher<typename next<TBeg>::type, TEnd, SVar>::result();
        static bool initialized = false;
        if (!initialized)
        {
            vec.push_back(typename deref<TBeg>::type());
            initialized = true;
        }
        return vec;
    }
};

template <class TSBeg, class SVar>
struct VariantVectorDispatcher<TSBeg, TSBeg, SVar>
{
    static std::vector<SVar>& result()
    {
        static std::vector<SVar> vec;
        return vec;
    }
};

template<class TSBeg, class TSEnd,class NT, class SVar, class TTypes>
struct TerminalParserIndexer
        : public TerminalParserIndexer<typename next<TSBeg>::type, TSEnd, NT, SVar, typename pop_front<TTypes>::type>
{
    typedef TerminalParserIndexer<typename next<TSBeg>::type, TSEnd, NT, SVar, typename pop_front<TTypes>::type>
        base_t_indexer_type;
    typedef typename front<TTypes>::type t_type;

    using base_t_indexer_type::operator();

    std::vector<SVar> const& operator()(t_type const& t, NT const& nt) const
    {
        typedef typename deref<TSBeg>::type types;
        return VariantVectorDispatcher<typename begin<types>::type, typename end<types>::type, SVar>::result();
    }
};

template<class TSBeg, class NT, class SVar, class TTypes>
struct TerminalParserIndexer <TSBeg, TSBeg, NT, SVar, TTypes>
{
    std::vector<SVar> const& operator()(NT const& a) const
    {
    }
};

template<class NTSBeg, class NTSEnd, class SVar, class NTTypes, class TTypes>
struct NonTerminalParserIndexer
        : public NonTerminalParserIndexer<typename next<NTSBeg>::type, NTSEnd, SVar, typename pop_front<NTTypes>::type, TTypes>
        , public TerminalParserIndexer<typename begin<typename deref<NTSBeg>::type>::type,
                                       typename end<typename deref<NTSBeg>::type>::type,
                                       typename front<NTTypes>::type, SVar, TTypes>
        , public virtual boost::static_visitor<std::vector<SVar> const&>
{
    typedef NonTerminalParserIndexer<typename next<NTSBeg>::type, NTSEnd, SVar, typename pop_front<NTTypes>::type, TTypes>
        base_nt_indexer_type;
    typedef typename deref<NTSBeg>::type nt_type;
    typedef TerminalParserIndexer<typename begin<typename deref<NTSBeg>::type>::type,
                                  typename end<typename deref<NTSBeg>::type>::type,
                                  typename front<NTTypes>::type, SVar, TTypes> t_indexer_type;

    using t_indexer_type::operator();
    using base_nt_indexer_type::operator();

};


template<class NTSBeg, class SVar, class NTTypes, class TTypes>
struct NonTerminalParserIndexer<NTSBeg, NTSBeg, SVar, NTTypes, TTypes>
{
    std::vector<SVar> const& operator()()
    {
    }
};

struct t_epsilon {};
struct t_eof {};


template<class Left, class Right>
struct Rule{
    typedef Left left;
    typedef Right right;
};

template <class Map, class NT, class T>
struct AddToNt
{
    typedef typename insert<Map, pair<NT, typename insert<typename at<Map, NT>::type, T>::type> >::type type;
};

template <class Map, class NT, class NT2>
struct AddAllToNt
{
    typedef typename insert<Map, typename pair<NT,
        typename fold<typename at<Map, NT2>::type,
                      Map,
                      typename insert<typename at<_1, NT>::type, _2>::type
                     >::type
                                               >::type
                           >::type type;
};

template<class Fst, class RBeg, class REnd>
struct RightRuleFirstIterate;

struct NextTermRightRuleFirstIterate
{

    template <class Fst, class RBeg, class REnd>
    struct apply
    {
        typedef typename deref<RBeg>::type cur;
        typedef typename deref<typename next<RBeg>::type>::type next_nt;

        typedef typename RightRuleFirstIterate<typename AddToNt<Fst, cur, next_nt>::type
                                  , typename next<RBeg>::type, REnd>::type type;
    };

};


struct NextNTermRightRuleFirstIterate
{

    template <class Fst, class RBeg, class REnd, class DidEps = bool_<false> >
    struct apply
    {
        typedef typename deref<RBeg>::type cur;
        typedef typename deref<typename next<RBeg>::type>::type next_nt;
        typedef typename at<Fst, next_nt>::type next_first;
        typedef typename advance<RBeg, int_<2> >::type next_next_iter;

        typedef typename if_<typename and_<typename has_key<next_first, t_epsilon>::type,
                                       typename not_<is_same<next_next_iter, REnd> >::type,
                                       typename not_<DidEps>::type
                                      >::type,
                         typename NextNTermRightRuleFirstIterate::apply<typename AddAllToNt<Fst, cur,
                             typename deref<next_next_iter>::type>::type, RBeg, REnd, bool_<true> >::type,
                         typename RightRuleFirstIterate<typename AddAllToNt<Fst, cur, next_nt>::type,
                             typename next<RBeg>::type, REnd
                                                        >::type
                        >::type type;
    };
};


template <class NextTag>
struct NextDispatch
{

};

template <>
struct NextDispatch<non_terminal_tag>
{
    typedef NextNTermRightRuleFirstIterate type;
};

template <>
struct NextDispatch<terminal_tag>
{
    typedef NextTermRightRuleFirstIterate type;
};

template<class Fst, class RBeg, class REnd>
struct RightRuleFirstIterate
{
    typedef typename deref<RBeg>::type cur;
    typedef typename NextDispatch<typename deref<typename next<RBeg>::type>::type::tag>::type
        ::template apply<Fst, RBeg, REnd>  next_processor;
    typedef typename if_<is_same<typename cur::tag, terminal_tag>,
                         typename RightRuleFirstIterate<Fst, typename next<RBeg>::type, REnd>::type,
                         typename next_processor::type
                        >::type type;
};

template <class Fst, class RBeg>
struct RightRuleFirstIterate<Fst, RBeg, typename next<RBeg>::type>
{
    typedef Fst type;
};


struct RuleFirstIterate
{
    template<class Fst, class Rule>
    struct apply
    {
    typedef typename if_<typename empty<typename Rule::right>::type,
                typename AddToNt <Fst, typename Rule::left, t_epsilon>::type,
                typename RightRuleFirstIterate<Fst, typename begin<typename Rule::right>::type,
                                typename end<typename Rule::right>::type>::type
               >::type type;
    };
};

template<class Fst, class  Rules, bool Ready = false>
struct FirstIterate;

template <class Fst, class Rules>
struct FirstIterate<Fst, Rules, true>
{
    typedef Fst type;
};

template <class Fst, class Rules>
struct FirstIterate<Fst, Rules, false>
{
    typedef typename fold<Rules, Fst, RuleFirstIterate>::type new_fst;
    typedef typename equal<Fst, new_fst>::type is_ready;
    typedef typename FirstIterate<new_fst, Rules, is_ready::value>::type type;
};


template<class Rules, class NTTypes>
struct First
{
    typedef typename fold<NTTypes, map<>, insert<_1, typename pair<_2, set<> >::type> >::type empty_first;
    typedef typename FirstIterate<empty_first, Rules>::type type;
};

}
#endif // PARSER_GEN_H
