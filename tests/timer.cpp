#include <catch2/catch.hpp>
#include <sstream>
#include <thread>
#include <utilities/timer.hpp>

TEST_CASE("Timer::record") {
    std::chrono::milliseconds time2sleep(5);
    utilities::Timer t;
    std::this_thread::sleep_for(time2sleep);
    t.record("sleep 5 ms");
    REQUIRE(t.at("sleep 5 ms") >= time2sleep);
}

TEST_CASE("Timer::reset") {
    std::chrono::milliseconds time2sleep(5);
    utilities::Timer t;
    std::this_thread::sleep_for(time2sleep);
    t.reset();
    t.record("no sleep");
    REQUIRE(t.at("no sleep") < time2sleep);
}

TEST_CASE("Timer::time_it") {
    std::chrono::milliseconds time2sleep(5);
    utilities::Timer t;
    t.time_it("sleep 5 ms", [=]() { std::this_thread::sleep_for(time2sleep); });
    REQUIRE(t.at("sleep 5 ms") >= time2sleep);
}

TEST_CASE("Timer::operator[]") {
    utilities::Timer t;
    REQUIRE_THROWS_AS(t["not a key"], std::out_of_range);
}

TEST_CASE("Printing a timer") {
    std::chrono::milliseconds time2sleep(5);
    utilities::Timer t;
    std::this_thread::sleep_for(time2sleep);
    t.record("sleep 5 ms");
    std::stringstream ss;
    ss << t << std::endl;
    auto found = ss.str().find("sleep 5 ms : 0 h 0 m 0 s 5 ms");
    REQUIRE(found != std::string::npos);
}
