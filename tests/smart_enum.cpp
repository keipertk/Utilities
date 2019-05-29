#include <utilities/smart_enum.hpp>
#include <catch2/catch.hpp>
#include <sstream>

using namespace utilities;

DECLARE_SmartEnum(Fruit, apple, pear, banana, grape);

/* I'm not sure if it's a compiler bug with Intel, but it won't let me use
 * the enums as constexpr despite them being initialized as such...
 */

TEST_CASE("SmartEnum") {
    /* Intel no like-y
    constexpr Fruit fuji = Fruit::apple;
    constexpr Fruit gala = Fruit::apple;
    constexpr Fruit bosc = Fruit::pear;
    static_assert(fuji == gala);
    static_assert(fuji != bosc);
    static_assert(fuji < bosc);
    static_assert(fuji <= gala);
    static_assert(bosc > fuji);
    static_assert(fuji >= gala);
    */

    Fruit fuji = Fruit::apple;
    Fruit gala = Fruit::apple;
    Fruit bosc = Fruit::pear;

    REQUIRE(fuji == gala);
    REQUIRE(fuji != bosc);

    // This uses the fact that enums are sorted alphabetically by the instance

    REQUIRE(fuji < bosc);
    REQUIRE(fuji <= gala);
    REQUIRE(bosc > fuji);
    REQUIRE(fuji >= gala);

    std::stringstream ss;
    ss << fuji;

    std::stringstream corr_ss;
    corr_ss << "apple";

    REQUIRE(ss.str() == corr_ss.str());
}
