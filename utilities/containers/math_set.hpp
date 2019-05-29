#pragma once
#include <algorithm> //For set_intersection and the like
#include <set>

namespace utilities {

/**
 * @brief Implements an std::unordered_set-like object except that it knows
 * about union, etc.
 *
 *  @note We actually use std::set to avoid the need for requiring the elements
 *  to be hashable.
 *  @note I'm not convinced that the various operations are optimal.  It may be
 *  easier to do them directly with the members of std::set.  Also switching to
 *  a sorted std::vector could possibly speed things up by taking advantage of
 *  the fact that it wouldn't be random memory access.
 *
 * @tparam element_type The type of the elements in the set.  Must be copyable.
 */
template<typename element_type>
class MathSet {
    private:
    /// Typedef of implementing container for easy modification later
    using container_type = std::set<element_type>;

    public:
    /// The type of an element inside this set
    using value_type = typename container_type::value_type;

    /// The type of a reference to an element in this set
    using reference = typename container_type::reference;

    /// The type of a read-only reference to an element in this set
    using const_reference = typename container_type::const_reference;

    /// The type of an iterator to this set
    using iterator = typename container_type::iterator;

    /// The type of an iterator to a read-only instance of this set
    using const_iterator = typename container_type::const_iterator;

    /// The type of the difference between two iterators
    using difference_type = typename container_type::difference_type;

    /// The type of index or an offset
    using size_type = typename container_type::size_type;

    /** @brief Constructs an instance that is the empty set.
     *
     *  @throw None. No throw guarantee.
     */
    MathSet() = default;

    /**
     * @brief Deep copies another MathSet instance.
     *
     * @param[in] rhs The MathSet instance to deep copy.
     * @throw std::bad_alloc if their is insufficient memory for the copy.
     * Strong throw guarantee.
     * @throw ??? If the copy constructor of element_type throws.  Strong throw
     * guarantee.
     */
    MathSet(const MathSet& /*rhs*/) = default;

    /**
     * @brief Takes ownership of another MathSet instance's state.
     *
     * @param[in] rhs The MathSet instance to take ownership of.  After this
     * call @p rhs will be in a valid, but undefined state.
     * @throw None. No throw guarantee.
     */
    MathSet(MathSet&&) noexcept = default;

    /**
     * @brief Initializes the set from an initializer list
     *
     * @param il The intializer list being used to set the initial state of the
     * container.
     *
     * @throw std::bad_alloc if there is insufficient memory for copying the
     * elements.  Strong throw guarantee.
     *
     */
    MathSet(std::initializer_list<element_type> il) : elements_(il) {}

    /** @brief Frees up memory associated with the current instance.
     *
     *  After this call all iterators to this class are invalidated.
     *
     *  @throw None. No throw guarantee.
     */
    ~MathSet() noexcept = default;

    /**
     * @brief Makes the current instance a deep copy of another instance.
     *
     * @param[in] rhs The instance to deep copy.
     *
     * @return The current instance now containing a deep copy of @p rhs 's
     * state.
     *
     * @throw std::bad_alloc if there is insufficient memory for the copy.
     * Strong throw guarantee.
     *
     * @throw ??? if the element's copy constructor throws. Strong throw
     * guarantee.
     *
     */
    MathSet& operator=(const MathSet&) = default;

    /** @brief Takes ownership of another MathSet instance's state.
     *
     * @param[in] rhs The MathSet instance to take ownership of.  After this
     * call @p rhs is in a valid, but otherwise undefined state.
     *
     * @return The current instance containing @p rhs 's state.
     * @throw None. No throw guarantee.
     */
    MathSet& operator=(MathSet&&) noexcept = default;

    /** @brief Creates an iterator that points to the first element in this set.
     *
     *  @return An iterator that points to the first element of this set.  The
     *          iterator satisfies bidirectional iterator.
     *  @throw None. No throw guarantee.
     */
    iterator begin() noexcept { return elements_.begin(); }

    /** @brief Creates an iterator that points to the first element in this set
     *         that can not be used to modify the elements of the set.
     *
     *  @copydetails begin()
     */
    const_iterator begin() const { return elements_.cbegin(); }

    ///@copydoc begin()const
    const_iterator cbegin() const { return elements_.cbegin(); }

    /** @brief Creates an iterator that points to just past the last element in
     *  this set.
     *
     *  @return An iterator that points to just past the last element of this
     *  set.  The iterator satisfies bidirectional iterator.
     *  @throw None. No throw guarantee.
     */
    iterator end() { return elements_.end(); }

    /** @brief Creates an iterator that points to just past the last element
     *  in this set that can not be used to modify the elements of the set.
     *
     *  @copydetails end()
     */
    const_iterator end() const { return elements_.cend(); }

    ///@copydoc end()const
    const_iterator cend() const { return elements_.cend(); }

    template<typename input_element>
    auto insert(input_element&& ei) {
        return elements_.insert(std::forward<input_element>(ei));
    }

