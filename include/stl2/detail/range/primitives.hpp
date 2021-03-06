// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_RANGE_PRIMITIVES_HPP
#define STL2_DETAIL_RANGE_PRIMITIVES_HPP

#include <stl2/detail/fwd.hpp>
#include <stl2/detail/iterator/dangling.hpp>
#include <stl2/detail/iterator/operations.hpp>
#include <stl2/detail/range/access.hpp>
#include <stl2/detail/range/concepts.hpp>

///////////////////////////////////////////////////////////////////////////
// Range primitives [range.primitives]
//
STL2_OPEN_NAMESPACE {
	// enumerate
	namespace ext {
		template<class I, class C>
		struct enumerate_result {
			I end;
			C count;

			template<class I2, class C2>
			requires ConvertibleTo<const I&, I2> && ConvertibleTo<const C&, C2>
			operator enumerate_result<I2, C2>() const& {
				return {end, count};
			}
			template<class I2, class C2>
			requires ConvertibleTo<I, I2> && ConvertibleTo<C, C2>
			operator enumerate_result<I2, C2>() && {
				return {std::move(end), std::move(count)};
			}
		};

		struct __enumerate_fn {
			template<Iterator I, Sentinel<I> S>
			constexpr enumerate_result<I, iter_difference_t<I>>
			operator()(I first, S last) const {
				if constexpr (SizedSentinel<S, I>) {
					auto d = last - first;
					STL2_EXPECT(Same<I, S> || d >= 0);
					return {__stl2::next(std::move(first), std::move(last)), d};
				} else if constexpr (SizedSentinel<I, I>) {
					auto end = __stl2::next(first, std::move(last));
					auto n = end - first;
					return {std::move(end), n};
				} else {
					iter_difference_t<I> n = 0;
					while (first != last) {
						++n;
						++first;
					}
					return {std::move(first), n};
				}
			}

			template<_ForwardingRange R>
			constexpr enumerate_result<iterator_t<R>,
				iter_difference_t<iterator_t<R>>>
			operator()(R&& r) const {
				if constexpr (SizedRange<R>) {
					using D = iter_difference_t<iterator_t<R>>;
					auto n = static_cast<D>(size(r));
					return {__stl2::next(begin(r), end(r)), n};
				} else {
					return (*this)(begin(r), end(r));
				}
			}
		};

		inline constexpr __enumerate_fn enumerate {};
	}

	struct __distance_fn : private __niebloid {
		template<Iterator I, Sentinel<I> S>
		constexpr iter_difference_t<I> operator()(I first, S last) const {
			using D = iter_difference_t<I>;
			D n = 0;
			if constexpr (SizedSentinel<S, I>) {
				n = last - first;
				STL2_EXPECT(Same<I, S> || n >= 0);
			} else if constexpr (SizedSentinel<I, I>) {
				auto end = __stl2::next(first, std::move(last));
				n = end - first;
			} else {
				for (; first != last; ++first) {
					++n;
				}
			}
			return n;
		}

		template<Range R>
		constexpr iter_difference_t<iterator_t<R>> operator()(R&& r) const {
			using D = iter_difference_t<iterator_t<R>>;
			D n = 0;
			if constexpr (SizedRange<R>) {
				n = static_cast<D>(size(r));
			} else {
				n = (*this)(begin(r), end(r));
			}
			STL2_EXPECT(n >= 0);
			return n;
		}
	};

	inline constexpr __distance_fn distance {};
} STL2_CLOSE_NAMESPACE

#endif
