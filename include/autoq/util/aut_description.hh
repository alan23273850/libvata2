/*****************************************************************************
 *  AUTOQ Tree Automata Library
 *
 *  Copyright (c) 2011  Ondra Lengal <ilengal@fit.vutbr.cz>
 *
 *  Description:
 *    Header file for class with automaton description.
 *
 *****************************************************************************/

#ifndef _AUTOQ_AUT_DESCRIPTION_HH_
#define _AUTOQ_AUT_DESCRIPTION_HH_

// AUTOQ headers
#include <regex>
#include <chrono>
#include <algorithm>
#include <autoq/autoq.hh>
#include <autoq/util/triple.hh>
#include <autoq/util/two_way_dict.hh>
#include <boost/multiprecision/cpp_int.hpp>

namespace AUTOQ
{
	namespace Util
	{
        template <typename TT> struct Automata;
        struct Concrete;
		typedef Automata<Concrete> TreeAutomata;
        struct Symbolic;
		typedef Automata<Symbolic> SymbolicAutomata;
        struct Predicate;
        typedef Automata<Predicate> PredicateAutomata;
        struct Constraint;
        struct linear_combination;
	}
}

template <typename TT>
struct AUTOQ::Util::Automata
{
public:   // data types
    typedef TT InitialSymbol;
    typedef int64_t State; // TODO: will make the program slightly slower. We wish to make another dynamic type.
	typedef std::vector<State> StateVector;
	typedef std::set<State> StateSet;

	typedef std::vector<int> Tag;
    typedef std::pair<InitialSymbol, Tag> stdpairInitialSymbolTag;
    struct Symbol : stdpairInitialSymbolTag {
        using stdpairInitialSymbolTag::stdpairInitialSymbolTag; // inherit parent constructors
        template<typename... Args> Symbol(Args... args) : stdpairInitialSymbolTag({args...}, {}) {}
        // Reference: https://stackoverflow.com/a/32595916/11550178
        InitialSymbol& initial_symbol() & { return this->first; }
        const InitialSymbol& initial_symbol() const & { return this->first; }
        Tag& tag() & { return this->second; }
        const Tag& tag() const & { return this->second; }
        int& tag(int index) & { return this->second.at(index); }
        const int& tag(int index) const & { return this->second.at(index); }
        /*********************************************************/
        bool is_internal() const { return initial_symbol().is_internal(); }
        bool is_leaf() const { return initial_symbol().is_leaf(); }
        bool is_tagged() const { return !tag().empty(); }
        bool operator<(const Symbol &rhs) const {
            if (is_internal() && rhs.is_leaf()) return true;
            else if (is_leaf() && rhs.is_internal()) return false;
            else if (initial_symbol() == rhs.initial_symbol()) { // if symbol content is the same, compare tag
                // TODO: I still don't understand why "tag size" should also be compared first.
                if (tag().size() < rhs.tag().size())
                    return true;
                else if (tag().size() > rhs.tag().size())
                    return false;
                else
                    return tag() < rhs.tag();
            } // compare symbol content first
            else return initial_symbol() < rhs.initial_symbol();
        }
        friend std::ostream& operator<<(std::ostream& os, const Symbol& obj) {
            os << obj.initial_symbol(); // print only the initial symbol
            return os;
        }
    };
    typedef std::map<Symbol, std::map<StateVector, StateSet>> TransitionMap;

public:   // data members
	std::string name;
    StateVector finalStates;
    State stateNum;
    int qubitNum;
	TransitionMap transitions;
    bool isTopdownDeterministic;
    inline static int gateCount;
    inline static bool gateLog, opLog;
    inline static std::chrono::steady_clock::duration binop_time, branch_rest_time, value_rest_time;
    inline static std::chrono::time_point<std::chrono::steady_clock> start_time;
    /* Notice inline is very convenient for declaring and defining a static member variable together! */

public:   // methods