    /**
     * @brief Returns the number of times an element appears in the current set.
     *
     * Since an element can only appear once in a set the return value of this
     * function is a boolean (true if the element is present and false
     * otherwise).
     *
     * @param ei The element to look for.
     * @return True if @p ei is contained within the current set and false
     * otherwise.
     *
     * @throw None. No throw guarantee.
     *
     */
    bool count(const element_type& ei) const noexcept {
        return elements_.count(ei);
    }

    /**
     * @brief Returns the number of elements in the current set.
     *
     * @return The number of elements currently in the set.
     * @throw None. No throw guarantee.
     */
    size_type size() const noexcept { return elements_.size(); }

    /**
     * @brief Returns the theoretical maximum number of elements this container
     * can accomodate ignoring hardware considerations.
     *
     * @return The maximum number of elements that can be added to this
     * container (theoretically).
     * @throw None. No throw guarantee.
     */
    size_type max_size() const noexcept { return elements_.max_size(); }

    /**
     * @brief Returns true if the current instance is the empty set.
     *
     * @return True if the current instance is the empty set and false if it
     * contains at least one element.
     * @throw None. No throw guarantee.
     */
    bool empty() const noexcept { return elements_.empty(); }

    /**
     * @brief Makes the current instance the union of itself and another
     * MathSet.
     *
     * The union of two sets @f$x@f$ and @f$y@f$ is a third set @f$z@f$ which
     * contains all elements that are present in @f$x@f$ and/or in @f$y@f$.
     * Note that a set will contain at most one copy of a given element.
     *
     * @param rhs The set we are taking a union with.
     * @return The current instance set to the union of itself and @p rhs.
     * @throw std::bad_alloc if there is insufficient memory to copy elements
     * over.  Weak throw guarantee.
     */
    MathSet& union_assign(const MathSet& rhs) {
        auto end_itr = end();
        for(const auto& x : rhs) end_itr = elements_.insert(end_itr, x);
        return *this;
    }

    /**
     * @brief Returns the union of this MathSet instance and another.
     *
     * This function behaves the same as union_assign() except that the
     * result is a new instance.
     *
     * @note union is a protected keyword in the C++ language and hence is not a
     *       valid member function name.
     *
     * @throw std::bad_alloc if there is insufficient memory to copy the
     * elements of this and @p rhs to a common container.  Strong throw
     * guarantee.
     */
    MathSet take_union(const MathSet& rhs) const {
        return MathSet(*this).union_assign(rhs);
    }

    /**
     * @brief Makes this the intersection of itself and another set.
     *
     * The intersection of a set @f$x@f$ and a set @f$y@f$ is the set of all
     * elements appearing in both @f$x@f$ and @f$y@f$.
     *
     * @return the current instance set to the intersection of itself and @p
     * rhs.
     *
     * @throw std::bad_alloc if there is insufficient memory to copy common
     * elements.  Strong throw guarantee.
     */
    MathSet& intersection_assign(const MathSet& rhs) {
        container_type temp;
        auto inserter = std::inserter(temp, temp.end());
        std::set_intersection(begin(), end(), rhs.begin(), rhs.end(), inserter);
        elements_.swap(temp);
        return *this;
    }

    /**
     * @brief Creates a new MathSet that is the intersection of this and rhs
     *
     * This function is basically the same as intersection_assign aside from the
     * fact that it makes a new instance.
     *
     * @param rhs The set to take the intersection with.
     * @return A new MathSet containing the intersection of this and @p rhs.
     * @throw std::bad_alloc if there is insufficient memory to make the new
     * set.  Strong throw guarantee.
     */
    MathSet intersection(const MathSet& rhs) const {
        return MathSet(*this).intersection_assign(rhs);
    }

    /** @brief Makes the current set the set difference of itself and another
     *         set.
     *
     * The set @f$x/y@f$ is the set difference of sets @f$x@f$ and @f$y@f$
     * and is all of the elements in @f$x@f$ that are not in @f$y@f$.
     *
     * @param rhs The set to take the difference with.
     * @return The current instance less elements common to @p rhs.
     * @throw std::bad_alloc if there is insufficient memory to copy common
     * elements over.
     */
    MathSet& difference_assign(const MathSet& rhs) {
        container_type temp;
        auto inserter = std::inserter(temp, temp.end());
        std::set_difference(begin(), end(), rhs.begin(), rhs.end(), inserter);
        elements_.swap(temp);
        return *this;
    }

    /**
     * @brief Returns the set difference of this and another set.
     *
     * This function is basically identical to set_difference_assign aside from
     * the fact that the result is put into a new instance.
     *
     * @param rhs The set to take the difference with.
     * @return A new instance containing the elements that are in this set and
     * not in @p rhs.
     * @throw std::bad_alloc if there is insufficient memory to copy.
     */
    MathSet difference(const MathSet& rhs) const {
        return MathSet(*this).difference_assign(rhs);
    }

