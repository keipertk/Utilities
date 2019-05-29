#include <utilities/mathematician/integer_utils.hpp>
#include <catch2/catch.hpp>

using namespace utilities;

TEST_CASE("UnsignedSubtract") {
    SECTION("Default types") {
        std::size_t n1{3};
        std::size_t n2{2};
        long n1mn2{1};
        long n2mn1{-1};
        REQUIRE(UnsignedSubtract(n1, n2) == n1mn2);
        REQUIRE(UnsignedSubtract(n2, n1) == n2mn1);
    }
    SECTION("Shorten return") {
        std::size_t n1{3};
        std::size_t n2{2};
        int n1mn2{1};
        int n2mn1{-1};
        REQUIRE(UnsignedSubtract<int>(n1, n2) == n1mn2);
        REQUIRE(UnsignedSubtract<int>(n2, n1) == n2mn1);
    }
    SECTION("Shorten input") {
        unsigned n1{3};
        unsigned n2{2};
        long n1mn2{1};
        long n2mn1{-1};
        REQUIRE(UnsignedSubtract(n1, n2) == n1mn2);
        REQUIRE(UnsignedSubtract(n2, n1) == n2mn1);
    }
    SECTION("Shorten both") {
        unsigned n1{3};
        unsigned n2{2};
        int n1mn2{1};
        int n2mn1{-1};
        REQUIRE(UnsignedSubtract<int>(n1, n2) == n1mn2);
        REQUIRE(UnsignedSubtract<int>(n2, n1) == n2mn1);
    }
}