	Automata() :
		name(),
		finalStates(),
        stateNum(),
        qubitNum(),
		transitions(),
        isTopdownDeterministic(false)
	{ }

	/**
	 * @brief  Relaxed equivalence check
	 *
	 * Checks whether the final states and transitions of two automata descriptions match.
	 */
	bool operator==(const Automata& rhs) const
	{
		return (finalStates == rhs.finalStates) && (transitions == rhs.transitions);
	}

	/**
	 * @brief  Strict equivalence check
	 *
	 * Checks whether all components of two automata descriptions match.
	 */
	bool StrictlyEqual(const Automata& rhs) const
	{
		return
			(name == rhs.name) &&
			(finalStates == rhs.finalStates) &&
            (stateNum == rhs.stateNum) &&
            (qubitNum == rhs.qubitNum) &&
			(transitions == rhs.transitions);
	}

	std::string ToString() const
	{
		std::string result;
		result += "name: " + name + "\n";
		result += "number of states: " + Convert::ToString(stateNum) + "\n";
		result += "final states: " + Convert::ToString(finalStates) + "\n";
		result += "transitions: \n";
		for (const auto &trans : transitions)
		{
			result += Convert::ToString(trans) + "\n";
		}

		return result;
	}

private:
    void state_renumbering();
    void remove_useless(bool only_bottom_up=false);
    Automata binary_operation(const Automata &o, bool add);
    void swap_forward(const int k);
    void swap_backward(const int k);

public:
    void fraction_simplification();
    void omega_multiplication(int rotation=1);
    void divide_by_the_square_root_of_two();
    void branch_restriction(int k, bool positive_has_value=true);
    void value_restriction(int k, bool branch);
    void semi_determinize();
    void semi_undeterminize();
    Automata operator+(const Automata &o);
    Automata operator-(const Automata &o);
    Automata Union(const Automata &o); // U is in uppercase since "union" is a reserved keyword.
    void print_aut(const char *str="") const;
    void print_stats(const std::string &str="", bool newline=false);
    int transition_size();

    /// simulation-based reduction
    void sim_reduce();
    /// lightweight size reduction, done upwards; returns @p true iff the automaton changed
    bool light_reduce_up();
    /// lightweight upwareds size reduction, iterated until change happens, returns @p true iff the automaton changed
    bool light_reduce_up_iter();
    /// lightweight size reduction, done downwards; returns @p true iff the automaton changed
    bool light_reduce_down();
    /// lightweight downwards size reduction, iterated until change happens, returns @p true iff the automaton changed
    bool light_reduce_down_iter();
    /// reduces the automaton using a prefered reduction
    void reduce();

    int count_transitions();
    void X(int t);
    void Y(int t);
    void Z(int t);
    void H(int t);
    void S(int t);
    void T(int t);
    void Rx(int t);
    void Ry(int t);
    void CNOT(int c, int t, bool opt=true);
    void CZ(int c, int t);
    void Toffoli(int c, int c2, int t);
    // void Fredkin(int c, int t, int t2);
    void randG(int G, int A, int B=0, int C=0);
    void Tdg(int t);
    void Sdg(int t);
    void swap(int t1, int t2);

    /* Produce an automaton instance. */
    static Automata uniform(int n);
    static Automata basis(int n);
    static Automata random(int n);
    static Automata zero(int n);
    static Automata basis_zero_one_zero(int n);
    static Automata zero_zero_one_zero(int n);
    static Automata zero_one_zero(int n);

    /* Equivalence Checking */
    static bool check_equal(const Automata& lhsPath, const Automata& rhsPath);
    static bool check_equal_aut(Automata lhs, Automata rhs);
    static bool check_inclusion(const std::string& lhsPath, const std::string& rhsPath);
    static bool check_inclusion(const Automata& lhsPath, const std::string& rhsPath);
    static bool check_inclusion(const std::string& lhsPath, const Automata& rhsPath);
    static bool check_inclusion(const Automata& lhsPath, const Automata& rhsPath);

