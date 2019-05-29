#pragma once
#include "utilities/iter_tools/tuple_container.hpp"
#include <limits>
// For numeric_limits
namespace utilities {
namespace detail_ {

/// Functor for calling the size member of each container.
struct ZipSizeFunctor {
    static constexpr std::size_t initial_value =
      std::numeric_limits<std::size_t>::max();

    template<std::size_t, typename T>
    auto run(std::size_t curr_min, T&& container) const {
        return std::min(curr_min, container.size());
    }
};

struct ZipIncrementFunctor {
    /// Functor for finding if any element is true via reduction
    struct AnyTrue {
        template<std::size_t, typename T>
        bool run(bool val, T&& element) const {
            return val || element;
        }
    };
    struct Comparer {
        template<std::size_t, typename lhs_type, typename rhs_type>
        auto run(lhs_type&& lhs, rhs_type&& rhs) const {
            return lhs == rhs;
        }
    };
    template<typename IteratorType, std::size_t... I>
    void run(const IteratorType& start, const IteratorType& end,
             IteratorType& value, std::index_sequence<I...>) {
        value       = std::make_tuple((++std::get<I>(value))...);
        auto at_end = tuple_transform(value, end, Comparer());
        bool done   = tuple_accumulate(at_end, AnyTrue(), false);
        if(done) value = end;
    }
};

} // namespace detail_

/** @brief Wrapper function that makes a Zip container.
 *
 *  The purpose/usage of the resulting object is perhaps best explained with an
 *  example.  Consider the following snippet.
 *
 *  @code
 *  std::vector<int> list1({1,2,3,4});
 *  std::vector<int> list2({2,3,4,5});
 *  for(auto& x : Zip(list1.begin(),list2.begin())
 *      std::cout<< std::get<0>(x) << " " << std::get<1>(x) <<std::endl;
 *  @endcode
 *
 *  The result would be:
 *
 *  @verbatim
 *  1 2
 *  2 3
 *  3 4
 *  4 5
 *  @endverbatim
 *
 *  Thus the resulting class increments an arbitrary number of iterators in
 *  unison as if the tuple of elements were in stored in a container (they are
 *  generated on the fly and not actually stored).
 *
 *  @note If the lengths of the iterators are unequal iteration will stop when
 *  the shortest iterator runs out of elements.
 *
 * @tparam ContainerTypes The types of the containers to zip together.  Must
 *         minimally meet the concept of container.
 * @param containers The actual container instances to zip together.  Containers
 *        will be captured by reference or constant reference as appropriate.
 * @return A TupleContainerImpl instance that is usable in a foreach loop
 * @throws ??? Throws if any of the size() members throw.  Strong throw
 *             guarantee.
 */
template<typename... ContainerTypes>
auto Zip(ContainerTypes&&... containers) {
    return detail_::TupleContainerImpl<
      detail_::ZipIncrementFunctor, std::remove_reference_t<ContainerTypes>...>(
      detail_::ZipSizeFunctor{}, std::forward<ContainerTypes>(containers)...);
}

} // namespace utilities
