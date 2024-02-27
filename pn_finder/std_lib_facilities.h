/*
*	                                 DISCLAIMER
* 
*	This file was originally sourced from https://www.stroustrup.com/programming_support.html. 
*	under "Supporting material:".
* 
*	I edited this file to make it easier for me to read through, and added functions to help
*	solve problems found in Programming Principles and Practice 2nd Edition.
* 
*	My edits are documented in the UNOFFICIAL CHANGES: list, listed January 2024 and later.
* 
*	Some changes won't be documented (mainly because they're too small and insignificant)
*	and will be acknowledged when a larger change is made to the file.
* 
*	I'm at a point where I won't need this file anymore, but I'll keep it around...
*/

/*
?	                           std_lib_facilities.h
*	
*	simple "Programming: Principles and Practice using C++ (second edition)" course header to
*	be used for the first few weeks.
*	It provides the most common standard headers (in the global namespace)
*	and minimal exception/error support.
*	
*	Students: please don't try to understand the details of headers just yet.
*	All will be explained. This header is primarily used so that you don't have
*	to understand every concept all at once.
*	
*	By Chapter 10, you don't need this file and after Chapter 21, you'll understand it
* 
* 
* CHANGELOG:
* 
*	Revised: Apr 25, 2010 - simple_error() added.
*
*	Revised: Nov 25, 2013 - remove support for pre-C++11 compilers, use C++11: <chrono>.
*	Revised: Nov 28, 2013 - add a few container algorithms.
*
*	Revised: Jun 8,  2014 - added #ifndef to workaround Microsoft C++11 weakness.
*
*	Revised: Feb 2,  2015 - randint() can now be seeded (see exercise 5.13).
*
*	Revised: Aug 3,  2020 - a cleanup removing support for ancient compilers.
*
* 
* UNOFFICIAL CHANGES:
* 
*	Revised: Jan 20, 2024 - simple reformatting to make reading easier, also removed using namespace std statement
*	                        and changed #ifndef directive.
* 
*	Revised: Jan 21, 2024 - added DMath namespace. DMath will hold math functions not found within the standard
*	                        library, or experimental math function redefines. Also commented out the vector #define
*	                        in-between Vector and String structs, I wasn't able to create vector objects because
*	                        of it.
* 
*	Revised: Feb 26, 2024 - Removed some functions from DMath, also added pragma warning because I wanted to use /Wall and /WX, and some
*	                        warnings were unavoidable. I also learned that the "#define vector Vector" macro was supposed to trick me into
*	                        using the range-checked Vector class... whoops :|
*/
#pragma once
#ifndef PPP2__std_lib_facilities__H
#define PPP2__std_lib_facilities__H

/*
*	THESE ARE FOR VISUAL STUDIO 2022, I DON'T KNOW IF THESE WILL WORK WITH ANY OTHER COMPILER? oh well :|
* 
*	These are put here to allow use of /Wall with /WX compiler args, raising any warning at the slightest fuck-up, and treating is as an ERROR!
*	Sometimes, the compiler generates warnings because of something either in this file, or (in the case of C4668), something within the std C++ lib
*	itself is causing a warning...Totally overkill I know, but anytime I get a warning from the slightest thing, It presents a learning opportunity,
*	which is always welcome.
*/
#pragma warning( disable : 4296 ) // when control always evaluates to false, this warning gets raised, we don't want... DISABLED!
#pragma warning( disable : 4365 ) // conversion when comparing to ints with different signs/sizes... DISABLED!
#pragma warning( disable : 4388 ) // sign mis-match when comparing, similar to above I think???
#pragma warning( disable : 4514 ) // when a function isn't used, compiler removes it, raising a warning just in case... DISABLED!
#pragma warning( disable : 4668 ) // __STDC_WANT_SECURE_LIB__ causing this one, totally out of my control :(
#pragma warning( disable : 4820 ) // this one has to do with padding and alignment of memory? I don't know what that means yet. DISABLED!
#pragma warning( disable : 5045 ) // /Qspectre compiler arg causing this, this can be ignored.


#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <list>
#include <forward_list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <regex>
#include <random>
#include <stdexcept>
#include <chrono>


namespace DMath { ///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
*	I opted to go with double instead of int even though factorial only operates on
*	integers. Now the largest number (besides inf, if you accept that) that can be
*	represented is 170!.
*
*	This function should only take integers, double is used for the range, show
*	error for input of double, but still calculate the result.
*
* USAGE:
*	DMath::factorial(5.0);
* or
*	double variable = DMath::factorial(5.5) <- this will truncate, but works
*
*/
double factorial(double factoriand)
{
	if (factoriand == 0) { /// Math says so, so I say so. :|
		return 1.0;
	}
	if (factoriand < 0) {
		std::cerr
			<< "[ERROR] Factorial operating on negative number...\n\a";
		return NAN;
	}
	double result = static_cast<int>(factoriand); // cast to truncate these variables
	double temp   = static_cast<int>(factoriand);
	if (result != factoriand) {
		std::cerr
			<< "[WARNING] Using floating-point type on factorial operator...\n"
			<< "          Truncation will occur!\n\a";
	}
		
	/*
	*	After all that setup, this is the entire algorithm for factorial...
	*	can this even be called an "algorithm"?
	*/
	for (int i = 1; i < static_cast<int>(factoriand); i++) {
		result *= temp - 1;
		temp--;
	}
	return result;
}


}///namespace DMath ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


