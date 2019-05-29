#include <utilities/iter_tools/zip.hpp>
#include <utilities/type_traits/type_traits_extensions.hpp>
#include <array>
#include <catch2/catch.hpp>

using namespace utilities;

template<typename T>
void check_state(T& zipper, std::size_t size) {
    REQUIRE(zipper.size() == size);
    const bool is_empty = (size == 0);
    REQUIRE(zipper.empty() == is_empty);
    if(is_empty)
        REQUIRE(zipper.begin() == zipper.end());
    else
        REQUIRE(zipper.begin() != zipper.end());
}

TEST_CASE("Zip") {
    SECTION("Empty zip") {
        auto test_container = Zip();
        check_state(test_container, 0);
    }

    SECTION("Single container") {
        std::vector<int> numbers{1, 2};
        auto test_container = Zip(numbers);
        check_state(test_container, 2);

        auto itr = test_container.begin();
        auto end = test_container.end();

        SECTION("Test iteration") {
            REQUIRE(std::get<0>(*itr) == 1);
            auto& pitr = (++itr);
            REQUIRE(&pitr == &itr);
            REQUIRE(itr != end);
            REQUIRE(std::get<0>(*itr) == 2);
            ++itr;
            REQUIRE(itr == end);
        }
    }

    SECTION("Two same size arrays") {
        std::vector<int> numbers({1, 2, 3});
        std::array<char, 3> letters({'a', 'b', 'c'});
        auto test_container = Zip(numbers, letters);
        check_state(test_container, 3);

        SECTION("Foreach loop") {
            std::size_t counter = 0;
            for(auto& x : test_container) {
                SECTION("Iteration test") {
                    REQUIRE(std::get<0>(x) == numbers[counter]);
                    REQUIRE(std::get<1>(x) == letters.at(counter));
                }
                ++counter;
            }
        }
    }

    SECTION("Different size arrays") {
        std::vector<int> numbers({1, 2, 3});
        std::vector<char> letters({'a', 'b'});
        auto test_container = Zip(numbers, letters);
        check_state(test_container, 2);
        std::size_t counter = 0;
        for(auto& x : test_container) {
            SECTION("Iteration test") {
                REQUIRE(std::get<0>(x) == numbers[counter]);
                REQUIRE(std::get<1>(x) == letters[counter]);
            }
            ++counter;
        }
    }

    SECTION("Different size arrays (one empty)") {
        std::vector<int> numbers({1, 2, 3});
        std::vector<char> letters;
        auto test_container = Zip(numbers, letters);
        check_state(test_container, 0);
    }

    SECTION("Const containers") {
        const std::vector<int> numbers{1, 2, 3};
        const std::vector<char> letters{'a', 'b', 'c'};
        auto test_container = Zip(numbers, letters);
        check_state(test_container, 3);
    }

    // SECTION("References to elements"){
    //    const std::vector<int> numbers{1, 2, 3};
    //    const std::vector<char> letters{'a', 'b', 'c'};
    //    std::size_t counter = 0;
    //    for(auto& i : Zip(numbers, letters)){
    //        REQUIRE(&(std::get<0>(i)) == &(numbers[counter]));
    //        REQUIRE(&(std::get<1>(i)) == &(letters[counter]));
    //    }
    //}
}