    void execute(const char *filename);
    void print_language();
    std::vector<std::vector<std::string>> print(const std::map<typename AUTOQ::Util::Automata<InitialSymbol>::State, typename AUTOQ::Util::Automata<InitialSymbol>::InitialSymbol> &leafSymbolMap, int qubit, typename AUTOQ::Util::Automata<InitialSymbol>::State state);
};

// Concrete initial symbol
typedef std::vector<boost::multiprecision::cpp_int> stdvectorboostmultiprecisioncpp_int;
struct AUTOQ::Util::Concrete : stdvectorboostmultiprecisioncpp_int {
    using stdvectorboostmultiprecisioncpp_int::stdvectorboostmultiprecisioncpp_int;
    typedef typename AUTOQ::Util::Concrete::value_type Entry;
    // Notice that if we do not use is_convertible_v, type int will not be accepted in this case.
    template <typename T, typename = std::enable_if_t<std::is_convertible<T, Entry>::value>>
        Concrete(T qubit) : stdvectorboostmultiprecisioncpp_int({qubit}) {} 
    Entry qubit() const { return is_leaf() ? 0 : at(0); }
    bool is_leaf() const { return size() == 5; }
    bool is_internal() const { return size() < 5; }
    void back_to_zero() { at(0) = at(1) = at(2) = at(3) = 0; }
    friend std::ostream& operator<<(std::ostream& os, const Concrete& obj) {
        os << AUTOQ::Util::Convert::ToString(static_cast<stdvectorboostmultiprecisioncpp_int>(obj));
        return os;
    }
    Concrete operator+(const Concrete &o) const { return binary_operation(o, true); }
    Concrete operator-(const Concrete &o) const { return binary_operation(o, false); }
    Concrete binary_operation(const Concrete &o, bool add) const {
        assert(this->at(4) == o.at(4)); // Two k's must be the same.
        Concrete symbol;
        for (int i=0; i<4; i++) { // We do not change k here.
            if (add) symbol.push_back(this->at(i) + o.at(i));
            else symbol.push_back(this->at(i) - o.at(i));
            // if ((a>=0 && b>=0 && a>std::numeric_limits<Entry>::max()-b)
            //  || (a<0 && b<0 && a<std::numeric_limits<Entry>::min()-b))
            //     throw std::overflow_error("");
        }
        symbol.push_back(this->at(4)); // remember to push k
        return symbol;
    }
    Concrete operator*(const Concrete &o) const {
        Concrete symbol;
        symbol.push_back(at(0)*o.at(0) - at(1)*o.at(3) - at(2)*o.at(2) - at(3)*o.at(1));
        symbol.push_back(at(0)*o.at(1) + at(1)*o.at(0) - at(2)*o.at(3) - at(3)*o.at(2));
        symbol.push_back(at(0)*o.at(2) + at(1)*o.at(1) + at(2)*o.at(0) - at(3)*o.at(3));
        symbol.push_back(at(0)*o.at(3) + at(1)*o.at(2) + at(2)*o.at(1) + at(3)*o.at(0));
        symbol.push_back(at(4) + o.at(4)); // remember to push k
        return symbol;
    }
    void fraction_simplification() {
        if (at(0)==0 && at(1)==0 && at(2)==0 && at(3)==0) at(4) = 0;
        else {
            while ((at(0)&1)==0 && (at(1)&1)==0 && (at(2)&1)==0 && (at(3)&1)==0 && at(4)>=2) { // Notice the parentheses enclosing at(i)&1 are very important! HAHA
                for (int i=0; i<4; i++) at(i) /= 2;
                at(4) -= 2;
            }
        }
    }
    void omega_multiplication(int rotation=1) {
        int r = rotation;
        while (r != 0) {
            if (r > 0) {
                auto temp = at(3);
                for (int i=3; i>=1; i--) { // exclude "k"
                    at(i) = at(i-1);
                }
                at(0) = -temp;
                r--;
            } else {
                auto temp = at(0);
                for (int i=0; i<=2; i++) { // exclude "k"
                    at(i) = at(i+1);
                }
                at(3) = -temp;
                r++;
            }
        }
    }
    void divide_by_the_square_root_of_two() {
        at(4)++;
    }
    void Y() {
        for (int i=0; i<4; i++)
            at(i) = -at(i);
    }
    void Tdg() {
        auto t = at(0);
        for (int i=0; i<3; i++) {
            at(i) = at(i+1);
        }
        at(3) = -t;
    }
    void Sdg() {
        auto a=at(2), b=at(3), c=at(0), d=at(1);
        at(0)=a; at(1)=b; at(2)=-c; at(3)=-d;
    }
};