typedef long Unicode;


template<class T>
std::string to_string(const T& t)
{
	std::ostringstream os;
	os << t;
	return os.str();
}

struct Range_error : std::out_of_range
{
	int index;
	Range_error(int i) :
		out_of_range("Range error: " + to_string(i)), index(i)
	{}
};


template< class T>
struct Vector : public std::vector<T>
{
	using size_type = typename std::vector<T>::size_type;
	using std::vector<T>::vector;	// inheriting constructor

	T& operator[](unsigned int i) // rather than return at(i);
	{
		if (i<0 || this->size() <= i) {
			throw Range_error(i);
		}
		return std::vector<T>::operator[](i);
	}
	const T& operator[](unsigned int i) const
	{
		if (i<0 || this->size() <= i) {
			throw Range_error(i);
		}
		return std::vector<T>::operator[](i);
	}
};

// disgusting macro hack to get a range checked vector:
//#define vector Vector										// was this even necessary? | feb 26, 2024: yes...

// trivially range-checked string (no iterator checking):
struct String : std::string
{
	using size_type = std::string::size_type;

	char& operator[](unsigned int i) // rather than return at(i);
	{
		if (i<0 || size() <= i) {
			throw Range_error(i);
		}
		return std::string::operator[](i);
	}

	const char& operator[](unsigned int i) const
	{
		if (i<0 || size() <= i) {
			throw Range_error(i);
		}
		return std::string::operator[](i);
	}
};


namespace std
{
	template<> struct hash<String>
	{
		size_t operator()(const String& s) const
		{
			return hash<std::string>()(s);
		}
	};

} // of namespace std


struct Exit : std::runtime_error
{
	Exit() :
		std::runtime_error("Exit")
	{}
};


// error() simply disguises throws:
inline void error(const std::string& s)
{
	throw std::runtime_error(s);
}

inline void error(const std::string& s, const std::string& s2)
{
	error(s + s2);
}

inline void error(const std::string& s, int i)
{
	std::ostringstream os;
	os << s << ": " << i;
	error(os.str());
}


template<class T>
char* as_bytes(T& i)	// needed for binary I/O
{
	void* addr = &i;	// get the address of the first byte of memory used to store the object
	return static_cast<char*>(addr); // treat that memory as bytes
}


inline void keep_window_open()
{
	std::cin.clear();
	std::cout << "Please enter a character to exit\n";
	char ch;
	std::cin >> ch;
	return;
}

inline void keep_window_open(std::string s)
{
	if (s == "") {
		return;
	}
	std::cin.clear();
	std::cin.ignore(120, '\n');
	for (;;)
	{
		std::cout << "Please enter " << s << " to exit\n";
		std::string ss;
		while (std::cin >> ss && ss != s)
		{
			std::cout << "Please enter " << s << " to exit\n";
		}
		return;
	}
}

// error function to be used (only) until error() is introduced in Chapter 5:
inline void simple_error(std::string s)	// write ``error: s and exit program
{
	std::cerr << "error: " << s << '\n';
	keep_window_open();		// for some Windows environments
	exit(1);
}

// make std::min() and std::max() accessible on systems with antisocial macros:
#undef min
#undef max

// run-time checked narrowing cast (type conversion). See ???.
template<class R, class A>
R narrow_cast(const A& a)
{
	R r = R(a);
	if (A(r) != a) {
		error(std::string("info loss"));
	}
	return r;
}

inline std::default_random_engine& get_rand()
{
	static std::default_random_engine ran;	// note: not thread_local
	return ran;
};

inline void seed_randint(int s)
{
	get_rand().seed(s);
}

inline int randint(int min, int max)
{
	return std::uniform_int_distribution<>{min, max}(get_rand());
}

inline int randint(int max)
{
	return randint(0, max);
}

template<typename C>
using Value_type = typename C::value_type;

template<typename C>
using Iterator = typename C::iterator;

template<typename C>
void sort(C& c)
{
	std::sort(c.begin(), c.end());
}

template<typename C, typename Pred>
void sort(C& c, Pred p)
{
	std::sort(c.begin(), c.end(), p);
}

template<typename C, typename Val>
Iterator<C> find(C& c, Val v)
{
	return std::find(c.begin(), c.end(), v);
}

template<typename C, typename Pred>
Iterator<C> find_if(C& c, Pred p)
{
	return std::find_if(c.begin(), c.end(), p);
}

#endif //PPP2__std_lib_facilities__H
