#include <utilities/static_string.hpp>
#include <cassert>
#include <catch2/catch.hpp>
#include <sstream>

// Note we use static_assert to assert that it's working at compile-time and we
// have to use assert in the check_string function because REQUIRE is not
// constexpr

using namespace utilities;

template<std::size_t N>
constexpr bool check_string(const StaticString& str, const char (&corr)[N]) {
    assert(str.size() == N - 1);
    for(std::size_t i = 0; i < N - 1; ++i) assert(str[i] == corr[i]);
    StaticString temp_corr(corr);
    assert(str == temp_corr);
    StaticString temp_not_corr("No strings use this value");
    assert(str != temp_not_corr);
    constexpr StaticString very_late_in_alphabet("zzzzzzzzzzzzzzzzzzzzzzzzzzz");
    assert(str < very_late_in_alphabet);
    assert(str <= str);
    assert(str <= very_late_in_alphabet);
    assert(very_late_in_alphabet > str);
    assert(str >= str);
    assert(very_late_in_alphabet >= str);
    constexpr StaticString null("");
    assert(str >= null);
    assert(null <= str);
    if(N != 1) {
        assert(str > null);
        assert(null < str);
    }
    return true;
}

TEST_CASE("Null StaticString") {
    constexpr StaticString null{""};
    static_assert(check_string(null, ""));
    REQUIRE(check_string(null, ""));
}

TEST_CASE("No Spaces StaticString") {
    constexpr StaticString hello{"hello"};
    static_assert(check_string(hello, "hello"));
    REQUIRE(check_string(hello, "hello"));
}

TEST_CASE("Spaces StaticString") {
    constexpr StaticString hello_world{"hello world"};
    static_assert(check_string(hello_world, "hello world"));
    REQUIRE(check_string(hello_world, "hello world"));
    std::stringstream ss_corr;
    ss_corr << "hello world";
    std::stringstream ss;
    ss << hello_world;
    REQUIRE(ss.str() == ss_corr.str());
}
