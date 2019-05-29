#include <utilities/iter_tools/range.hpp>
#include <utilities/iter_tools/zip.hpp>
#include <utilities/type_traits/type_traits_extensions.hpp>
#include <catch2/catch.hpp>

using namespace utilities;

template<typename container>
void check_state(container rng, std::initializer_list<int> val_in) {
    std::vector<int> vals(val_in);
    constexpr auto max_i =
      std::numeric_limits<typename container::size_type>::max();
    REQUIRE(rng.size() == vals.size());
    REQUIRE(rng.max_size() == max_i);
    if(vals.size()) {
        REQUIRE(rng.begin() != rng.end());
        REQUIRE(rng.cbegin() != rng.cend());
    } else {
        REQUIRE(rng.begin() == rng.end());
        REQUIRE(rng.cbegin() == rng.cend());
    }

    // Normal iterators
    for(auto& x : Zip(rng, vals)) { REQUIRE(std::get<0>(x) == std::get<1>(x)); }
}

TEST_CASE("Range") {
    SECTION("Is a container") {
        REQUIRE(is_container<detail_::RangeImpl<>>::value);
    }

    SECTION("Range default constructor") {
        auto rng1 = Range(0);
        check_state(rng1, {});
    }

    SECTION("Simple range [0,5)") {
        auto rng1 = Range(5);
        check_state(rng1, {0, 1, 2, 3, 4});
    }

    SECTION("Moderate range [1,6)") {
        auto rng1 = Range(1, 6);
        check_state(rng1, {1, 2, 3, 4, 5});
    }

    SECTION("Hard range [1,7) by 2s") {
        auto rng1 = Range(1, 7, 2);
        check_state(rng1, {1, 3, 5});
    }

    SECTION("Reverse range") {
        auto rng1 = Range(8, 2, -2);
        check_state(rng1, {8, 6, 4});
    }

    SECTION("Can be used in a foreach loop") {
        std::size_t counter = 0;
        for(auto x : Range(5)) {
            REQUIRE(x == counter);
            ++counter;
        }
        REQUIRE(counter == 5);
    }
}
