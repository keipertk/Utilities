#pragma once
#include "utilities/iter_tools/range.hpp"
#include "utilities/iter_tools/zip.hpp"

namespace utilities {

template<typename container_type>
auto Enumerate(container_type&& c) {
    auto r = Range(c.size());
    return Zip(r, std::forward<container_type>(c));
}

} // namespace utilities
