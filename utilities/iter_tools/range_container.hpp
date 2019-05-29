#pragma once
#include <limits> // For std::numeric_limits<size_type>
#include <tuple>  //  For std::tie
#include <vector>

namespace utilities {
namespace detail_ {

/** @brief Simulates a container filled all elements in a range.
 *
 *  This class is largely code factorization from the various IterTools
 *  containers. It contains no actual state aside from copies of the starting
 *  and ending iterators (i.e. it does not actually store every element in the
 *  range like it pretends to).  Consequentially all iterators to this container
 *  are constant iterators as they can not mutate the container.
 *
 *  This class implements most of the container API for the derived class.  The
 *  derived class is only responsible for the constructors.  That said instances
 *  of RangeContainers are never meant to be instantiated by the user.
 *
 *  @todo It's possible (maybe with a bit of tweaking) to remove some of the
 *  boilerplate in IterTools classes using this class as a base class.
 *
 *  @tparam IteratorType The type of iterator used to generate the elements.  We
 *  presently assume it at least meets the concept of bidirectional iterator.
 */
template<typename IteratorType>
class RangeContainer {
    public:
    /// The type of the elements contained within this container
    using value_type = typename IteratorType::value_type;

    /// The type of a reference to an element in this container
    using reference = value_type&;

    /// The type of a reference to a read-only element in this container
    using const_reference = const value_type&;

    /// The type of an iterator over this container.
    using iterator = IteratorType;

    /// The type of an iterator that only reads the elements of this container.
    using const_iterator = IteratorType;

    /// The type of the difference between two elements of this container
    using difference_type = long int;

    /// The type of an offset or index to an element in this container
    using size_type = std::size_t;

    /** @brief Constructs an empty container.
     *
     *  @throw ??? Throws if the default constructor for the iterator throws.
     *  Strong throw guarantee.
     */
    RangeContainer() = default;

    /** @brief The primarily useful constructor capable of making a container
     *         holding the specified range.
     *
     * @param start_itr An iterator pointing to the first element in the
     *        container.
     * @param end_itr An iterator pointing to just past the last element in the
     *        container.
     * @param size_in The number of elements between end_itr and start_itr.
     * @throw ??? If IteratorType's copy constructor throws.  Strong throw
     *        guarantee.
     */
    RangeContainer(iterator start_itr, iterator end_itr, size_type size_in) :
      start_(start_itr),
      end_(end_itr),
      size_(size_in) {}

    /**
     * @brief Returns the @p i -th element of the container.
     * @param i the index of the desired element.
     * @return a copy of the @p i -th element.
     * @throw ??? if either the copy constructor or std::advance throw.  Same
     *        guarantee as IteratorType's copy ctor.
     */
    value_type operator[](size_type i) const {
        const_iterator copy_of_start(start_);
        std::advance(copy_of_start, i);
        return *copy_of_start;
    }

    /** @brief Returns an iterator to first element of the container.
     *
     *  All the state of the container is contained within the iterator and thus
     *  the iterator remains valid even if the container goes out of scope.
     *
     *  @return An iterator pointing to the first element of this container.
     *  @throw ??? Throws if the iterator's copy constructor throws. Same
     *         guarantee as IteratorType's copy constructor.
     */
    iterator begin() { return start_; }

    ///@copydoc begin()
    const_iterator begin() const { return start_; }

    ///@copydoc begin()
    const_iterator cbegin() const { return start_; }

    /**
     * @brief Returns an iterator that points to just past the last element of
     * the container.
     *
     * The entire state of the container is kept in the iterators, hence the
     * resulting iterator is valid even if the container goes out of scope.
     *
     * @return An iterator set to the element just past the last element of the
     * array.
     *
     * @throw ??? Throws if the iterator's copy constructor throws.  Same
     * guarantee as IteratorType's copy ctor.
     */
    iterator end() { return end_; }

    ///@copydoc end()
    const_iterator end() const { return end_; }

    ///@copydoc end()
    const_iterator cend() const { return end_; }

    /** @brief Returns the number of elements in this container.
     *
     *
     *  @return The number of elements in this container.
     *  @throw None. No throw guarantee.
     */
    size_type size() const noexcept { return size_; }

    /**
     * @brief The theoretical maximum size of the container as determined by
     * known software and hardware limitations.
     *
     * Since this container does not actually store its contents, but rather
     * generates them on the fly, the maximum number of elements this container
     * can hold is only bound by the requirement that the size of the container
     * be indexable, which is the same as the maximum value of size_t.
     *
     * @return The theoretical maximum number of elements this container can
     * hold.
     *
     * @throw None. No throw guarantee.
     */
    static constexpr size_type max_size() {
        return std::numeric_limits<size_type>::max();
    }

    /**
     * @brief Used to check if a container "contains" elements.
     *
     * Since RangeContainer instances do not actually contain their elements
     * this check is really ensuring that the range of elements traversed by the
     * iterators is non-empty.
     *
     * @return True if the container is empty and false otherwise.
     * @throw ??? if the equality operator of IteratorType throws.  Same
     * guarantee as IteratorType's equality operator.
     */
    bool empty() const { return start_ == end_; }

    /**
     * @brief Compares two RangeContainer instances for equality.
     *
     * Equality of two RangeContainer instances is defined as having internal
     * iterators that compare equal.
     *
     * @param rhs The RangeContainer instance to compare to.
     * @return True if the two instances are equivalent and false otherwise.
     * @throw ??? Throws if the equality comparison operator of the iterator
     * throws.  Strong throw guarantee.
     */
    bool operator==(const RangeContainer& rhs) const {
        return std::tie(start_, end_) == std::tie(rhs.start_, rhs.end_);
    }

    /**
     * @brief Determines if two RangeContainer instances are different.
     *
     * Equality of RangeContainer instances is defined as wrapping iterators
     * that compare equal.
     *
     * @param rhs The container to compare to.
     * @return True if either iterator in this class differs from the
     * corresponding iterator in @p rhs.
     * @throw ??? Throws if operator==() of the iterators throws.  Same
     * guarantee as equality operator.
     */
    bool operator!=(const RangeContainer& rhs) const {
        return !((*this) == rhs);
    }

    /**
     * @brief Swaps the contents of two RangeContainer instances.
     *
     * @param rhs The container to swap this one with.  After the swap @p rhs
     *        will contain the contents of the current instance.
     * @throw ??? if std::swap(IteratorType, IteratorType) throws. Is no throw
     *        guarantee if swapping IteratorType instances is no throw,
     *        otherwise it's weak at best.
     */
    void swap(RangeContainer& rhs) {
        std::swap(start_, rhs.start_);
        std::swap(end_, rhs.end_);
        std::swap(size_, rhs.size_);
    }

    private:
    /// An iterator pointing to the first element
    iterator start_;

    /// An iterator to just past the last element
    iterator end_;

    /// The number of elements between @p start_ and @p end_
    size_type size_ = 0;
};

} // namespace detail_
} // namespace utilities
