#pragma once
#include <cstddef>     // For size_t
#include <type_traits> // For is_integral

/** @file IntegerUtils.hpp
 *
 *  This file contains utilities for working with POD types that are integral.
 *  In particular it includes:
 *
 *  - UnsignedSubtract : Subtracts two unsigned values resulting in a signed
 *                       type
 *
 */

namespace utilities {

/**
 * @brief Given two integer type instances this function will take their
 * difference in a manner that will not lead to an overflow and will generate
 * the correct sign on the result.
 *
 * @tparam SignedType The type of the result.  Should be a signed integral POD
 *         type.
 * @tparam UnsignedType The type of the input values.  Should be an unsigned
 *         integral POD type.
 * @param lhs The number of the left of the minus sign.
 * @param rhs The number on the right of the minus sign.
 * @return The signed difference between @p lhs and @p rhs.
 * @throw None. No throw guarantee.
 */
template<typename SignedType = long, typename UnsignedType = std::size_t>
SignedType UnsignedSubtract(const UnsignedType& lhs,
                            const UnsignedType& rhs) noexcept {
    static_assert(std::is_integral<UnsignedType>::value, "Must be integral "
                                                         "type");
    static_assert(std::is_integral<SignedType>::value, "Must be integral type");
    if(lhs > rhs) return static_cast<SignedType>(lhs - rhs);
    SignedType negative1{-1};
    return negative1 * static_cast<SignedType>(rhs - lhs);
}

} // namespace utilities
