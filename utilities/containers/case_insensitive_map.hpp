#pragma once
#include <algorithm> // For lexicographical_compare
#include <cctype>    // For tolower
#include <map>
#include <string>

namespace utilities {
namespace detail_ {

/**
 * @brief Implements a case insensitive less-than operation for two
 * std::strings.
 *
 * @note This is shamelessly stolen from StackOverflow's topic:
 * "Making map find operation case insensitive"
 */
struct CaseInsensitiveLess_ {
    /// Letter by letter case-insensitive functor
    struct LetterComparer_ {
        /**
         * @brief Compares two characters in a case-insensitive way.
         * @param c1 The first
         * @param c2
         * @return True if lowercase @p c1 is less than lowercase @p c2 and
         * false otherwise
         * @throw ??? Throws if the locale backend throws (I am unable to
         * find a list of what could cause that).  Strong throw guarantee.
         */
        bool operator()(const unsigned char& c1,
                        const unsigned char& c2) const {
            return std::tolower(c1) < std::tolower(c2);
        }
    };

    /**
     * @brief Compares two strings in a case insensitive manner.
     *
     * Ultimately this will use LetterComparer_ to compare the strings
     * lexicographically letter by letter.
     * @param s1 The first string
     * @param s2 The second string
     * @return True if lowercase s1 comes before lowercase s2.  False otherwise.
     * @throw ??? Throws if LetterComparer_'s operator() throws.  Strong throw
     * guarantee.
     */
    bool operator()(const std::string& s1, const std::string& s2) const {
        return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(),
                                            s2.end(), LetterComparer_());
    }
};

} // namespace detail_

/**
 * @brief A case-insensitive std::map (the keys are assumed to be
 * std::string otherwise it doesn't make a whole lot of sense to do a
 * case-insensitive compare...)
 *
 * This class is really just a partial specialization of std::map so it's
 * API is simply that of std::map.
 *
 * @tparam T the type of values you are putting into the map
 */
template<typename T>
using CaseInsensitiveMap =
  std::map<std::string, T, detail_::CaseInsensitiveLess_>;

} // namespace utilities
