#pragma once

/** @file ForEach.hpp
 *
 * This header file is auto-generated by MakeForEach.py
 * To allow more looping regenerate this file with a higher
 * value of "depth".
 *
 *  Includes C-Preprocessor macros for iterating over a list
 *  of arguments and applying a macro to them.  The contents of
 *  this header are borrowed from StackOverflow.
 *  Of the various macros in here only the following are
 *  intended for use:
 *  - CALL_MACRO_X_FOR_EACH : Calls a macro for each argument
 *    passed to the list
 *  - CALL_MACRO_X_FOR_EACH1: Calls a macro with one bound
 *    argument for each argument in the list
 */
/**
 * @brief Returns the N-th argument of a list.
 *
 * By padding what's to the left and what's to the right of
 * element N, this macro can be made to arbitrarily select an
 * element from a list.  To select say the third argument of a
 * list you simply put M-3 placeholders in front of it where M
 * is the maximum number written out here (25).
 */
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, \
                     _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24,  \
                     _25, N, ...)                                            \
    N

/// Guts of the CALL_MACRO_X_FOR_EACH0 macro
///{
#define _fe0_0(_call, _0, ...)
#define _fe0_1(_call, _0, ...) _call(_0)
#define _fe0_2(_call, _0, ...) _call(_0) _fe0_1(_call, __VA_ARGS__)
#define _fe0_3(_call, _0, ...) _call(_0) _fe0_2(_call, __VA_ARGS__)
#define _fe0_4(_call, _0, ...) _call(_0) _fe0_3(_call, __VA_ARGS__)
#define _fe0_5(_call, _0, ...) _call(_0) _fe0_4(_call, __VA_ARGS__)
#define _fe0_6(_call, _0, ...) _call(_0) _fe0_5(_call, __VA_ARGS__)
#define _fe0_7(_call, _0, ...) _call(_0) _fe0_6(_call, __VA_ARGS__)
#define _fe0_8(_call, _0, ...) _call(_0) _fe0_7(_call, __VA_ARGS__)
#define _fe0_9(_call, _0, ...) _call(_0) _fe0_8(_call, __VA_ARGS__)
#define _fe0_10(_call, _0, ...) _call(_0) _fe0_9(_call, __VA_ARGS__)
#define _fe0_11(_call, _0, ...) _call(_0) _fe0_10(_call, __VA_ARGS__)
#define _fe0_12(_call, _0, ...) _call(_0) _fe0_11(_call, __VA_ARGS__)
#define _fe0_13(_call, _0, ...) _call(_0) _fe0_12(_call, __VA_ARGS__)
#define _fe0_14(_call, _0, ...) _call(_0) _fe0_13(_call, __VA_ARGS__)
#define _fe0_15(_call, _0, ...) _call(_0) _fe0_14(_call, __VA_ARGS__)
#define _fe0_16(_call, _0, ...) _call(_0) _fe0_15(_call, __VA_ARGS__)
#define _fe0_17(_call, _0, ...) _call(_0) _fe0_16(_call, __VA_ARGS__)
#define _fe0_18(_call, _0, ...) _call(_0) _fe0_17(_call, __VA_ARGS__)
#define _fe0_19(_call, _0, ...) _call(_0) _fe0_18(_call, __VA_ARGS__)
#define _fe0_20(_call, _0, ...) _call(_0) _fe0_19(_call, __VA_ARGS__)
#define _fe0_21(_call, _0, ...) _call(_0) _fe0_20(_call, __VA_ARGS__)
#define _fe0_22(_call, _0, ...) _call(_0) _fe0_21(_call, __VA_ARGS__)
#define _fe0_23(_call, _0, ...) _call(_0) _fe0_22(_call, __VA_ARGS__)
#define _fe0_24(_call, _0, ...) _call(_0) _fe0_23(_call, __VA_ARGS__)
///}

