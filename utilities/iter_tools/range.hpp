#pragma once
#include "utilities/iter_tools/range_container.hpp"
#include "utilities/type_traits/iterator_types.hpp"

namespace utilities {
namespace detail_ {

/** @brief Implements a generator that produces all elements in a sequence
 *
 *  This class is the guts of the Range semantics.  It basically holds the
 *  start, end, increment, and current value and keeps adding or subtracting
 *  increment from the current value until end is reached.  No checks to make
 *  sure we haven't stepped out of the range are ever performed.
 *
 *  Note that care needs to be taken with the arithmetic in case we have a mix
 *  of signed and unsigned quantities.
 *
 *  @tparam element_type The type of the value in the sequence.  Assumed to be
 *          a numeric POD type.
 */
template<typename element_type>
class RangeItr
  : public RandomAccessIteratorBase<RangeItr<element_type>, element_type> {
    private:
    /// Typedef of the base type for sanity
    using base_type =
      detail_::RandomAccessIteratorBase<RangeItr<element_type>, element_type>;

    public:
    /// Pulls the const_reference typedef into scope
    using const_reference = const element_type&;

    /// Pulls the difference_type typedef into scope
    using typename base_type::difference_type;

    /**
     * @brief Makes a default RangeItr instance.
     *
     * By default a RangeItr instance is capable of iteratring over the empty
     * range.
     *
     * @throw ??? Throws if the default constructors of element_type throw.
     * Strong throw guarantee.
     */
    RangeItr() = default;

    /**
     * @brief Deep copies another RangItr instance.
     *
     * @param[in] rhs The RangeItr to deep-copy.
     * @throw ??? If either element_type's or difference_type's copy
     * constructors throw.  Strong throw guarantee.
     */
    RangeItr(const RangeItr& /*rhs*/) = default;

    /**
     * @brief Takes ownership of another RangeItr instance's state.
     * @param[in] rhs The RangeItr to take ownership of.  After this function is
     * called @p rhs is in a valid, but otherwise undefined state.
     * @throw ??? Throws if either element_type's or difference_type's copy
     * constructors throw.  Strong throw guarantee.
     */
    RangeItr(RangeItr&& /*rhs*/) = default;

    /**
     * @brief Frees up memory associated with this iterator.
     * @throw None. No throw guarantee.
     */
    ~RangeItr() noexcept = default;

    /**
     * @brief Assigns a deep copy of another RangeItr to the current instance.
     *
     * @param[in] rhs The RangeItr instance to deep-copy.
     * @return The current instance containing a deep-copy of @p rhs's state.
     * @throw ??? Throws if element_type's or difference_type's copy
     * constructors throw.  Strong throw guarantee.
     */
    RangeItr& operator=(const RangeItr& /*rhs*/) = default;

    /**
     * @brief Assigns a deep copy of another RangeItr to the current instance.
     * @param[in] rhs The RangeItr instance to take ownership of.  After the
     * call
     * @p rhs is in a valid, but otherwise undefined state.
     * @return The current instance containing @p rhs's state.
     * @throw ??? Throws if element_type's or difference_type's move assignment
     * operator throws.  Strong throw guarantee.
     */
    RangeItr& operator=(RangeItr&& /*rhs*/) = default;

    /**
     * @brief The primary constructor for the RangItr class.
     *
     * This constructor is ultimately called by the Range function to provide
     * the desired API.
     *
     * @param start The value the range starts at.
     * @param stop  The value just after the last value in the range
     * @param increment The amount to increment by each iteration.
     *
     * @throw ??? If copying element_type or difference_type throws.  Strong
     * throw guarantee.
     */
    RangeItr(element_type start, element_type stop, difference_type increment) :
      start_(start),
      stop_(stop),
      increment_(increment),
      value_(start_) {}

    private:
    /// Allows base class to implement random access iterator
    friend base_type;

    /// Implements operator*()
    const_reference dereference() const override { return value_; }

    /// Implements operator++
    RangeItr& increment() override { return advance(increment_); }

    /// Implements operator--
    RangeItr& decrement() override { return advance(-1 * increment_); }

    /// Implements operator+=
    RangeItr& advance(difference_type adv) override {
        adv > 0 ? value_ += adv : value_ -= -1 * adv;
        return *this;
    }

    /// Implements itr1 - itr2
    difference_type distance_to(const RangeItr& rhs) const noexcept {
        const bool is_positive = rhs.value_ > value_;
        difference_type abs_diff =
          is_positive ? rhs.value_ - value_ : value_ - rhs.value_;
        return is_positive ? abs_diff : -1 * abs_diff;
    }

    /** @brief Implements iterator equality comparisons.
     *
     *  Two RangeItrs are defined to be equal if they currently point to the
     *  same value regardless of whether or no that value resides in the
     *  same container.
     *
     *  @param rhs The RangeItr to compare to.
     *  @return True if this and @p rhs point to the same value and false
     *  otherwise.
     *  @throw None. No throw guarantee.
     */
    bool are_equal(const RangeItr& rhs) const noexcept {
        return value_ == rhs.value_;
    }

    /// The first value in our range
    element_type start_ = 0;

    /// The last value in our range
    element_type stop_ = 0;

    /// The amount to increment by
    difference_type increment_ = 1;

    /// The current value of the iterator
    element_type value_ = 0;
};

/** @brief Simulates a container filled with all the values in a range.
 *
 *  The Range class is a C++ implementation of Python's range generator.  This
 *  object behaves as if it is a container filled with all numeric values in a
 *  range [start, stop) that are separated by some defined amount (by default
 *  1).
 *
 *  Like many of the containers in the IterTools sublibrary, this container does
 *  not actually store all of the values in the range, but rather computes them
 *  on the fly.  It's worth noting that this class does not assume that the
 *  range is integral (although it does default to it).  That is it is possible
 *  to generate ranges like:
 *
 *  @verbatim
 *  1.1, 1.2, 1.3, ..., 1.9
 *  @endverbatim
 *
 *  with a call like:
 *  @code
 *  Range(1.1, 2.0, 0.1);
 *  @endcode
 *
 *  The Range class satisfies the C++ concept of container.
 *
 *  @tparam element_type The type of the numbers inside this container.  Must
 *  satisfy the concept of copyable.
 */
template<typename element_type = std::size_t>
class RangeImpl
  : public detail_::RangeContainer<detail_::RangeItr<element_type>> {
    private:
    /// Typedef of base class for sanity
    using base_type = detail_::RangeContainer<detail_::RangeItr<element_type>>;

    public:
    using typename base_type::difference_type;

    /**
     * @brief Makes an empty range.
     *
     * @throw ??? Throws if element_type's default constructor throws.  Strong
     * throw guarantee.
     */
    RangeImpl() = default;

    /**
     * @brief Makes the current container be filled with a deep copy of another
     * RangeImpl container.
     *
     * @param[in] rhs The container to copy.
     * @throw ??? Throws if the copy constructor of the iterator throws. Strong
     * throw guarantee.
     */
    RangeImpl(const RangeImpl& /*rhs*/) = default;

    /**
     * @brief Takes ownership of another RangeImpl instance.
     *
     * @param[in] rhs The instance to take ownership of.  After this call the
     * instance is in a valid, but otherwise undefined state.
     * @throw ??? Throws if the move constructor of the iterator throws.
     */
    RangeImpl(RangeImpl&& /*rhs*/) = default;

    /**
     * @brief Frees up memory associated with the current RangeImpl instance.
     *
     * Note because the iterators maintain all state iterators to this container
     * remain valid even after the container has been removed from scope.
     *
     * @throw None. No throw guarantee.
     */
    ~RangeImpl() noexcept = default;

    /**
     * @brief Assigns a deep-copy of another RangeImpl to the current instance.
     *
     * @param[in] rhs The instance to copy.
     * @return The current instance containing a deep-copy of @p rhs's state
     * @throw ??? Throws if the copy constructor of the iterator throws.  Strong
     * throw guarantee.
     */
    RangeImpl& operator=(const RangeImpl& /*rhs*/) = default;

    /**
     * @brief Takes ownership of another RangeImpl instance's state.
     *
     * @param[in] rhs The instance to take ownership of.  After this call @p rhs
     * is in a valid, but otherwise undefined state.
     *
     * @return The current instance containing @p rhs's state.
     * @throw ??? Throws if the move constructor of the iterator throws.  Strong
     * throw guarantee.
     */
    RangeImpl& operator=(RangeImpl&& /*rhs*/) noexcept = default;

    /**
     * @brief The most useful constructor for the RangeContainer class.
     *
     * Makes a RangeContainer holding all values [start, stop).  The difference
     * between subsequent values is determined by @p increment
     *
     * @note It is assumed that start+n*increment == stop for some integer n. It
     *       is undefined behavior if this is not true.
     *
     * @param start The first value in the sequence.
     * @param stop  The value to stop before.
     * @param increment The number to add to generate the next value in the
     *                  sequence.  Can be negative to make a sequence countdown.
     */
    RangeImpl(element_type start, element_type stop,
              difference_type increment = 1) :
      base_type(RangeItr<element_type>(start, stop, increment),
                RangeItr<element_type>(stop, stop, increment),
                stop > start ? (stop - start) / increment :
                               (start - stop) / (-1 * increment)) {}
};

} // namespace detail_

/** @brief Wrapper function for making a RangeImpl object.
 *  @relates detail_::RangeImpl
 *
 *  This wrapper exists so that the user doesn't have to specify the template
 *  parameter for the type, but rather it can be determined automatically.
 *
 * @tparam element_type The type of the elements in the produced range.  Must
 *         be copyable.
 * @param stop The value [0, stop) for the produced range
 * @return A container with the resulting range.
 * @throw ??? Throws if RangeImpl's constructor throws.  Strong throw guarantee.
 */
template<typename element_type>
auto Range(element_type stop) {
    return detail_::RangeImpl<element_type>(0, stop, 1);
}

/** @brief Wrapper function for making a RangeImpl object.
 *  @relates detail_::RangeImpl
 *
 *  This wrapper exists so that the user doesn't have to specify the template
 *  parameter for the type, but rather it can be determined automatically.
 *
 * @tparam element_type The type of the elements in the produced range.  Must
 *         be copyable.
 * @param start The first value in the range.
 * @param stop  Just past the last value in the desired range
 * @param increment The amount to increase each value by
 *
 * @return A container with the resulting range.
 * @throw ??? Throws if RangeImpl's constructor throws.  Strong throw guarantee.
 */
template<typename element_type>
auto Range(element_type start, element_type stop, element_type increment = 1) {
    return detail_::RangeImpl<element_type>(start, stop, increment);
}

} // namespace utilities