// Symbolic initial symbol
typedef std::map<std::string, boost::multiprecision::cpp_int> stdmapstdstringboostmultiprecisioncpp_int;
struct AUTOQ::Util::linear_combination : std::map<std::string, boost::multiprecision::cpp_int> {
    using stdmapstdstringboostmultiprecisioncpp_int::stdmapstdstringboostmultiprecisioncpp_int;
    linear_combination operator+(linear_combination b) const {
        for (const auto &kv : *this) {
            auto k = kv.first;
            auto v = kv.second;
            b[k] += v;
        }
        return b;
    }
    linear_combination operator-(const linear_combination &b) const {
        auto a = *this; // copy!
        for (const auto &kv : b) {
            auto k = kv.first;
            auto v = kv.second;
            a[k] -= v;
        }
        return a;
    }
    linear_combination operator*(const linear_combination &b) const {
        linear_combination ans;
        for (const auto &kv1 : *this) {
            for (const auto &kv2 : b) {
                if (kv1.first == "1") {
                    ans[kv2.first] += kv1.second * kv2.second;
                } else if (kv2.first == "1") {
                    ans[kv1.first] += kv1.second * kv2.second;
                } else if (kv1.first < kv2.first) {
                    ans[kv1.first + "*" + kv2.first] += kv1.second * kv2.second;
                } else {
                    ans[kv2.first + "*" + kv1.first] += kv1.second * kv2.second;
                }
            }
        }
        return ans;
    }
    friend std::ostream& operator<<(std::ostream& os, const linear_combination& obj) {
        // os << AUTOQ::Util::Convert::ToString(static_cast<stdmapstdstringboostmultiprecisioncpp_int>(obj));
        if (obj.empty()) {
            os << "0";
            return os;
        }
        for (auto kv = obj.begin(); kv != obj.end(); ++kv) {
            if (kv->first == "1")
                os << kv->second;
            else {
                if (kv->second != 1)
                    os << kv->second;
                os << kv->first;
            }
            if (std::next(kv) != obj.end())
                os << ' ';
        }
        return os;
    }
};
typedef std::vector<AUTOQ::Util::linear_combination> stdvectorstdmapstdstringboostmultiprecisioncpp_int;
struct AUTOQ::Util::Symbolic : stdvectorstdmapstdstringboostmultiprecisioncpp_int {
    using stdvectorstdmapstdstringboostmultiprecisioncpp_int::stdvectorstdmapstdstringboostmultiprecisioncpp_int;
    typedef typename AUTOQ::Util::Symbolic::value_type Map;
    typedef typename AUTOQ::Util::Symbolic::value_type::value_type Pair;
    typedef typename AUTOQ::Util::Symbolic::value_type::mapped_type Entry;
    template <typename T, typename = std::enable_if_t<std::is_convertible<T, Entry>::value>>
        Symbolic(T qubit) : stdvectorstdmapstdstringboostmultiprecisioncpp_int({Map{{"1", qubit}}}) {}
    Entry qubit() const { return is_leaf() ? 0 : at(0).at("1"); }
    bool is_leaf() const { return size() == 5; }
    bool is_internal() const { return size() < 5; }
    void back_to_zero() {
        std::fill(begin(), begin()+4, Map());
    }
    friend std::ostream& operator<<(std::ostream& os, const Symbolic& obj) {
        os << AUTOQ::Util::Convert::ToString(static_cast<stdvectorstdmapstdstringboostmultiprecisioncpp_int>(obj));
        return os;
    }
    Symbolic operator+(const Symbolic &o) const { return binary_operation(o, true); }
    Symbolic operator-(const Symbolic &o) const { return binary_operation(o, false); }
    Symbolic binary_operation(Symbolic o, bool add) const {
        assert(this->at(4) == o.at(4)); // Two k's must be the same.
        Symbolic symbol;
        for (int i=0; i<4; i++) { // We do not change k here.
            if (add)
                symbol.push_back(at(i) + o.at(i));
            else
                symbol.push_back(at(i) - o.at(i));
        }
        symbol.push_back(this->at(4)); // remember to push k
        return symbol;
    }
    Symbolic operator*(const Symbolic &o) const {
        Symbolic symbol;
        symbol.push_back(at(0)*o.at(0) - at(1)*o.at(3) - at(2)*o.at(2) - at(3)*o.at(1));
        symbol.push_back(at(0)*o.at(1) + at(1)*o.at(0) - at(2)*o.at(3) - at(3)*o.at(2));
        symbol.push_back(at(0)*o.at(2) + at(1)*o.at(1) + at(2)*o.at(0) - at(3)*o.at(3));
        symbol.push_back(at(0)*o.at(3) + at(1)*o.at(2) + at(2)*o.at(1) + at(3)*o.at(0));
        symbol.push_back(at(4) + o.at(4)); // remember to push k
        return symbol;
    }
    void fraction_simplification() {
        while (std::all_of(begin(), begin()+4, [](const Map &m) {
            return std::all_of(m.begin(), m.end(), [](const auto &p) { return (p.second&1)==0; });
        }) && at(4).find("1")!=at(4).end() && at(4).at("1") >= 2) { // Notice the parentheses enclosing i&1 are very important!
            for (int i=0; i<4; i++) {
                std::for_each(at(i).begin(), at(i).end(), [](auto &p) { p.second /= 2; });
                for (auto it = at(i).begin(); it != at(i).end(); )
                    if (it->second == 0)
                        it = at(i).erase(it);
                    else
                        ++it;
            }
            at(4).at("1") -= 2;
        }
        if (std::all_of(begin(), begin()+4, [](const Map &m) { return m.empty(); }))
            at(4).clear();
    }
    void omega_multiplication(int rotation=1) {
        int r = rotation;
        while (r != 0) {
            if (r > 0) {
                auto temp = at(3);
                for (int i=3; i>=1; i--) { // exclude "k"
                    at(i) = at(i-1);
                }
                std::for_each(temp.begin(), temp.end(), [](auto &p) { p.second = -p.second; });
                at(0) = temp;
                r--;
            } else {
                auto temp = at(0);
                for (int i=0; i<=2; i++) { // exclude "k"
                    at(i) = at(i+1);
                }
                std::for_each(temp.begin(), temp.end(), [](auto &p) { p.second = -p.second; });
                at(3) = temp;
                r++;
            }
        }
    }
    void divide_by_the_square_root_of_two() {
        at(4)["1"]++; // use [] instead of () in case the original map is empty.
    }
    void Y() {
        for (int i=0; i<4; i++)
            std::for_each(at(i).begin(), at(i).end(), [](auto &p) { p.second = -p.second; });
    }
    void Tdg() {
        auto t = at(0);
        for (int i=0; i<3; i++) {
            at(i) = at(i+1);
        }
        std::for_each(t.begin(), t.end(), [](auto &p) { p.second = -p.second; });
        at(3) = t;
    }
    void Sdg() {
        auto a=at(2), b=at(3), c=at(0), d=at(1);
        at(0)=a; at(1)=b;
        std::for_each(c.begin(), c.end(), [](auto &p) { p.second = -p.second; });
        at(2)=c;
        std::for_each(d.begin(), d.end(), [](auto &p) { p.second = -p.second; });
        at(3)=d;
    }
};

