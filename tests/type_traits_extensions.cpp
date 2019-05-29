#include <utilities/type_traits/type_traits_extensions.hpp>
#include <catch2/catch.hpp>
#include <vector>

using namespace utilities;

struct Struct1 {
    void begin();
    void swap(Struct1&);
    bool operator!=(const Struct1&);
    int operator*();
    int operator++(int);
    int operator[](int);
    int operator+=(int);
    int operator-=(int);
    int operator<=(int);
    int operator>=(int);
};
struct Struct2 {
    using value_type = int;
    bool operator==(const Struct2&);
    int operator++();
    Struct2* operator->();
    int operator--();
    int operator--(int);
    int operator+(int);
    int operator-(int);
    int operator<(int);
    int operator>(int);
};

TEST_CASE("type_traits library extensions") {
    REQUIRE(Negation<std::false_type>::value);

    SECTION("Has types check") {
        const bool has_type_false = has_type_value_type<Struct1>::value;
        const bool has_type_true  = has_type_value_type<Struct2>::value;
        REQUIRE(!has_type_false);
        REQUIRE(has_type_true);
    }

    SECTION("Has function checks") {
        const bool has_fxn_false = has_begin<Struct2>::value;
        const bool has_fxn_true  = has_begin<Struct1>::value;
        REQUIRE(!has_fxn_false);
        REQUIRE(has_fxn_true);
    }

    // Operators are a bit tricky so actually check each one
    SECTION("Has operation checks") {
        const bool equal_to_false = has_equal_to<Struct2, Struct1>::value;
        const bool equal_to_true  = has_equal_to<Struct2, Struct2>::value;
        REQUIRE(!equal_to_false);
        REQUIRE(equal_to_true);

        bool not_equal_to_false = has_not_equal_to<Struct2, Struct1>::value;
        bool not_equal_to_true  = has_not_equal_to<Struct1, Struct1>::value;
        REQUIRE(!not_equal_to_false);
        REQUIRE(not_equal_to_true);

        bool dereference_false = has_dereference<Struct2>::value;
        bool dereference_true  = has_dereference<Struct1>::value;
        REQUIRE(!dereference_false);
        REQUIRE(dereference_true);

        bool arrow_false = has_arrow<Struct1>::value;
        bool arrow_true  = has_arrow<Struct2>::value;
        REQUIRE(!arrow_false);
        REQUIRE(arrow_true);

        bool prefix_inc_false = has_prefix_increment<Struct1>::value;
        bool prefix_inc_true  = has_prefix_increment<Struct2>::value;
        REQUIRE(!prefix_inc_false);
        REQUIRE(prefix_inc_true);

        bool postfix_inc_false = has_postfix_increment<Struct2>::value;
        bool postfix_inc_true  = has_postfix_increment<Struct1>::value;
        REQUIRE(!postfix_inc_false);
        REQUIRE(postfix_inc_true);

        bool prefix_dec_false = has_prefix_decrement<Struct1>::value;
        bool prefix_dec_true  = has_prefix_decrement<Struct2>::value;
        REQUIRE(!prefix_dec_false);
        REQUIRE(prefix_dec_true);

        bool postfix_dec_false = has_postfix_decrement<Struct1>::value;
        bool postfix_dec_true  = has_postfix_decrement<Struct2>::value;
        REQUIRE(!postfix_dec_false);
        REQUIRE(postfix_dec_true);

        bool is_idx_false = is_indexable<Struct2>::value;
        bool is_idx_true  = is_indexable<Struct1>::value;
        REQUIRE(!is_idx_false);
        REQUIRE(is_idx_true);

        bool has_lte_true  = has_less_than_equal<Struct1, int>::value;
        bool has_lte_false = has_less_than_equal<Struct2, int>::value;
        REQUIRE(!has_lte_false);
        REQUIRE(has_lte_true);

        bool has_gte_true  = has_greater_than_equal<Struct1, int>::value;
        bool has_gte_false = has_greater_than_equal<Struct2, int>::value;
        REQUIRE(!has_gte_false);
        REQUIRE(has_gte_true);

        bool has_lt_true  = has_less_than<Struct2, int>::value;
        bool has_lt_false = has_less_than<Struct1, int>::value;
        REQUIRE(!has_lt_false);
        REQUIRE(has_lt_true);

        bool has_gt_true  = has_greater_than<Struct2, int>::value;
        bool has_gt_false = has_greater_than<Struct1, int>::value;
        REQUIRE(!has_gt_false);
        REQUIRE(has_gt_true);

        bool has_inc_by_true  = has_increment_by<Struct1, int>::value;
        bool has_inc_by_false = has_increment_by<Struct2, int>::value;
        REQUIRE(!has_inc_by_false);
        REQUIRE(has_inc_by_true);

        bool has_dec_by_true  = has_decrement_by<Struct1, int>::value;
        bool has_dec_by_false = has_decrement_by<Struct2, int>::value;
        REQUIRE(!has_dec_by_false);
        REQUIRE(has_dec_by_true);

        bool has_plus_true  = has_plus<Struct2, int>::value;
        bool has_plus_false = has_plus<Struct1, int>::value;
        REQUIRE(!has_plus_false);
        REQUIRE(has_plus_true);

        bool has_minus_true  = has_minus<Struct2, int>::value;
        bool has_minus_false = has_minus<Struct1, int>::value;
        REQUIRE(!has_minus_false);
        REQUIRE(has_minus_true);
    }

    SECTION("Container concept checks") {
        const bool is_container_false = is_container<Struct1>::value;
        const bool is_container_true = is_container<std::vector<double>>::value;
        REQUIRE(!is_container_false);
        REQUIRE(is_container_true);
    }

    SECTION("Iterator concept checks") {
        using iterator               = typename std::vector<double>::iterator;
        const bool is_iterator_false = is_iterator<Struct1>::value;
        const bool is_iterator_true  = is_iterator<iterator>::value;

        REQUIRE(!is_iterator_false);
        REQUIRE(is_iterator_true);

        SECTION("InputIterator") {
            const bool input_iterator_false = is_input_iterator<Struct1>::value;
            const bool input_iterator_true = is_input_iterator<iterator>::value;
            REQUIRE(!input_iterator_false);
            REQUIRE(input_iterator_true);

            SECTION("BidirectionalIterator") {
                const bool bid_iterator_false =
                  is_bidirectional_iterator<Struct1>::value;
                const bool bid_iterator_true =
                  is_bidirectional_iterator<iterator>::value;
                REQUIRE(!bid_iterator_false);
                REQUIRE(bid_iterator_true);

                SECTION("RandomAccessIterator") {
                    const bool ra_itr_false =
                      is_random_access_iterator<Struct1>::value;
                    const bool ra_itr_true =
                      is_random_access_iterator<iterator>::value;
                    REQUIRE(!ra_itr_false);
                    REQUIRE(ra_itr_true);
                }
            }
        }
    }
}
