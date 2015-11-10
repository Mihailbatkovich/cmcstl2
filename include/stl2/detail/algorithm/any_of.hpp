// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Andrew Sutton 2014
//  Copyright Gonzalo Brito Gadeschi 2014
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_ALGORITHM_ANY_OF_HPP
#define STL2_DETAIL_ALGORITHM_ANY_OF_HPP

#include <initializer_list>
#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// any_of [alg.any_of]
//
STL2_OPEN_NAMESPACE {
  template <InputIterator I, Sentinel<I> S,
            class Pred, class Proj = identity>
  requires
    models::IndirectCallablePredicate<
      __f<Pred>, projected<I, __f<Proj>>>
  bool any_of(I first, S last, Pred&& pred_, Proj&& proj_ = Proj{})
  {
    auto pred = ext::make_callable_wrapper(__stl2::forward<Pred>(pred_));
    auto proj = ext::make_callable_wrapper(__stl2::forward<Proj>(proj_));
    for (; first != last; ++first) {
      if (pred(proj(*first))) {
        return true;
      }
    }
    return false;
  }

  template <InputRange R, class Pred, class Proj = identity>
  requires
    models::IndirectCallablePredicate<
      __f<Pred>, projected<iterator_t<R>, __f<Proj>>>
  bool any_of(R&& rng, Pred&& pred, Proj&& proj = Proj{})
  {
    return __stl2::any_of(__stl2::begin(rng), __stl2::end(rng),
      __stl2::forward<Pred>(pred), __stl2::forward<Proj>(proj));
  }

  // Extension
  template <class E, class Pred, class Proj = identity>
  requires
    models::IndirectCallablePredicate<
      __f<Pred>, projected<const E*, __f<Proj>>>
  bool any_of(std::initializer_list<E> il,
              Pred&& pred, Proj&& proj = Proj{})
  {
    return __stl2::any_of(il.begin(), il.end(),
      __stl2::forward<Pred>(pred), __stl2::forward<Proj>(proj));
  }
} STL2_CLOSE_NAMESPACE

#endif
