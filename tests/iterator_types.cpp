#include <utilities/type_traits/iterator_types.hpp>
#include <utilities/type_traits/type_traits_extensions.hpp>
#include <catch2/catch.hpp>

using namespace utilities;
using namespace detail_;

struct Iterator : public InputIteratorBase<Iterator, int> {
    int value_ = 0;

    Iterator& increment() {
        ++value_;
        return *this;
    }

    const int& dereference() const { return value_; }

    bool are_equal(const Iterator& other) const noexcept {
        return value_ == other.value_;
    }
};

struct Iterator2 : public InputIteratorBase<Iterator, const int> {};

TEST_CASE("InputIterator base class") {
    Iterator itr;
    SECTION("Satisfies iterator concept") {
        bool is_itr = is_input_iterator<Iterator>::value;
        REQUIRE(is_itr);
    }
    SECTION("Can be dereferenced in non-const state") {
        int& value = *itr;
        REQUIRE(value == 0);
        REQUIRE(&value == &itr.value_);
    }
    SECTION("Can be dereferenced in a const state") {
        const int& value = *const_cast<const Iterator&>(itr); // NOLINT
        REQUIRE(value == 0);
        REQUIRE(&value == &itr.value_);
    }
    SECTION("Can be prefix-incremented") {
        auto& rv = ++itr;
        REQUIRE(&rv == &itr);
        REQUIRE(*rv == 1);

        SECTION("Equality works") { REQUIRE(rv == itr); }
    }
    SECTION("Can be post-incremented") {
        auto rv = itr++;
        REQUIRE(*rv == 0);
        REQUIRE(&rv.value_ != &itr.value_);
        REQUIRE(*itr == 1);
        SECTION("Inequality works") { REQUIRE(rv != itr); }
    }
    SECTION("Const is obeyed") {
        bool is_const =
          std::is_same<const int&, typename Iterator2::reference>::value;
        REQUIRE(is_const);
    }
}

struct BidirectionalIterator
  : public BidirectionalIteratorBase<BidirectionalIterator, int> {
    int value_ = 0;

    BidirectionalIterator& increment() {
        ++value_;
        return *this;
    }

    BidirectionalIterator& decrement() {
        --value_;
        return *this;
    }

    const int& dereference() const { return value_; }

    bool are_equal(const BidirectionalIterator& other) const noexcept {
        return value_ == other.value_;
    }
};

TEST_CASE("BidirectionalIterator base class") {
    BidirectionalIterator itr;
    SECTION("Satisfies iterator concept") {
        bool is_itr = is_bidirectional_iterator<BidirectionalIterator>::value;
        REQUIRE(is_itr);
    }
    SECTION("Can be prefix decremented") {
        BidirectionalIterator& rv = --itr;
        REQUIRE(*itr == -1);
        REQUIRE(&rv == &itr);
    }
    SECTION("Can be postfix decremented") {
        BidirectionalIterator rv = itr--;
        REQUIRE(&rv.value_ != &itr.value_);
        REQUIRE(*rv == 0);
        REQUIRE(*itr == -1);
    }
}

struct RandomAccessIterator
  : public RandomAccessIteratorBase<RandomAccessIterator, int> {
    int value_ = 0;

    RandomAccessIterator& increment() {
        ++value_;
        return *this;
    }

    RandomAccessIterator& decrement() {
        --value_;
        return *this;
    }

    const int& dereference() const { return value_; }

    bool are_equal(const RandomAccessIterator& other) const noexcept {
        return value_ == other.value_;
    }

    long int distance_to(const RandomAccessIterator& other) const noexcept {
        return other.value_ - value_;
    }

    RandomAccessIterator& advance(long int n) {
        value_ += n;
        return *this;
    }
};

TEST_CASE("RandomAccessIterator base class") {
    RandomAccessIterator itr;
    SECTION("Satisfies iterator concept") {
        bool is_itr = is_random_access_iterator<RandomAccessIterator>::value;
        REQUIRE(is_itr);
    }
    SECTION("Order comparisons work") {
        RandomAccessIterator itr2;
        RandomAccessIterator itr3;
        itr3.value_ = 10;
        REQUIRE(itr < itr3);
        REQUIRE(itr <= itr2);
        REQUIRE(itr3 > itr);
        REQUIRE(itr2 >= itr);
    }
    SECTION("Advancing works") {
        RandomAccessIterator& rv = (itr += 10);
        REQUIRE(&rv == &itr);
        REQUIRE(rv.value_ == 10);
    }
    SECTION("Advance and copy works") {
        RandomAccessIterator rv = (itr + 10);
        REQUIRE(itr.value_ == 0);
        REQUIRE(rv.value_ == 10);
    }
    SECTION("Go backwards works") {
        RandomAccessIterator& rv = (itr -= 10);
        REQUIRE(&rv == &itr);
        REQUIRE(itr.value_ == -10);
    }
    SECTION("Go backwards and copy works") {
        RandomAccessIterator rv = (itr - 10);
        REQUIRE(itr.value_ == 0);
        REQUIRE(rv.value_ == -10);
    }
    SECTION("Random access works") {
        int rv = itr[100];
        REQUIRE(itr.value_ == 0);
        REQUIRE(rv == 100);
    }
}
