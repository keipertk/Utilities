#include <utilities/iter_tools/enumerate.hpp>
#include <catch2/catch.hpp>
#include <vector>

using namespace utilities;

TEST_CASE("Enumerate") {
    std::vector<double> test{1.1, 2.2, 3.3};
    std::size_t counter = 0;
    for(auto x : Enumerate(test)) {
        REQUIRE(std::get<0>(x) == counter);
        REQUIRE(std::get<1>(x) == test[counter]);
        ++counter;
    }
}