    /** @brief Makes the current set the symmetric set difference of itself and
     * another  set.
     *
     * The symmetric set difference of sets @f$x@f$ and @f$y@f$ is the set of
     * all elements appearing in either @f$x@f$ or @f$y@f$, but not in both.
     *
     * @param rhs The set to take the symmetric difference with.
     * @return The current instance less elements common to @p rhs plus unique
     *         elements of @p rhs.
     * @throw std::bad_alloc if there is insufficient memory to copy common
     * elements over.
     */
    MathSet& symmetric_difference_assign(const MathSet& rhs) {
        container_type temp;
        auto inserter = std::inserter(temp, temp.end());
        std::set_symmetric_difference(begin(), end(), rhs.begin(), rhs.end(),
                                      inserter);
        elements_.swap(temp);
        return *this;
    }

    /**
     * @brief Returns the set difference of this and another set.
     *
     * This function is basically identical to set_difference_assign aside from
     * the fact that the result is put into a new instance.
     *
     * @param rhs The set to take the difference with.
     * @return A new instance containing the elements that are in this set and
     * not in @p rhs.
     * @throw std::bad_alloc if there is insufficient memory to copy.
     */
    MathSet symmetric_difference(const MathSet& rhs) const {
        return MathSet(*this).symmetric_difference_assign(rhs);
    }

    /** @brief Returns true if the current MathSet is a proper subset of @p rhs.
     *
     * In order for a set @f$x@f$ to be a proper subset of a set @f$y@f$ every
     * element of @f$x@f$ must be contained in @f$y@f$ and furthermore there
     * must exist at least one element in @f$y@f$ that does not exist in
     * @f$x@f$.
     *
     * @warning A value of false doesn't mean that the current instance is a
     * superset of @p rhs.
     *
     * @param rhs The MathSet to compare to.
     * @return True if the current instance is a proper subset of @p rhs.
     * @throw None. No throw guarantee.
     */
    bool operator<(const MathSet& rhs) const noexcept {
        if(size() >=
           rhs.size()) // Can't be subset if we don't have less elements
            return false;
        return operator<=(rhs);
    }

    /**
     * @brief Returns whether or not the current MathSet is a proper superset of
     *        another MathSet.
     *
     * Given two sets @f$x@f$ and @f$y@f$, @f$x@f$ is a proper superset of
     * @f$y@f$ if and only if all elements of @f$y@f$ are also in @f$x@f$ and
     * all there exists at least one element in @f$x@f$ that is not in @f$y@f$.
     *
     * @param rhs The instance to compare against.
     * @return True if the current set is a proper superset of @p rhs and false
     * otherwise.
     * @throw None. No throw guarantee.
     */
    bool operator>(const MathSet& rhs) const noexcept {
        return rhs < *this; // X superset Y means Y subset of X
    }

    /** @brief Returns whether or not the current MathSet is subset of another
     *         MathSet.
     *
     * The difference between a subset and a proper subset is that all sets are
     * also subsets of themselves (i.e. subset allows for equality whereas a
     * proper subset is strictly less than).
     *
     * @param rhs The set to compare against.
     * @return True if the current instance is a subset of @p rhs and false if
     * there is at least one element that is not in @p rhs.
     * @throw None. No throw guarantee.
     */
    bool operator<=(const MathSet& rhs) const noexcept {
        if(size() > rhs.size()) // Can't be subset with more elements
            return false;
        for(auto& x : *this)
            if(!rhs.count(x)) return false;
        return true;
    }

    /** @brief Returns whether or not the current MathSet is a superset of
     *  another MathSet.
     *
     * The difference between a superset and a proper superset is that all sets
     * are also supersets of themselves (i.e. superset allows for equality
     * whereas proper superset is strictly greater than).
     *
     * @param rhs The set to compare against.
     * @return True if the current instance is a superset of @p rhs and false if
     * there is at least one element in @p rhs that is not in the current
     * instance.
     * @throw None. No throw guarantee.
     */
    bool operator>=(const MathSet& rhs) const noexcept {
        return rhs <= (*this);
    }

    /**
     * @brief Determines if two MathSet instances are equivalent.
     *
     * We define two MathSet instances to be equal if they are both supersets
     * and subsets of eachother.
     *
     * @param rhs The set to compare against.
     * @return True if all elements in this set are in @p rhs and vice versa.
     * @throw None. No throw guarantee.
     */
    bool operator==(const MathSet& rhs) const noexcept {
        return elements_ == rhs.elements_;
    }

    /**
     * @brief Used to determine if two sets are different.
     *
     * Two sets are different if there exists at least one element in one that
     * is not in the other.
     *
     * @param rhs The set to compare against.
     * @return True if there is an element in this set or @p rhs that is not in
     * the other.
     * @throw None. No throw guarantee.
     */
    bool operator!=(const MathSet& rhs) const noexcept {
        return !((*this) == rhs);
    }

    private:
    /// The actual set of elements
    std::set<element_type> elements_ = {};
};

} // namespace utilities
