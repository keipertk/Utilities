#pragma once
#include <algorithm> //For sort and count
#include <deque>     //For returns of fns functions
#include <numeric>   //For inner_product and accumulate
#include <vector>

/** @file Combinatorics.hpp
 *
 * Free functions for things that are vaguely combinatorial in nature.
 *
 *  The current contents of this file are:
 *  - binomial_coefficient : calculates binomial coefficients
 *  - multinomial_coefficient : calculates multinomial coefficients
 *  - n_permutations : calculates the number of permutations of a sequence
 *  - permutation_to_fns : maps a permutation to its representation in the\n
 *                         factorial number system
 *  - fns_to_permutation : reverse mapping of permutation_to_fns
 *  - fns_place_values : returns the place values in the factorial number\n
 *                       system for a given permutation.
 *  - decimal_to_fns: Maps a value in decimal to its corresponding\n
 *                    representation in the factorial number system
 *  - permutation_to_decimal : wrapper over fns intermediate
 *  - decimal_to_permutation : reverse mapping of permutation_to_decimal
 */

/// Main namespace for the Utilities library
namespace utilities {
/// Name space for things not mean to see the light of day
namespace detail_ {

/// Struct that actually implements the binomial coefficient
template<typename T>
struct BCImpl {
    /// Returns @p n choose @p k
    T eval(std::size_t n, std::size_t k);
};

/// Struct that actually implements the multinomial coefficient
template<typename T>
struct MCImpl {
    /// Returns sum(ks) chooses k0, k1, k2,...
    T eval(const std::vector<std::size_t>& ks);
};

} // namespace detail_

/** @brief Computes the binomial coefficient @f${n\choose k}@f$
 *
 *  Depending on the requested return type a different algorithm is used.  If
 *  the user requests an integral return type an algorithm based on Pascal's
 *  triangle will be used.  If the user requests a floating point return type it
 *  will then be assumed that the user is fine with loosing precision *i.e.* the
 *  value will be returned in scientific notation with the significand
 *  truncated to the precision of the floating point value.
 *
 * @tparam T The desired return type.
 * @param n top number of the bc *i.e.* number of objects to pick from
 * @param k bottom number of the bc *i.e.* number of objects to pick at a time
 * @return The requested value of the binomial coefficient
 * @throws std::overflow_error if the requested binomial coefficient can not be
 *         held in the requested return type.
 */
template<typename T>
T binomial_coefficient(std::size_t n, std::size_t k) {
    return detail_::BCImpl<T>().eval(n, k);
}

/** @brief Computes the multinomial coefficient
 * @f${{\sum k_i} \choose {\prod k_i!}}@f$
 *
 * Given @f$N@f$ observations taken from @f$M@f$ unique choices such that the
 * @f$i^\text{th}@f$ unique choice is observed @f$m_i@f$ times, the multinomial
 * coefficient @f${ N \choose {\prod_{i=1}^M m_i!}}@f$, is the number of
 * possible orderings for those observations.
 *
 * @param[in] ks The number of times each unique observation occurs.
 * @tparam T The desired return type.
 * @return sum of @p ks choose k0!,k1!,...
 * @throws None. No throw guarantee.
 *
 */

template<typename T>
T multinomial_coefficient(const std::vector<std::size_t>& ks) noexcept {
    return detail_::MCImpl<T>().eval(ks);
}

/** @brief Given a sequence returns the number of unique permutations
 *
 *  This function is primarily code factorization.  Basically, given a sequence
 *  we can't assume all elements are unique so we can't assume its just the
 *  length of the sequence factorial.  Rather, we need to figure out the
 *  number of times each unique element appears in the sequence and then use
 *  that information to compute the appropriate multinomial coefficient.
 *
 *  @par Memory
 *  The present algorithm will make a copy of @p seq and an
 *  std::vector<std::size_t> of the length of @p seq.
 *
 *  @param[in] seq The sequence for which we wish to know the number of
 *  permutations.
 *  @tparam container_type The type of @p seq.  Must satisfy the sequence
            concept.
 *  @return The number of unique permutations of a sequence
 *  @throws std::bad_alloc if there is not enough memory.  Strong throw
 *          guarantee.
 */
template<typename container_type>
std::size_t n_permutations(const container_type& seq);

/** @brief Given a permutation this function maps that permutation to its value
 *         in the factorial number system.
 *
 * For a description of the factorial number system see the Intro to
 * Combinatorics section of the manual.
 *
 * @par Memory
 * The current algorithm is recursive and thus at a depth @f$i@f$ will require
 * two copies of @p perm (except they contain @f$i-1@f$ less elements).  Memory
 * for the return type will also be allocated.
 *
 * @tparam container_type The type of the sequences
 * @param perm A permutation of @p original
 * @param original The original sequence from which @p perm was generated
 * @return The value of @p perm in the factorial number system.
 * @throws std::bad_alloc if memory allocation fails.  Strong throw guarantee.
 */
template<typename container_type>
std::deque<std::size_t> permutation_to_fns(const container_type& perm,
                                           const container_type& original);

/** @brief Converts a value in the factorial number system to the equivalent
 *         permutation.
 *
 *  @par Memory
 *
 *  This call ultimately will call decimal_to_fns and therefore incurs its
 *  memory overhead as well as that of the return value.
 *
 *  @param[in] fns The value in the factorial number system to convert to a
 *                 permutation.
 *  @param[in] original The original permutation.
 *  @returns The requested permutation.
 *  @throws std::bad_alloc if there is insufficient memory to complete the
 *          operation.
 */
template<typename container_type>
container_type fns_to_permutation(const std::deque<std::size_t>& fns,
                                  const container_type& original);

/** @brief Given a permutation of a sequence computes the values of the places
 *         in the number.
 *
 * For a description of the factorial number system see the Intro to
 * Combinatorics section of the manual.  Since for a general permutation the
 * values of the places depends on the permutation this function will compute
 * the place values.
 *
 * @par Memory
 * At the moment the internal algorithm is recursive requiring a copy of @p perm
 * at each depth @f$i@f$; the copy will contain @f$i@f$ less elements than
 * @p perm. Additionally a call to n_permutations will be made at each depth.
 * Finally, memory for the return type will be allocated.
 *
 * @tparam container_type The type of the sequences
 * @param perm The current permutation
 * @return The value of the places in the factorial number system
 * @throws std::bad_alloc if any memory allocation fails.  Strong throw
 *         guarantee.
 */
template<typename container_type>
std::deque<std::size_t> fns_place_values(const container_type& perm);

/** @brief Given a decimal value and the original sequence, computes the
 *         corresponding value in the factorial number system.
 *
 *  Mapping a number in the factorial number system (FNS) to decimal is done by
 *  taking the inner product of the number's digits and the place values.  This
 *  function is the inverse mapping, taking a value in decimal and returning the
 *  value in the FNS.
 *
 *  @par Memory
 *  The current algorithm requires two copies of @p perm and a call to
 *  n_permutations.  Additionally memory for the return will be allocated.
 *
 *  @param[in] D The requested permutation.  @p D must be in the range
 *        [0, \f$N\f$) where \f$N\f$ is the number of permutations of @p perm.
 *  @param[in] perm The original permutation.
 *  @tparam container_type The type of @p perm.  Must satisfy the concept of a
 *          container.
 *  @returns The value of the @p D -th permutation in the factorial number
 *  system.
 *  @throws std::bad_alloc if memory allocation fails.  Strong throw guarantee.
 *
 */
template<typename container_type>
std::deque<std::size_t> decimal_to_fns(std::size_t D,
                                       const container_type& perm);

/** @brief A convenience function for bypassing the FNS intermediate.
 *
 * @par Memory
 * This is ultimately a thin wrapper around permutation_to_fns and
 * fns_place_values.  It thus has their combined memory footprints.
 *
 * @tparam container_type The type of the two sequences.
 * @param perm The permutation for which we want the decimal representation
 * @param original The original sequence
 * @return The decimal representation of the permutation.
 * @throws std::bad_alloc if either of the two sub calls run out of memory.
 */
template<typename container_type>
std::size_t permutation_to_decimal(const container_type& perm,
                                   const container_type& original) {
    auto fns    = permutation_to_fns(perm, original);
    auto values = fns_place_values(perm);
    return std::inner_product(fns.begin(), fns.end(), values.begin(), 0);
}

/** @brief A convenience function for going from decimal to the corresponding
 *         permutation.
 *
 * @par Memory
 *
 * Ultimately this calls fns_to_permutation and decimal_to_fns and thus has the
 * same footprints.
 *
 * @tparam container_type The type of the original sequence.
 * @param[in] n The index of the requested permutation
 * @param[in] original The original sequence
 * @returns The requested permutation
 * @throws std::bad_alloc if either of the two subroutines run out of memory
 */
template<typename container_type>
container_type decimal_to_permutation(std::size_t n,
                                      const container_type& original) {
    return fns_to_permutation(decimal_to_fns(n, original), original);
}

////////////////////////////////// Implementations /////////////////////////////

template<typename container_type>
std::size_t n_permutations(const container_type& seq) {
    container_type temp(seq);
    std::vector<std::size_t> counts;
    counts.reserve(seq.size());
    std::sort(temp.begin(), temp.end());
    auto ei = temp.begin();
    while(ei != temp.end()) {
        const auto n_ei = std::count(ei, temp.end(), *ei);
        counts.push_back(n_ei);
        ei += n_ei;
    }
    return multinomial_coefficient<std::size_t>(counts);
}

// Note: Without assuming a container type we have to rely on iterators
template<typename container_type>
std::deque<std::size_t> permutation_to_fns(const container_type& perm,
                                           const container_type& original) {
    // Recursion end
    if(perm.size() == 0) return std::deque<std::size_t>({});
    if(perm.size() == 1) return std::deque<std::size_t>({0});

    // Strip off 1st element and put remaining elements in new container
    auto perm_itr = perm.begin();
    auto ei       = *perm_itr;
    container_type sub_perm(++perm_itr, perm.end());

    // Get digit in FNS and fill new_orig with all elements aside from it
    std::size_t digit = 0;
    container_type new_orig;
    auto orig_itr = original.begin();
    for(std::size_t i = 0; i < original.size(); ++i) {
        auto ej = *orig_itr++;
        if(ej == ei) {
            digit = i;
            while(orig_itr != original.end()) {
                new_orig.insert(new_orig.end(), *orig_itr++);
            }
            break;
        }
        new_orig.insert(new_orig.end(), ej);
    }
    auto rv = permutation_to_fns(sub_perm, new_orig);
    rv.push_front(digit);
    return rv;
}

template<typename container_type>
container_type fns_to_permutation(const std::deque<std::size_t>& fns,
                                  const container_type& original) {
    container_type temp(original);
    container_type rv;
    for(auto x : fns) {
        auto ei_itr = temp.begin();
        container_type temp2;
        for(std::size_t i = 0; i < temp.size(); ++i) {
            if(i != x)
                temp2.insert(temp2.end(), *ei_itr);
            else
                rv.insert(rv.end(), *ei_itr);
            ++ei_itr;
        }
        temp.swap(temp2);
    }
    return rv;
}

template<typename container_type>
std::deque<std::size_t> fns_place_values(const container_type& perm) {
    // Recursion end
    if(perm.size() == 0) return std::deque<std::size_t>({});
    if(perm.size() == 1) return std::deque<std::size_t>({1});
    if(perm.size() == 2) return std::deque<std::size_t>({1, 1});

    // Strip off 1st element and put remaining elements in new container
    auto perm_itr = perm.begin();
    // auto ei=*perm_itr;
    container_type sub_perm(++perm_itr, perm.end());

    auto rv = fns_place_values(sub_perm);
    rv.push_front(n_permutations(perm) / perm.size());
    return rv;
}

template<typename container_type>
std::deque<std::size_t> decimal_to_fns(std::size_t D,
                                       const container_type& orig) {
    container_type new_orig(orig.begin(), orig.end());
    std::sort(new_orig.begin(), new_orig.end());
    std::deque<std::size_t> rv;

    while(new_orig.begin() !=
          new_orig.end()) // We pop an element out per iteration
    {
        auto curr_guess         = new_orig.begin();
        std::size_t total_perms = 0;
        std::size_t digit_i     = 0;
        while(curr_guess != new_orig.end()) {
            auto ei = *curr_guess;
            // New container w/o element i
            container_type temp(new_orig.begin(), curr_guess++);
            temp.insert(temp.end(), curr_guess, new_orig.end());

            const auto P_ei = n_permutations(temp);
            total_perms += P_ei;
            if(D < total_perms) {
                rv.push_back(digit_i);
                new_orig.swap(temp);
                D = D - (total_perms - P_ei);
                break;
            }
            // We know the element isn't ei, but ei may still be in temp...
            auto n_ei = std::count(temp.begin(), temp.end(), ei);
            digit_i += 1 + n_ei;
            std::advance(curr_guess, n_ei);
        }
    }

    return rv;
}

} // namespace utilities
