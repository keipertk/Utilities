set(
    UTILITIES_HEADERS containers/case_insensitive_map.hpp
                      containers/math_set.hpp
                      iter_tools/cartesian_product.hpp
                      iter_tools/combinations.hpp
                      iter_tools/enumerate.hpp
                      iter_tools/permutations.hpp
                      iter_tools/range.hpp
                      iter_tools/range_container.hpp
                      iter_tools/tuple_container.hpp
                      iter_tools/zip.hpp
                      macros/for_each.hpp
                      mathematician/combinatorics.hpp
                      mathematician/integer_utils.hpp
                      type_traits/iterator_types.hpp
                      type_traits/tuple_utilities.hpp
                      type_traits/type_traits_extensions.hpp
                      iter_tools.hpp
                      smart_enum.hpp
                      static_string.hpp
                      timer.hpp
)

set(UTILITIES_SRCS mathematician/combinatorics.cpp)

cpp_add_library(
    utilities
    SOURCES ${UTILITIES_SRCS}
    INCLUDES ${UTILITIES_HEADERS}
)
cpp_install(TARGETS utilities)

