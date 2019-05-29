#include <utilities/iter_tools/combinations.hpp>
#include <catch2/catch.hpp>

using namespace utilities;
using vector_t = std::vector<int>;

template<bool repeat>
using comb_itr = detail_::CombinationItr<vector_t, repeat>;

std::vector<std::vector<vector_t>> corr_combs{
  {{}},                     // k=0
  {{1}, {2}, {3}},          // k=1
  {{1, 2}, {1, 3}, {2, 3}}, // k=2
  {{1, 2, 3}}               // k=3
};

std::vector<std::vector<vector_t>> corr_combs_wr{
  {{}},                                                                  // k=0
  {{1}, {2}},                                                            // k=1
  {{1, 1}, {1, 2}, {2, 2}},                                              // k=2
  {{1, 1, 1}, {1, 1, 2}, {1, 2, 2}, {2, 2, 2}},                          // k=3
  {{1, 1, 1, 1}, {1, 1, 1, 2}, {1, 1, 2, 2}, {1, 2, 2, 2}, {2, 2, 2, 2}} // k=4
};

template<bool repeat>
void check_state(comb_itr<repeat> start, const comb_itr<repeat>& end,
                 const vector_t& orig, const std::vector<vector_t>& corr) {
    if(corr.size() != 0)
        REQUIRE(start != end);
    else
        REQUIRE(start == end);
    size_t counter = 0;
    while(start != end) {
        for(std::size_t i = 0; i < corr.size(); ++i) {
            comb_itr<repeat> tmp{orig, corr[i].size(), false};
            for(std::size_t j = 0; j < i; ++j) ++tmp;
            const long dx = static_cast<long>(i) - counter;
            REQUIRE(start.distance_to(tmp) == dx);
            comb_itr<repeat> copy{start};
            REQUIRE(copy.advance(dx) == tmp);
        }
        REQUIRE(*start++ == corr[counter++]);
    }
}
TEST_CASE("Combinations") {
    SECTION("Default Ctor w/o Repeat") {
        comb_itr<false> c0;
        check_state(c0, c0, {}, {});
    }

    SECTION("Default Ctor w Repeat") {
        comb_itr<true> c0;
        check_state(c0, c0, {}, {});
    }

    SECTION("Combinations") {
        std::vector<int> s0{1, 2, 3};
        for(size_t k = 0; k < 4; ++k) {
            SECTION("{1, 2, 3} choose " + std::to_string(k)) {
                comb_itr<false> c0(s0, k, false);
                comb_itr<false> c1(s0, k, true);
                check_state<false>(c0, c1, s0, corr_combs[k]);
            }
        }
    }
    SECTION("Ctors w/o Repeat") {
        std::vector<int> s0{1, 2, 3};
        comb_itr<false> c0(s0, 2, false);
        SECTION("Copy Ctor") {
            comb_itr<false> c2{c0};
            REQUIRE(c2 == c0);
        }
        SECTION("Copy assignment") {
            comb_itr<false> c2;
            REQUIRE(c2 != c0);
            c2 = c0;
            REQUIRE(c2 == c0);
        }
        SECTION("Move Ctor") {
            comb_itr<false> c2{c0};
            comb_itr<false> c3{std::move(c0)};
            REQUIRE(c3 == c2);
        }
        SECTION("Move assignment") {
            comb_itr<false> c2;
            REQUIRE(c2 != c0);
            comb_itr<false> c3{c0};
            c2 = std::move(c3);
            REQUIRE(c2 == c0);
        }
    }

    SECTION("Combinations With Repeat") {
        std::vector<int> s0{1, 2};
        for(size_t k = 0; k <= 4; ++k) {
            SECTION("{1, 2} multichoose " + std::to_string(k)) {
                comb_itr<true> c0(s0, k, false);
                comb_itr<true> c1(s0, k, true);
                check_state<true>(c0, c1, s0, corr_combs_wr[k]);
            }
        }
    }

    SECTION("Ctors w/Repeat") {
        std::vector<int> s0{1, 2};
        comb_itr<true> c0(s0, 2, false);
        SECTION("Copy Ctor") {
            comb_itr<true> c2{c0};
            REQUIRE(c2 == c0);
        }
        SECTION("Copy assignment") {
            comb_itr<true> c2;
            REQUIRE(c2 != c0);
            c2 = c0;
            REQUIRE(c2 == c0);
        }
        SECTION("Move Ctor") {
            comb_itr<true> c2{c0};
            comb_itr<true> c3{std::move(c0)};
            REQUIRE(c3 == c2);
        }
        SECTION("Move assignment") {
            comb_itr<true> c2;
            REQUIRE(c2 != c0);
            comb_itr<true> c3{c0};
            c2 = std::move(c3);
            REQUIRE(c2 == c0);
        }
    }
}
