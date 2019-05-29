#pragma once
#include "utilities/iter_tools/tuple_container.hpp"

namespace utilities {
namespace detail_ {

/// Functor that computes the number of elements in the CartesianProduct
struct CartSizeFunctor {
    /// Empty case is caught by base class
    static constexpr std::size_t initial_value = 1L;

    template<std::size_t, typename T>
    auto run(std::size_t curr_min, T&& container) const {
        return curr_min * container.size();
    }
};

/// Functor used to increment the CartesianProduct iterator
struct CartIncrementFunctor {
    /// Functor that returns true if an iterator is at the end
    template<typename iterator_type>
    struct AtEnd {
        const iterator_type& end;
        AtEnd(const iterator_type& da_end) : end(da_end) {}

        template<std::size_t I, typename T>
        bool run(T&& elem) const noexcept {
            return elem == std::get<I>(end);
        }
    };

    /// Functor that resets all indices starting with @p turn_on
    template<typename iterator_type>
    struct Reseter {
        const iterator_type& start;
        std::size_t turn_on = 0;
        Reseter(const iterator_type& da_start, std::size_t on) :
          start(da_start),
          turn_on(on) {}

        template<std::size_t I, typename T>
        auto run(T&& itr) {
            return (I >= turn_on) ? std::get<I>(start) : itr;
        }
    };

    /// Functor that increments @p turn_on
    struct Incrementer {
        std::size_t turn_on = 0;
        Incrementer(std::size_t on) : turn_on(on) {}

        template<std::size_t I, typename T>
        auto run(T&& itr) {
            return (I == turn_on) ? ++itr : itr;
        }
    };

    /// The function called by TupleContainer
    template<typename IteratorType, std::size_t... I>
    void run(const IteratorType& start, const IteratorType& end,
             IteratorType& value, std::index_sequence<I...>) {
        constexpr std::size_t nelems = sizeof...(I);
        std::size_t idx              = nelems;
        while(true) {
            if(!idx) break; // idx==0 means we have no indices left to try
            // Increment the right most element that we know isn't at end
            auto temp_value = tuple_transform(value, Incrementer(idx - 1));
            // Now get first index at end
            auto new_idx = tuple_find_if(temp_value, AtEnd<IteratorType>(end));
            // If new_idx hasn't changed (i.e. is still idx) that was a good inc
            if(new_idx == idx) {
                value = tuple_transform(temp_value,
                                        Reseter<IteratorType>(start, idx));
                break;
            }
            idx = new_idx; // not good means new_idx has moved down one
        }
    }
};

} // namespace detail_

/** @brief Wrapper function that makes a CartesianProduct container.
 *
 *  The purpose/usage of the resulting object is perhaps best explained with an
 *  example.  Consider the following snippet.
 *
 *  @code
 *  std::vector<int> list1({1,2,3});
 *  std::vector<int> list2({2,3});
 *  for(auto& x : CartesianProduct(list1.begin(),list2.begin())
 *      std::cout<< std::get<0>(x) << " " << std::get<1>(x) <<std::endl;
 *  @endcode
 *
 *  The result would be:
 *
 *  @verbatim
 *  1 2
 *  1 3
 *  2 2
 *  2 3
 *  3 2
 *  3 3
 *  @endverbatim
 *
 *  Thus the resulting class makes all possible tuples (in lexicographical
 *  order) that can be made from the elements in ean container.
 *
 * @tparam ContainerTypes The types of the containers to form the tuples from.
 *         Must minimally meet the concept of container.
 * @param containers The actual container instances to zip together.  Containers
 *        will be captured by reference or constant reference as appropriate.
 * @return A TupleContainerImpl instance that is usable in a foreach loop
 * @throws ??? Throws if any of the size() members throw.  Strong throw
 *             guarantee.
 */
template<typename... ContainerTypes>
auto CartesianProduct(ContainerTypes&&... containers) {
    return detail_::TupleContainerImpl<
      detail_::CartIncrementFunctor,
      std::remove_reference_t<ContainerTypes>...>(
      detail_::CartSizeFunctor{}, std::forward<ContainerTypes>(containers)...);
}

} // namespace utilities