struct AUTOQ::Util::Predicate : std::string {
    using std::string::string;
    bool is_leaf_v = true;
    template <typename T, typename = std::enable_if_t<std::is_convertible<T, boost::multiprecision::cpp_int>::value>>
        Predicate(T qubit) : std::string(static_cast<boost::multiprecision::cpp_int>(qubit).str()) { is_leaf_v = false; }
    bool is_leaf() const { return is_leaf_v; }
    bool is_internal() const { return !is_leaf_v; }
    boost::multiprecision::cpp_int qubit() const { return is_leaf() ? 0 : boost::multiprecision::cpp_int(*this); }
    Predicate operator*(const Predicate &o) const {
        if (*this == "true") return o;
        if (o == "true") return *this;
        if (*this == "false" || o == "false") return "false";
        return Predicate(std::string("(and " + *this + " " + o + ")").c_str());
    }
};

struct AUTOQ::Util::Constraint {
    std::string content;
    Constraint(const std::string &s) : content(s) {}
    operator std::string() const { return content; }
    std::vector<std::string> to_exprs(Symbolic s) {
        assert(s.size() == 5);
        std::vector<std::string> result;
        for (int i=0; i<4; i++) {
            std::string expr = "0";
            for (const auto &kv : s.at(i)) {
                expr = "(+ " + expr + " (* " + kv.second.str() + " " + kv.first + "))";
            }
            expr = "(/ " + expr + " (pow_sqrt2_k " + s.at(4)["1"].str() + "))";
            result.push_back(expr);
            content.append("(assert (= ");
            content.append(boost::multiprecision::pow(boost::multiprecision::cpp_int(2), static_cast<int>(s.at(4)["1"])).convert_to<std::string>());
            content.append(std::regex_replace(" (* (pow_sqrt2_k $) (pow_sqrt2_k $))))(assert (>= (pow_sqrt2_k $) 0))", std::regex("\\$"), s.at(4)["1"].str()));
        }
        content = "(declare-fun pow_sqrt2_k (Int) Real)" + content;
        return result;
    }
};

