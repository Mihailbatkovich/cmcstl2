// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//  Copyright Christopher Di Bella 2016
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_MEMORY_UNINITIALIZED_FILL_HPP
#define STL2_DETAIL_MEMORY_UNINITIALIZED_FILL_HPP

#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/memory/concepts.hpp>
#include <stl2/detail/memory/construct_at.hpp>
#include <stl2/detail/memory/destroy.hpp>
#include <stl2/detail/tagged.hpp>

STL2_OPEN_NAMESPACE {
	///////////////////////////////////////////////////////////////////////////
	// uninitialized_fill [Extension]
	//
	template<__NoThrowForwardIterator I, Sentinel<I> S, typename T>
	requires
		Constructible<iter_value_t<I>, const T&>
	I uninitialized_fill(I first, S last, const T& x)
	{
		auto guard = detail::destroy_guard<I>{first};
		for (; first != last; ++first) {
			__stl2::__construct_at(*first, x);
		}
		guard.release();
		return first;
	}

	///////////////////////////////////////////////////////////////////////////
	// uninitialized_fill [Extension]
	//
	template<__NoThrowForwardRange Rng, typename T>
	requires
		Constructible<iter_value_t<iterator_t<Rng>>, const T&>
	safe_iterator_t<Rng>
	uninitialized_fill(Rng&& rng, const T& x)
	{
		return __stl2::uninitialized_fill(begin(rng), end(rng), x);
	}

	///////////////////////////////////////////////////////////////////////////
	// uninitialized_fill_n [Extension]
	//
	template<__NoThrowForwardIterator I, typename T>
	requires
		Constructible<iter_value_t<I>, const T&>
	I uninitialized_fill_n(I first, const iter_difference_t<I> n, const T& x)
	{
		return __stl2::uninitialized_fill(
			counted_iterator{std::move(first), n},
			default_sentinel{}, x).base();
	}
} STL2_CLOSE_NAMESPACE

#endif // STL2_DETAIL_MEMORY_UNINITIALIZED_FILL_HPP
