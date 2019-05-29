#include <utilities/mathematician/combinatorics.hpp>
#include <catch2/catch.hpp>

using namespace utilities;
using set_type = std::vector<size_t>;
using fns_type = std::deque<size_t>;

TEST_CASE("Binomial coefficient") {
    REQUIRE(binomial_coefficient<size_t>(0, 0) == 1);
    REQUIRE(binomial_coefficient<size_t>(0, 1) == 0);
    REQUIRE(binomial_coefficient<size_t>(4, 0) == 1);
    REQUIRE(binomial_coefficient<size_t>(35, 4) == 52360);
    REQUIRE_THROWS_AS(binomial_coefficient<size_t>(99999, 88),
                      std::overflow_error);
}

TEST_CASE("Multinomial coefficient") {
    REQUIRE(multinomial_coefficient<size_t>(set_type{}) == 1);
    REQUIRE(multinomial_coefficient<size_t>(set_type{5}) == 1);
    REQUIRE(multinomial_coefficient<size_t>(set_type{5, 6, 7}) == 14702688);
    REQUIRE(multinomial_coefficient<size_t>(set_type{4, 31}) == 52360);
}

TEST_CASE("N Permutations") {
    REQUIRE(n_permutations(set_type{}) == 1);
    REQUIRE(n_permutations(set_type{0}) == 1);
    REQUIRE(n_permutations(set_type{0, 1}) == 2);
    REQUIRE(n_permutations(set_type{0, 0}) == 1);
    REQUIRE(n_permutations(set_type{0, 1, 2}) == 6);
    REQUIRE(n_permutations(set_type{0, 1, 1}) == 3);
    REQUIRE(n_permutations(set_type{0, 1, 2, 3}) == 24);
    REQUIRE(n_permutations(set_type{0, 1, 1, 2}) == 12);
    REQUIRE(n_permutations(set_type{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
                                    2, 2, 2}) == 14702688);
}

TEST_CASE("Factorial Number System Free Functions") {
    SECTION("Empty sequence {}") {
        set_type orig;
        fns_type fns;
        REQUIRE(permutation_to_fns(orig, orig) == fns);
        REQUIRE(fns_to_permutation(fns, orig) == orig);
        REQUIRE(fns_place_values(orig) == fns);
        REQUIRE(decimal_to_fns(0, orig) == fns);
        REQUIRE(decimal_to_permutation(0, orig) == orig);
        REQUIRE(permutation_to_decimal(orig, orig) == 0);
    }

    SECTION("{1}") {
        set_type orig({1});
        fns_type fns({0});
        fns_type values({1});
        REQUIRE(permutation_to_fns(orig, orig) == fns);
        REQUIRE(fns_to_permutation(fns, orig) == orig);
        REQUIRE(fns_place_values(orig) == values);
        REQUIRE(decimal_to_fns(0, orig) == fns);
        REQUIRE(decimal_to_permutation(0, orig) == orig);
        REQUIRE(permutation_to_decimal(orig, orig) == 0);
    }

    SECTION("Sequence {1,2,3} (no repeats)") {
        std::vector<set_type> perms(
          {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}});
        std::vector<fns_type> fns_s(
          {{0, 0, 0}, {0, 1, 0}, {1, 0, 0}, {1, 1, 0}, {2, 0, 0}, {2, 1, 0}});

        SECTION("Permutation 2 FNS") {
            for(size_t i = 0; i < 6; ++i) {
                auto corr = fns_s[i];
                auto comp = permutation_to_fns(perms[i], perms[0]);
                REQUIRE(corr == comp);
            }
        }

        SECTION("FNS 2 Permutation") {
            for(size_t i = 0; i < 6; ++i) {
                auto corr = perms[i];
                auto comp = fns_to_permutation(fns_s[i], perms[0]);
                REQUIRE(corr == comp);
            }
        }

        SECTION("FNS place values") {
            fns_type corr({2, 1, 1});
            for(size_t i = 0; i < 6; ++i) {
                auto comp = fns_place_values(perms[i]);
                REQUIRE(corr == comp);
            }
        }

        SECTION("Decimal to FNS") {
            for(size_t i = 0; i < 6; ++i) {
                auto corr = fns_s[i];
                auto comp = decimal_to_fns(i, perms[0]);
                REQUIRE(corr == comp);
            }
        }

        SECTION("Permutation to Decimal") {
            for(size_t i = 0; i < 6; ++i) {
                auto comp = permutation_to_decimal(perms[i], perms[0]);
                REQUIRE(comp == i);
            }
        }
        SECTION("Decimal to Permutation") {
            for(size_t i = 0; i < 6; ++i) {
                auto comp = decimal_to_permutation(i, perms[0]);
                REQUIRE(comp == perms[i]);
            }
        }
    }
    SECTION("Permuation {1,3,3,7} (with repeats)") {
        std::vector<set_type> perms({{1, 3, 3, 7},
                                     {1, 3, 7, 3},
                                     {1, 7, 3, 3},
                                     {3, 1, 3, 7},
                                     {3, 1, 7, 3},
                                     {3, 3, 1, 7},
                                     {3, 3, 7, 1},
                                     {3, 7, 1, 3},
                                     {3, 7, 3, 1},
                                     {7, 1, 3, 3},
                                     {7, 3, 1, 3},
                                     {7, 3, 3, 1}});

        std::vector<fns_type> fns_s({{0, 0, 0, 0},
                                     {0, 0, 1, 0},
                                     {0, 2, 0, 0},
                                     {1, 0, 0, 0},
                                     {1, 0, 1, 0},
                                     {1, 1, 0, 0},
                                     {1, 1, 1, 0},
                                     {1, 2, 0, 0},
                                     {1, 2, 1, 0},
                                     {3, 0, 0, 0},
                                     {3, 1, 0, 0},
                                     {3, 1, 1, 0}});

        SECTION("Permutation to FNS") {
            for(size_t i = 0; i < 12; ++i) {
                auto comp = permutation_to_fns(perms[i], perms[0]);
                auto corr = fns_s[i];
                REQUIRE(comp == corr);
            }
        }

        SECTION("FNS place values") {
            std::vector<fns_type> vals({{3, 1, 1, 1},
                                        {3, 1, 1, 1},
                                        {3, 1, 1, 1},
                                        {3, 2, 1, 1},
                                        {3, 2, 1, 1},
                                        {3, 2, 1, 1},
                                        {3, 2, 1, 1},
                                        {3, 2, 1, 1},
                                        {3, 2, 1, 1},
                                        {3, 1, 1, 1},
                                        {3, 1, 1, 1},
                                        {3, 1, 1, 1}});
            for(size_t i = 0; i < 12; ++i) {
                auto comp = fns_place_values(perms[i]);
                auto corr = vals[i];
                REQUIRE(comp == corr);
            }
        }

        SECTION("Decimal to FNS") {
            for(size_t i = 0; i < 12; ++i) {
                auto comp = decimal_to_fns(i, perms[0]);
                auto corr = fns_s[i];
                REQUIRE(comp == corr);
            }
        }
        SECTION("Decimal to permutation") {
            for(size_t i = 0; i < 12; ++i) {
                auto comp = decimal_to_permutation(i, perms[0]);
                auto corr = perms[i];
                REQUIRE(comp == corr);
            }
        }
        SECTION("Permutation to decimal") {
            for(size_t i = 0; i < 12; ++i) {
                auto comp = permutation_to_decimal(perms[i], perms[0]);
                auto corr = i;
                REQUIRE(comp == corr);
            }
        }
    }
}
