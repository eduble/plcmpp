/*
 * SimpleDigest.h
 *
 *  Created on: 5 févr. 2014
 *      Author: etienne
 */

#pragma once

#include <cstdint>
#include <iostream>
using namespace std;

//#define COMPUTE_DIGEST_STATS

#ifdef COMPUTE_DIGEST_STATS
#define REPORT_DIGEST_STATS() SimpleDigest::report_stats()
#define ADD_DIGEST_FALSE_POSITIVE(a, b, c, d) \
		SimpleDigest::add_false_positive(a, b, c, d)
#else
#define REPORT_DIGEST_STATS()
#define ADD_DIGEST_FALSE_POSITIVE(a, b, c, d)
#endif

/* You can specify a shuffling algorithm at build time by using, for example:
 * export CPPFLAGS='-DSIMPLE_DIGEST_SHUFFLING="d ^= (*begin) << (d%47+1);"'
 * If the hashes are then used many times, it should be valuable to perform
 * an accurate hashing, which will produce few duplicates.
 * Otherwise, the calculation of hashes may be too costly, and you should
 * select a faster (and maybe less accurate) algorithm.
 *  */
#define ACCURATE_SHUFFLING 		d ^= d << (d%47+1); d ^= *begin + d%61 + 1;
#define INTERMEDIATE_SHUFFLING	d ^= (*begin) << (d%47+1);
#define NO_SHUFFLING 			break;

#ifndef SIMPLE_DIGEST_SHUFFLING
#define SIMPLE_DIGEST_SHUFFLING  INTERMEDIATE_SHUFFLING
#endif

namespace util {

#define SEED		0x01030507090b0d0f

class SimpleDigest {

public:
	typedef uint64_t Type;

	template <class T>
	static uint64_t digest(T *begin, T *end)
	{
		uint64_t d = SEED;

		// case of empty range
		if (begin == end)
		{
			return d-1;
		}

		// 1st occurrence, do something simple
		d ^= *begin;
		++begin;

		// next occurrences, we will have to
		// be smarter and shuffle things...
		for (; begin != end; ++begin)
		{
			SIMPLE_DIGEST_SHUFFLING
		}

#ifdef COMPUTE_DIGEST_STATS
		record_hash(d);
#endif
		return d;
	}

	template <class T>
	static void add_false_positive(
			T* t1_start, T* t1_end,
			T* t2_start, T* t2_end)
	{
		cout << "The following 2 integers series returned the same hash (this is a false positive):" << endl;
		cout << "[" ;
		for (auto it = t1_start; it < t1_end; it++)
		{
			cout << " " << (int)*it;
		}
		cout << " ]" << endl;

		cout << "[" ;
		for (auto it = t2_start; it < t2_end; it++)
		{
			cout << " " << (int)*it;
		}
		cout << " ]" << endl;

		increment_false_positives();
	}

	static void record_hash(uint64_t d);
	static void increment_false_positives();
	static void report_stats();
};

} /* namespace util */

