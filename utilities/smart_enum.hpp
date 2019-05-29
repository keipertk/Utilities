#pragma once
#include "utilities/macros/for_each.hpp"
#include "utilities/static_string.hpp"
#include <set>

namespace utilities {

/** @brief A class to implement smart enumerations, based on strings
 *
 *  I stole the basic design of this class from
 *  [here](http://www.drdobbs.com/when-enum-just-isnt-enough-enumeration-c
 *  /184403955).  That said the actual implementation is a bit different in that
 *  I am not concerned with being able to iterate over all the possibilities.
 *  This means I can dodge the weird static `std::set` instance.  It also
 *  means I can get the entire class to be a compile-time literal.
 */
template<class T>
class SmartEnum {
private:
    /// The type of this class, for convenience
    using my_type = SmartEnum<T>;

public:
    /// Intel won't let this be private
    template<std::size_t N>
    constexpr explicit SmartEnum(const char (&str)[N]) : value_(str) {}

    /**
     * @brief Compares two SmartEnums for equality.
     *
     * All SmartEnum comparison operators defer to the underlying StaticString
     * instance and thus this class uses the same definition of equality.
     *
     * @param rhs The SmartEnum to compare to.
     *
     * @return True if the two SmartEnum instances are the same and false
     * otherwise.
     *
     * @throw None. No throw guarantee.
     *
     * @par Complexity:
     * Worst case linear in the length of the underlying string
     *
     * @par Data Races:
     * None.  All SmartEnums are read-only at run-time.
     */
    constexpr bool operator==(const my_type& rhs) const noexcept {
        return value_ == rhs.value_;
    }

    /**
     * @brief Compares two SmartEnums for inequality.
     *
     * All SmartEnum comparison operators defer to the underlying StaticString
     * instance and thus this class uses the same definition of inequality.
     *
     * @param rhs The SmartEnum to compare to.
     *
     * @return True if the two SmartEnum instances are different and false
     * otherwise.
     *
     * @throw None. No throw guarantee.
     *
     * @par Complexity:
     * Worst case linear in the length of the underlying string
     *
     * @par Data Races:
     * None.  All SmartEnums are read-only at run-time.
     */
    constexpr bool operator!=(const my_type& rhs) const noexcept {
        return value_ != rhs.value_;
    }

    /**
     * @brief Determines if the current SmartEnum is less than another instance.
     *
     * All SmartEnum comparison operators defer to the underlying StaticString
     * instance and thus this class uses the same definition of less than.
     *
     * @param rhs The SmartEnum to compare to.
     *
     * @return True if the current SmartEnum instance is less than and
     *         false otherwise.
     *
     * @throw None. No throw guarantee.
     *
     * @par Complexity:
     * Worst case linear in the length of the underlying string
     *
     * @par Data Races:
     * None.  All SmartEnums are read-only at run-time.
     */
    constexpr bool operator<(const my_type& rhs) const noexcept {
        return value_ < rhs.value_;
    }

    /**
     * @brief Determines if the current SmartEnum is less than or equal to
     *        another instance.
     *
     * All SmartEnum comparison operators defer to the underlying StaticString
     * instance and thus this class uses the same definition of less than or
     * equal to.
     *
     * @param rhs The SmartEnum to compare to.
     *
     * @return True if the current SmartEnum instance is less than or equal to
     *         @p rhs and false otherwise.
     *
     * @throw None. No throw guarantee.
     *
     * @par Complexity:
     * Worst case linear in the length of the underlying string
     *
     * @par Data Races:
     * None.  All SmartEnums are read-only at run-time.
     */
    constexpr bool operator<=(const my_type& rhs) const noexcept {
        return value_ <= rhs.value_;
    }

