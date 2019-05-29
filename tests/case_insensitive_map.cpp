#include <utilities/containers/case_insensitive_map.hpp>
#include <catch2/catch.hpp>

using namespace utilities;

// Note that the actual CaseInsensitiveMap is just a typedef so this test
// focuses on the less than comparer which actually bestows the case
// insensitive property.

TEST_CASE("CaseInsensitiveLess_::LetterComparer_") {
    detail_::CaseInsensitiveLess_::LetterComparer_ fxn;

    SECTION("Both lowercase") {
        REQUIRE(fxn('a', 'b'));
        REQUIRE(!fxn('b', 'a'));
        REQUIRE(!fxn('a', 'a'));
    }

    SECTION("Left uppercase, right lower") {
        REQUIRE(fxn('A', 'b'));
        REQUIRE(!fxn('B', 'a'));
        REQUIRE(!fxn('A', 'a'));
    }

    SECTION("Left lowercase, right upper") {
        REQUIRE(fxn('a', 'B'));
        REQUIRE(!fxn('b', 'A'));
        REQUIRE(!fxn('a', 'A'));
    }

    SECTION("Both uppercase") {
        REQUIRE(fxn('A', 'B'));
        REQUIRE(!fxn('B', 'A'));
        REQUIRE(!fxn('A', 'A'));
    }
}

TEST_CASE("CaseInsensitiveLess_") {
    detail_::CaseInsensitiveLess_ fxn;
    SECTION("Both lowercase") {
        REQUIRE(fxn("abcde", "bcdef"));
        REQUIRE(!fxn("bcdef", "abcde"));
        REQUIRE(!fxn("abcde", "abcde"));
    }

    SECTION("Left uppercase, right lower") {
        REQUIRE(fxn("ABCDE", "bcdef"));
        REQUIRE(!fxn("BCDEF", "abcde"));
        REQUIRE(!fxn("ABCDE", "abcde"));
    }

    SECTION("Left lowercase, right upper") {
        REQUIRE(fxn("abcde", "BCDEF"));
        REQUIRE(!fxn("bcdef", "ABCDE"));
        REQUIRE(!fxn("abcde", "ABCDE"));
    }

    SECTION("Both uppercase") {
        REQUIRE(fxn("ABCDE", "BCDEF"));
        REQUIRE(!fxn("BCDEF", "ABCDE"));
        REQUIRE(!fxn("ABCDE", "ABCDE"));
    }

    SECTION("Mixed case") {
        REQUIRE(fxn("aB", "bC"));
        REQUIRE(fxn("aB", "Bc"));
        REQUIRE(fxn("Ab", "bC"));
        REQUIRE(fxn("Ab", "Cb"));
    }
}

TEST_CASE("CaseInsensitiveMap") {
    CaseInsensitiveMap<int> a_map;

    SECTION("Before add") {
        REQUIRE(!a_map.count("abc"));
        REQUIRE(!a_map.count("Abc"));
        REQUIRE(!a_map.count("ABc"));
        REQUIRE(!a_map.count("AbC"));
        REQUIRE(!a_map.count("aBC"));
        REQUIRE(!a_map.count("ABC"));
    }

    a_map["abc"] = 2;

    SECTION("After add") {
        REQUIRE(a_map.count("abc"));
        REQUIRE(a_map.count("Abc"));
        REQUIRE(a_map.count("ABc"));
        REQUIRE(a_map.count("AbC"));
        REQUIRE(a_map.count("aBC"));
        REQUIRE(a_map.count("ABC"));
    }
}
