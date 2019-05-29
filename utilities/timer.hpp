#pragma once
#include "utilities/containers/case_insensitive_map.hpp"
#include <chrono>
#include <iostream>
#include <string>

namespace utilities {

/** @brief Facilitates timing one or more functions.
 *
 *  The primary purpose of this class is to time one or more functions. Timing
 *  of a single function is done by:
 *  ```
 *  Timer t;
 *  t.time_it("Description of function", fxn_to_time, arg1, arg2, ...);
 *  ```
 *
 *  Alternatively you can do:
 *
 *  ```
 *  Timer t;
 *  fxn_to_time(arg1, arg2, ...);
 *  t.record("Description of function");
 *  ```
 *  The latter syntax facilitates timing multiple functions together (simply
 *  hold off on calling `record` until you have run all the functions you want
 *  to time).
 *
 *  The timer keeps track of each of the timings that it has run and stores them
 *  in a map-like manner where the description can be used to retrieve the time
 *  a function took.
 */
class Timer {
private:
    /// The type of clock we use internally
    using clock_type = std::chrono::high_resolution_clock;
    /// The type the clock uses to record time
    using time_point = typename clock_type::time_point;

public:
    /// The type of a descriptions
    using key_type = std::string;
    /// The duration that passed between time points, the value_type
    using duration = typename clock_type::duration;

    /** @brief Creates and starts a timer.
     *
     *  In the spirit of RAII, this class immediately starts a timer upon
     *  construction. Hence the user should create a timer before the first
     *  function they want to time.
     *
     *  @throw none No throw guarantee.
     *
     */
    Timer() noexcept : m_started_(clock_type::now()) {}

    /** @brief Called to manually note the time that has passed since the timer
     *         started counting.
     *
     *  This function is intended for use when the user wants to time multiple
     *  functions as one, or it is inconvenient to pass the function to
     *  `time_it`.
     *
     *  @note This call will reset the timer so that it is ready for the next
     *        timing.
     *
     *  @param[in] desc A description of what was just timed.
     *  @throw std::bad_alloc if there is insufficient memory to record the
     *                        timing. Strong throw guarantee.
     */
    void record(key_type desc);

    /** @brief Times the provided call back
     *
     *  This function will time the provided call back and save the result
     *  internally.
     *
     *  @note This function will reset the timer after the call.
     *
     * @tparam Fxn The type of the call back
     * @tparam Args The types of the arguments that will be forwarded to the
     *              callback
     * @param[in] desc A description of the callback that is being timed.
     * @param[in] fxn The actual callback to run
     * @param[in] args The arguments to forward to the callback.
     *
     * @throw ??? If the callback throws. Strong throw guarantee.
     * @throw std::bac_alloc if there is insufficient memory to record the
     *                       timing. Strong throw guarantee.
     */
    template<typename Fxn, typename... Args>
    void time_it(key_type desc, Fxn&& fxn, Args&&... args);

    /** @brief Resets how long the timer has been running for.
     *
     *  This function restarts the timer. All recorded times are still
     *  preserved. This function is primarily used internally, but is of use to
     *  the user if the user has run functions since the last timing, and the
     *  timing of those functions is not of interest.
     *
     *  @throw none No throw guarantee.
     */
    void reset() noexcept { m_started_ = clock_type::now(); }

    /** @name Duration accessors
     *
     *   @brief Allows the user to manually retrieve the duration of a timed
     *         function.
     *
     *  This function can be used to retrieve how long a particular timed
     *  section ran for. The result is read-only.
     *
     * @param[in] desc The timing the user is interested in.
     * @return How the long the requested operation ran for as an object of type
     *         @p duration.
     * @throw std::out_of_range if @p desc is not a valid key.
     */
    ///@{
    auto& at(const key_type& desc) const { return m_times_.at(desc); }
    auto& operator[](const key_type& desc) const { return at(desc); }
    ///@}

    /** @name Iterators
     *
     *  @note All iterators are only good so long as the Timer instance that
     *        created them is.
     *
     *  @return An iterator pointing at the first timing result (for begin and
     *          cbegin) or an iterator just past the last timing result (for
     *          end and cend).
     *
     *  @throw none All iterator creation methods are no throw gurantee.
     */
    ///@{
    auto begin() const noexcept { return m_times_.begin(); }
    auto end() const noexcept { return m_times_.end(); }
    auto cbegin() const noexcept { return begin(); }
    auto cend() const noexcept { return end(); }
    ///@}
private:
    /// code factorization for recording a timing
    void record_(key_type desc, time_point t1, time_point t2);

    /// When we started timing
    time_point m_started_;

    /// The timings that this instance has recorded
    utilities::CaseInsensitiveMap<duration> m_times_;
};

/** @brief Wraps printing a Timer instance out
 *
 *  @relates Timer
 *
 *  This will print out all timings contained within a particular Timer
 *  instance. Each timing will be of the format:
 *  ```
 *  <desc> : <x> h <y> m <z> s <w> ms
 *  ```
 *  where `desc` is the description the user provided, `x` is the number of
 *  hours that the function took, `y` is the number of minutes, `z` is the
 *  number of seconds, and `w` is the number of milliseconds. The total time is
 *  the sum of `x`, `y`, `z`, and `w`.
 *
 *  @param[in] os The std::ostream object to print @p t to.
 *  @param[in] t The timer instance to print out.
 *  @return @p os containing the string representation of @p t.
 *  @throw ??? Throws if os::operator<< throws. Weak throw guarantee (@p os
 *             will be in a valid, but otherwise undefined state).
 */
inline std::ostream& operator<<(std::ostream& os, const Timer& t);

//------------------------------Implementations--------------------------------

inline void Timer::record(std::string desc) {
    /// Capture the time 1st in order to be slightly more accurate
    auto t2 = clock_type::now();
    record_(std::move(desc), m_started_, t2);
}

template<typename Fxn, typename... Args>
void Timer::time_it(std::string desc, Fxn&& fxn, Args&&... args) {
    auto t1 = clock_type::now();
    fxn(std::forward<Args>(args)...);
    auto t2 = clock_type::now();
    record_(std::move(desc), t1, t2);
}

inline void Timer::record_(std::string desc, time_point t1, time_point t2) {
    m_times_.emplace(std::move(desc), t2 - t1);
    reset();
}

inline std::ostream& operator<<(std::ostream& os, const Timer& t) {
    for(const auto& [desc, dt] : t) {
        using namespace std::chrono;
        auto h  = duration_cast<hours>(dt);
        auto m  = duration_cast<minutes>(dt) - h;
        auto s  = duration_cast<seconds>(dt) - h - m;
        auto ms = duration_cast<milliseconds>(dt) - h - m - s;
        os << desc << " : " << h.count() << " h " << m.count() << " m "
           << s.count() << " s " << ms.count() << " ms" << std::endl;
    }
    return os;
}

} // namespace utilities