    /**
     * @brief Determines if the current SmartEnum is greater than another
     *        instance.
     *
     * All SmartEnum comparison operators defer to the underlying StaticString
     * instance and thus this class uses the same definition of greater than.
     *
     * @param rhs The SmartEnum to compare to.
     *
     * @return True if the current SmartEnum instance is greater than and
     *         false otherwise.
     *
     * @throw None. No throw guarantee.
     *
     * @par Complexity:
     * Worst case linear in the length of the underlying string
     *
     * @par Data Races:
     * None.  All SmartEnums are read-only at run-time.
     */
    constexpr bool operator>(const my_type& rhs) const noexcept {
        return value_ > rhs.value_;
    }

    /**
     * @brief Determines if the current SmartEnum is greater than or equal to
     *        another instance.
     *
     * All SmartEnum comparison operators defer to the underlying StaticString
     * instance and thus this class uses the same definition of greater than or
     * equal to.
     *
     * @param rhs The SmartEnum to compare to.
     *
     * @return True if the current SmartEnum instance is greater than or equal
     *         to @p rhs, and false otherwise.
     *
     * @throw None. No throw guarantee.
     *
     * @par Complexity:
     * Worst case linear in the length of the underlying string
     *
     * @par Data Races:
     * None.  All SmartEnums are read-only at run-time.
     */
    constexpr bool operator>=(const my_type& rhs) const noexcept {
        return value_ >= rhs.value_;
    }

    /**
     * @brief Allows the enum to be implicitly converted to a C-style string.
     *
     *
     * @return The current enum as a C-style string.
     * @throw None. No throw guarantee.
     * @par Complexity:
     * Constant.
     * @par Data Races:
     * None. All SmartEnums are read-only at run-time.
     */
    constexpr operator const char*() const noexcept { return value_; }

private:
    /// The value this particular enum holds
    StaticString value_;

protected:
    /// Constructor, protected cause you won't ever call it outside derived
    /// class
};

} // namespace utilities

/// Implementation detail for declaring the enum instances in DECLARE_SmartEnum
#define _ADD_SMARTENUM(name) static const T name;

/// Implementation detail for defining the enum instances in DECLARE_SmartEnum
#define _DEFINE_SMARTENUM(enum_name, name) \
    template<typename T>                   \
    constexpr T enum_name##Impl<T>::name(#name);

/**
 * @brief Convenience function for declaring a SmartEnum set.
 *
 * This macro hides the boilerplate for declaring a new set of SmartEnum
 * instances.  It relies on the CALL_MACRO_X_FOR_EACH macro which has a hard
 * coded limit of the maximum number of variable arguments it can handle (this
 * is an artifact of the fact that C macros can't be called recursively).
 *
 * Usage to make a set of SmartEnum "Fruits" which contains apple, pear, and
 * orange:
 * @code
 * DECLARE_SmartEnum(Fruits, apple, pear, orange);
 * @endcode
 *
 * @param[in] enum_name This will become the name of the class which holds your
 *            enum (Fruits in the example above)
 * @param[in] ... These are the values of your enum. (apple, pear, orange in the
 *            example above).
 *
 * Notes on the actual implementation:
 * - We actually create a templated trampoline class enum_nameImpl which allows
 *   us to define the static member variables in a header without fear of
 *   multiple definition reprisal because templates are immune to the ODR.
 *
 *
 */
#define DECLARE_SmartEnum(enum_name, ...)                             \
    template<typename T>                                              \
    class enum_name##Impl : public utilities::SmartEnum<T> {          \
    public:                                                           \
        using utilities::SmartEnum<T>::SmartEnum;                     \
                                                                      \
    public:                                                           \
        CALL_MACRO_X_FOR_EACH(_ADD_SMARTENUM, __VA_ARGS__)            \
    };                                                                \
    CALL_MACRO_X_FOR_EACH1(_DEFINE_SMARTENUM, enum_name, __VA_ARGS__) \
    class enum_name : public enum_name##Impl<enum_name> {             \
    public:                                                           \
        using enum_name##Impl<enum_name>::enum_name##Impl;            \
    }