/// Guts of the CALL_MACRO_X_FOR_EACH1 macro
///{
#define _fe1_0(_call, _0, _1, ...)
#define _fe1_1(_call, _0, _1, ...) _call(_0, _1)
#define _fe1_2(_call, _0, _1, ...) _call(_0, _1) _fe1_1(_call, _0, __VA_ARGS__)
#define _fe1_3(_call, _0, _1, ...) _call(_0, _1) _fe1_2(_call, _0, __VA_ARGS__)
#define _fe1_4(_call, _0, _1, ...) _call(_0, _1) _fe1_3(_call, _0, __VA_ARGS__)
#define _fe1_5(_call, _0, _1, ...) _call(_0, _1) _fe1_4(_call, _0, __VA_ARGS__)
#define _fe1_6(_call, _0, _1, ...) _call(_0, _1) _fe1_5(_call, _0, __VA_ARGS__)
#define _fe1_7(_call, _0, _1, ...) _call(_0, _1) _fe1_6(_call, _0, __VA_ARGS__)
#define _fe1_8(_call, _0, _1, ...) _call(_0, _1) _fe1_7(_call, _0, __VA_ARGS__)
#define _fe1_9(_call, _0, _1, ...) _call(_0, _1) _fe1_8(_call, _0, __VA_ARGS__)
#define _fe1_10(_call, _0, _1, ...) _call(_0, _1) _fe1_9(_call, _0, __VA_ARGS__)
#define _fe1_11(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_10(_call, _0, __VA_ARGS__)
#define _fe1_12(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_11(_call, _0, __VA_ARGS__)
#define _fe1_13(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_12(_call, _0, __VA_ARGS__)
#define _fe1_14(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_13(_call, _0, __VA_ARGS__)
#define _fe1_15(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_14(_call, _0, __VA_ARGS__)
#define _fe1_16(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_15(_call, _0, __VA_ARGS__)
#define _fe1_17(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_16(_call, _0, __VA_ARGS__)
#define _fe1_18(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_17(_call, _0, __VA_ARGS__)
#define _fe1_19(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_18(_call, _0, __VA_ARGS__)
#define _fe1_20(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_19(_call, _0, __VA_ARGS__)
#define _fe1_21(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_20(_call, _0, __VA_ARGS__)
#define _fe1_22(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_21(_call, _0, __VA_ARGS__)
#define _fe1_23(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_22(_call, _0, __VA_ARGS__)
#define _fe1_24(_call, _0, _1, ...) \
    _call(_0, _1) _fe1_23(_call, _0, __VA_ARGS__)
///}

/**
 * @brief Applies a macro to each argument in a list
 *
 * @param[in] x The macro to apply
 * @param[in] ... The list of arguments to apply @p x to
 */
#define CALL_MACRO_X_FOR_EACH(_call, ...)                                     \
                                                                              \
    _GET_NTH_ARG("ignored", __VA_ARGS__, _fe0_24, _fe0_23, _fe0_22, _fe0_21,  \
                 _fe0_20, _fe0_19, _fe0_18, _fe0_17, _fe0_16, _fe0_15,        \
                 _fe0_14, _fe0_13, _fe0_12, _fe0_11, _fe0_10, _fe0_9, _fe0_8, \
                 _fe0_7, _fe0_6, _fe0_5, _fe0_4, _fe0_3, _fe0_2, _fe0_0)      \
    (_call, __VA_ARGS__)
/**
 * @brief Applies a macro to each argument in a list
 *
 * @param[in] x The macro to apply
 * @param[in] _0 The 0-th argument to bind
 * @param[in] ... The list of arguments to apply @p x to
 */
#define CALL_MACRO_X_FOR_EACH1(_call, _0, ...)                                \
                                                                              \
    _GET_NTH_ARG("ignored", __VA_ARGS__, _fe1_24, _fe1_23, _fe1_22, _fe1_21,  \
                 _fe1_20, _fe1_19, _fe1_18, _fe1_17, _fe1_16, _fe1_15,        \
                 _fe1_14, _fe1_13, _fe1_12, _fe1_11, _fe1_10, _fe1_9, _fe1_8, \
                 _fe1_7, _fe1_6, _fe1_5, _fe1_4, _fe1_3, _fe1_2, _fe1_0)      \
    (_call, _0, __VA_ARGS__)
