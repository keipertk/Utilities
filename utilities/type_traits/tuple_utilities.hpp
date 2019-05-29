#pragma once
#include "utilities/type_traits/type_traits_extensions.hpp"
#include <tuple>
#include <type_traits>

/** @file TupleUtilities.hpp
 *
 *  This file collects a series of useful utilities for doing template
 *  meta-programming using tuples.  Their actual definitions are a bit messy so
 *  it is highly recommended you consult their documentation or the tests to see
 *  how to use them.  For the most part the algorithms in this file strive to
 *  emulate the STL container routines with similar names.
 *
 *  - tuple_transform: element-wise applies a unary/binary functor to a
 *                     tuple/pair of tuples and collects the results in a new
 *                     tuple
 *  - tuple_accumulate: applies a functor to each element of a tuple and
 *                      accumulates the results in a single value
 *  - tuple_find_if: returns the first index of the tuple element for which a
 *                   functor returns true
 *
 */

namespace utilities {
namespace detail_ {

/** @brief Type that will tell us if we have iterated over all elements of a
 *         tuple.
 *
 *  If @p I is the same as the number of elements in the tuple the resulting
 *  class will contain a bool member @p value set to true.  Otherwise @p value
 *  will be set to false.
 *
 *  @tparam I The iteration number.
 *  @tparam tuple_type The type of the tuple we are iterating over.
 */
template<std::size_t I, typename tuple_type>
using recursion_done = std::is_same<
  std::integral_constant<std::size_t, I>,
  std::integral_constant<std::size_t,
                         std::tuple_size<std::decay_t<tuple_type>>::value>>;

/** @brief Type that will tell us if we are not done iterating over all
 * elements of a tuple.
 *
 *  If @p I is the same as the number of elements in the tuple the resulting
 *  class will contain a bool member @p value set to false.  Otherwise @p value
 *  will be set to true.
 *
 *  @note One could negate the result of recursion_done instead of using this
 *  class; however, doing so results in the same type and can't be used for
 *  SFINAE.
 *
 *  @tparam I The iteration number.
 *  @tparam tuple_type The type of the tuple we are iterating over.
 */
template<std::size_t I, typename tuple_type>
using recursion_not_done = Negation<recursion_done<I, tuple_type>>;

/// Enables a function via SFINAE if recursion through a tuple has finished
template<std::size_t I, typename tuple_type>
using done_recursing =
  std::enable_if<recursion_done<I, tuple_type>::value, int>;

/// Enables a function via SFINAE if recursion through a tuple is still going
template<std::size_t I, typename tuple_type>
using recursing = std::enable_if<recursion_not_done<I, tuple_type>::value, int>;

/// Actually implements the tuple_transform function
template<typename tuple_type, typename functor_type, std::size_t... I>
auto tuple_transform_impl(tuple_type&& tuple, functor_type&& functor,
                          std::index_sequence<I...>) {
    return std::make_tuple(
      functor.template run<I>(std::get<I>(std::forward<tuple_type>(tuple)))...);
};

/// The guts behind actually combining tuples via a functor
template<typename lhs_type, typename rhs_type, typename functor_type,
         std::size_t... I>
auto tuple_transform_impl(lhs_type&& lhs, rhs_type&& rhs,
                          functor_type&& functor, std::index_sequence<I...>) {
    return std::make_tuple(
      functor.template run<I>(std::get<I>(std::forward<lhs_type>(lhs)),
                              std::get<I>(std::forward<rhs_type>(rhs)))...);
};

/// End-point for reducing a tuple
template<std::size_t I, typename tuple_type, typename functor_type,
         typename return_type, typename done_recursing<I, tuple_type>::type = 0>
return_type tuple_accumulate_impl(tuple_type&&, functor_type&&,
                                  return_type val) {
    return val;
};

/// The guts of the recursive process for reducing a tuple
template<std::size_t I, typename tuple_type, typename functor_type,
         typename return_type, typename recursing<I, tuple_type>::type = 0>
return_type tuple_accumulate_impl(tuple_type&& tuple, functor_type&& functor,
                                  return_type val) {
    auto new_val = functor.template run<I>(
      val, std::get<I>(std::forward<tuple_type>(tuple)));
    return tuple_accumulate_impl<I + 1>(std::forward<tuple_type>(tuple),
                                        std::forward<functor_type>(functor),
                                        new_val);
};

template<std::size_t I, typename tuple_type, typename functor_type,
         typename done_recursing<I, tuple_type>::type = 0>
std::size_t tuple_find_if_impl(tuple_type&& t, functor_type&& functor) {
    return I;
}

template<std::size_t I, typename tuple_type, typename functor_type,
         typename recursing<I, tuple_type>::type = 0>
std::size_t tuple_find_if_impl(tuple_type&& t, functor_type&& functor) {
    return functor.template run<I>(std::get<I>(t)) ?
             I :
             tuple_find_if_impl<I + 1>(std::forward<tuple_type>(t),
                                       std::forward<functor_type>(functor));
}

} // namespace detail_

/** @brief Given a tuple of objects this function will apply a functor to
 * each of them and return the results as a tuple.
 *
 * @param[in] tuple The tuple to apply the functor to.
 * @param[in] functor An instance of the functor to use.
 *
 * @tparam tuple_type The type of the std::tuple we are applying a functor to.
 * @tparam functor_type The type of the functor we are applying to the tuple.
 * The functor's operator() must be take a single argument, which will be one of
 * the tuple's elements (hence for tuples with hetrogenous types, it must be
 * templated on the input type).  Hint: one can use auto as the return type to
 * avoid having to do additional template meta-programming.
 *
 */
template<typename tuple_type, typename functor_type>
auto tuple_transform(tuple_type&& tuple, functor_type&& functor) {
    constexpr std::size_t nelems =
      std::tuple_size<std::decay_t<tuple_type>>::value;
    return detail_::tuple_transform_impl(std::forward<tuple_type>(tuple),
                                         std::forward<functor_type>(functor),
                                         std::make_index_sequence<nelems>());
};

/**
 * @brief Applies a binary operation two tuples returning the result.
 *
 * @tparam lhs_type The type of the tuple on the left of the operation.
 * @tparam rhs_type The type of the tuple on the right of the operation.
 * @tparam functor_type They type of the binary operation to apply
 * @param lhs The tuple on the left.
 * @param rhs The tuple on the right.
 * @param functor The actual functor instance to apply.
 * @return A tuple whose @f$i@f$-th element contains the result of applying
 *         @p functor to the @f$i@f$-th element of @p lhs and @p rhs.
 *
 */
template<typename lhs_type, typename rhs_type, typename functor_type>
auto tuple_transform(lhs_type&& lhs, rhs_type&& rhs, functor_type&& functor) {
    constexpr std::size_t size = std::tuple_size<std::decay_t<lhs_type>>::value;
    static_assert(size == std::tuple_size<std::decay_t<rhs_type>>::value,
                  "LHS size != RHS size");
    return detail_::tuple_transform_impl(
      std::forward<lhs_type>(lhs), std::forward<rhs_type>(rhs),
      std::forward<functor_type>(functor), std::make_index_sequence<size>());
};

/** @brief Applies a reduction to a tuple
 *
 *  @param tuple The tuple to reduce
 *  @param functor The functor instance to use to do the reduction
 *  @param val The initial value for the reduction.
 *  @return The value resulting from reducing all elements of the tuple
 *
 *  @tparam tuple_type The type of the tuple we are reducing
 *
 *  @tparam functor_type The type of the functor doing the reduction.  Must
 *  define a member() with the signature:
 *  @code
 *  //Applies the functor to a tuple element combining it
 *  //param sum is the current reduction total
 *  //param element is the element to reduce into sum
 *  //return is the result of reducing sum and element
 *  return_type operator()(return_type sum, tuple_element element);
 *  @endcode
 *
 *  @tparam return_type The type resulting from a reduction.
 *
 *  @throws ??? Throws if any application of the functor to the tuple throws.
 *              Throw guarantee is same as that of functor.
 */
template<typename tuple_type, typename functor_type, typename return_type>
return_type tuple_accumulate(tuple_type&& tuple, functor_type&& functor,
                             return_type val) {
    return detail_::tuple_accumulate_impl<0>(
      std::forward<tuple_type>(tuple), std::forward<functor_type>(functor),
      val);
};

/**
 * @brief Applies a functor to each element of a tuple and returns the index
 *        of the tuple element for which the functor first evaluates to true.
 * @tparam tuple_type The type of the tuple.
 * @tparam functor_type The type of a unary functor used to "select" an element
 *          of the tuple.
 * @param t The tuple.
 * @param functor The functor instance to apply to each element of the tuple.
 *        Should define a member function bool operator()(T&&) which will be
 *        called for the evaluation.
 * @return The index for which the functor first evaluates to true or the size
 *         of the tuple if no entry evaluates to true.
 */
template<typename tuple_type, typename functor_type>
std::size_t tuple_find_if(tuple_type&& t, functor_type&& functor) {
    return detail_::tuple_find_if_impl<0>(std::forward<tuple_type>(t),
                                          std::forward<functor_type>(functor));
};

} // namespace utilities