namespace AUTOQ {
	namespace Util {
        bool is_spec_satisfied(const Constraint &C, const SymbolicAutomata &Ae, const PredicateAutomata &As);
        bool check_validity(Constraint C, const PredicateAutomata::InitialSymbol &ps, const SymbolicAutomata::InitialSymbol &te);
    }
}

namespace std {
//     template<> class numeric_limits<__int128_t> {
//         public:
//             static __int128_t max() {
//                 return (static_cast<__int128_t>(numeric_limits<__int64_t>::max()) << 64) + numeric_limits<__uint64_t>::max();
//             }
//             static __int128_t min() {
//                 return static_cast<__uint128_t>(1) << 127;
//             }
//             inline static int digits = 127;
//     };
//     template<> struct hash<__int128_t> {
//         size_t operator()(__int128_t var) const {
//             return std::hash<__uint64_t>{}(static_cast<__uint64_t>(var) ^ static_cast<__uint64_t>(var >> 64));
//         }
//     };
    template <> struct hash<typename AUTOQ::Util::TreeAutomata::Symbol> {
        size_t operator()(const AUTOQ::Util::TreeAutomata::Symbol& obj) const {
            std::size_t seed = 0;
            boost::hash_combine(seed, obj.first);
            boost::hash_combine(seed, obj.second);
            return seed;
        }
    };
    template <> struct hash<typename AUTOQ::Util::SymbolicAutomata::Symbol> {
        size_t operator()(const AUTOQ::Util::SymbolicAutomata::Symbol& obj) const {
            std::size_t seed = 0;
            boost::hash_combine(seed, obj.first);
            boost::hash_combine(seed, obj.second);
            return seed;
        }
    };
}

#endif
