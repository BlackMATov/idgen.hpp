/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include <idgen.hpp/idgen_map.hpp>
namespace idgen = idgen_hpp;

TEST_CASE("idgen_map")
{
    SECTION("ctor") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s;
            REQUIRE(s.empty());
            REQUIRE(s.size() == 0u);
        }
        {
            id8_map s{{id8(1), 10u}, {id8(2), 20u}};
            REQUIRE_FALSE(s.empty());
            REQUIRE(s.size() == 2u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(2)));
            REQUIRE(s.at(id8(1)) == 10u);
            REQUIRE(s.at(id8(2)) == 20u);
        }
    }

    SECTION("operator=") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s1{{id8(1), 10u}, {id8(2), 20u}, {id8(3), 30u}};
            id8_map s2;
            s2 = s1;

            REQUIRE_FALSE(s1.empty());
            REQUIRE(s1.size() == 3u);
            REQUIRE(s1.contains(id8(1)));
            REQUIRE(s1.contains(id8(2)));
            REQUIRE(s1.contains(id8(3)));
            REQUIRE(s1.at(id8(1)) == 10u);
            REQUIRE(s1.at(id8(2)) == 20u);
            REQUIRE(s1.at(id8(3)) == 30u);

            REQUIRE_FALSE(s2.empty());
            REQUIRE(s2.size() == 3u);
            REQUIRE(s2.contains(id8(1)));
            REQUIRE(s2.contains(id8(2)));
            REQUIRE(s2.contains(id8(3)));
            REQUIRE(s2.at(id8(1)) == 10u);
            REQUIRE(s2.at(id8(2)) == 20u);
            REQUIRE(s2.at(id8(3)) == 30u);
        }
        {
            id8_map s1{{id8(1), 10u}, {id8(2), 20u}, {id8(3), 30u}};
            id8_map s2;
            s2 = std::move(s1);

            REQUIRE(s1.empty());
            REQUIRE(s1.size() == 0u);

            REQUIRE_FALSE(s2.empty());
            REQUIRE(s2.size() == 3u);
            REQUIRE(s2.contains(id8(1)));
            REQUIRE(s2.contains(id8(2)));
            REQUIRE(s2.contains(id8(3)));
            REQUIRE(s2.at(id8(1)) == 10u);
            REQUIRE(s2.at(id8(2)) == 20u);
            REQUIRE(s2.at(id8(3)) == 30u);
        }
    }

    SECTION("empty") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s1{};
            id8_map s2{{id8(11), 110u}};

            REQUIRE(s1.empty());
            REQUIRE_FALSE(s2.empty());
        }
    }

    SECTION("size") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s1{};
            id8_map s2{{id8(11), 110u}};
            id8_map s3{{id8(11), 110u}, {id8(12), 120u}};
            id8_map s4{{id8(11), 110u}, {id8(12), 120u}, {id8(13), 130u}};

            REQUIRE(s1.size() == 0u);
            REQUIRE(s2.size() == 1u);
            REQUIRE(s3.size() == 2u);
            REQUIRE(s4.size() == 3u);
        }
    }

    SECTION("clear") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s{{id8(11), 110u}, {id8(12), 120u}};
            REQUIRE(s.size() == 2u);
            REQUIRE(s.contains(id8(11)));
            REQUIRE(s.contains(id8(12)));

            s.clear();
            REQUIRE(s.size() == 0u);
            REQUIRE_FALSE(s.contains(id8(11)));
            REQUIRE_FALSE(s.contains(id8(12)));

            s.insert({{id8(1), 10u}, {id8(2), 20u}, {id8(3), 30u}});
            REQUIRE(s.size() == 3u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(3)));

            s.clear();
            REQUIRE(s.size() == 0u);
            REQUIRE_FALSE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(2)));
            REQUIRE_FALSE(s.contains(id8(3)));
        }
    }

    SECTION("insert") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s;
            REQUIRE(s.size() == 0u);
            REQUIRE_FALSE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(3)));

            auto i1 = s.insert({id8(1), 10u});
            REQUIRE(i1.first == 10u);
            REQUIRE(i1.second);

            REQUIRE(s.size() == 1u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(3)));

            auto i2 = s.insert({id8(3), 30u});
            REQUIRE(i2.first == 30u);
            REQUIRE(i2.second);

            REQUIRE(s.size() == 2u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(3)));

            auto i1b = s.insert({id8(1), 10u});
            REQUIRE(i1b.first == 10u);
            REQUIRE_FALSE(i1b.second);

            auto i2b = s.insert({id8(3), 30u});
            REQUIRE(i2b.first == 30u);
            REQUIRE_FALSE(i2b.second);

            REQUIRE(s.size() == 2u);
        }
        {
            id8_map s;
            REQUIRE(s.size() == 0u);

            s.insert({{id8(1), 10u}, {id8(2), 20u}, {id8(3), 30u}});
            REQUIRE(s.size() == 3u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(3)));
            REQUIRE(s.at(id8(1)) == 10u);
            REQUIRE(s.at(id8(2)) == 20u);
            REQUIRE(s.at(id8(3)) == 30u);
        }
    }

    SECTION("insert_or_assign") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s;

            const id8 id8_1(1);
            auto i1 = s.insert_or_assign(id8_1, 10u);
            REQUIRE(i1.first == 10u);
            REQUIRE(i1.second);

            auto i2 = s.insert_or_assign(id8(2), 20u);
            REQUIRE(i2.first == 20u);
            REQUIRE(i2.second);

            auto i3 = s.insert_or_assign(id8_1, 100u);
            REQUIRE(i3.first == 100u);
            REQUIRE_FALSE(i3.second);

            auto i4 = s.insert_or_assign(id8(2), 200u);
            REQUIRE(i4.first == 200u);
            REQUIRE_FALSE(i4.second);
        }
    }

    SECTION("emplace") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s;

            auto e1 = s.emplace(std::make_pair(id8(1), 10u));
            REQUIRE(e1.first == 10u);
            REQUIRE(e1.second);
            REQUIRE(s.size() == 1u);
            REQUIRE(s.at(id8(1)) == 10u);

            auto e2 = s.emplace(id8(2), 20u);
            REQUIRE(e2.first == 20u);
            REQUIRE(e2.second);
            REQUIRE(s.size() == 2u);
            REQUIRE(s.at(id8(2)) == 20u);

            auto e3 = s.emplace(id8(2), 30u);
            REQUIRE(e3.first == 20u);
            REQUIRE_FALSE(e3.second);
            REQUIRE(s.size() == 2u);
            REQUIRE(s.at(id8(2)) == 20u);
        }
    }

    SECTION("try_emplace") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s;

            const id8 id8_1(1);
            auto e1 = s.try_emplace(id8_1, 10u);
            REQUIRE(e1.first == 10u);
            REQUIRE(e1.second);
            REQUIRE(s.size() == 1u);
            REQUIRE(s.at(id8_1) == 10u);

            auto e2 = s.try_emplace(id8(2), 20u);
            REQUIRE(e2.first == 20u);
            REQUIRE(e2.second);
            REQUIRE(s.size() == 2u);
            REQUIRE(s.at(id8(2)) == 20u);

            auto e3 = s.try_emplace(id8(2), 30u);
            REQUIRE(e3.first == 20u);
            REQUIRE_FALSE(e3.second);
            REQUIRE(s.size() == 2u);
            REQUIRE(s.at(id8(2)) == 20u);

            auto e4 = s.try_emplace(id8(4));
            REQUIRE(e4.second);
            REQUIRE(s.size() == 3u);
        }
    }

    SECTION("erase") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s{{id8(1), 10u}, {id8(2), 20u}, {id8(4), 40u}};
            REQUIRE(s.size() == 3u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(4)));
            REQUIRE(s.at(id8(1)) == 10u);
            REQUIRE(s.at(id8(2)) == 20u);
            REQUIRE(s.at(id8(4)) == 40u);

            REQUIRE(s.erase(id8(3)) == 0u);
            REQUIRE(s.erase(id8(2)) == 1u);
            REQUIRE(s.size() == 2u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(4)));
            REQUIRE(s.at(id8(1)) == 10u);
            REQUIRE(s.at(id8(4)) == 40u);

            REQUIRE(s.erase(id8(1)) == 1u);
            REQUIRE(s.size() == 1u);
            REQUIRE_FALSE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(4)));
            REQUIRE(s.at(id8(4)) == 40u);

            REQUIRE(s.erase(id8(4)) == 1u);
            REQUIRE(s.size() == 0u);
            REQUIRE_FALSE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(2)));
            REQUIRE_FALSE(s.contains(id8(4)));
        }
    }

    SECTION("swap") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s1{{id8(1), 10u}, {id8(3), 30u}};
            id8_map s2{{id8(2), 20u}, {id8(4), 40u}, {id8(6), 60u}};

            using std::swap;
            swap(s1, s2);

            REQUIRE(s1.size() == 3u);
            REQUIRE(s2.size() == 2u);

            REQUIRE_FALSE(s1.contains(id8(1)));
            REQUIRE(s1.contains(id8(2)));
            REQUIRE_FALSE(s1.contains(id8(3)));
            REQUIRE(s1.contains(id8(4)));
            REQUIRE(s1.contains(id8(6)));
            REQUIRE(s1.at(id8(2)) == 20u);
            REQUIRE(s1.at(id8(4)) == 40u);
            REQUIRE(s1.at(id8(6)) == 60u);

            REQUIRE(s2.contains(id8(1)));
            REQUIRE_FALSE(s2.contains(id8(2)));
            REQUIRE(s2.contains(id8(3)));
            REQUIRE_FALSE(s2.contains(id8(4)));
            REQUIRE_FALSE(s2.contains(id8(6)));
            REQUIRE(s2.at(id8(1)) == 10u);
            REQUIRE(s2.at(id8(3)) == 30u);
        }
    }

    SECTION("at") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s{{id8(1), 10u}, {id8(2), 20u}};

            REQUIRE(s.at(id8(1)) == 10u);
            REQUIRE(s.at(id8(2)) == 20u);
            REQUIRE_THROWS_AS(s.at(id8(3)), std::logic_error);

            REQUIRE(std::as_const(s).at(id8(1)) == 10u);
            REQUIRE(std::as_const(s).at(id8(2)) == 20u);
            REQUIRE_THROWS_AS(std::as_const(s).at(id8(3)), std::logic_error);

            s.at(id8(2)) = 30u;
            REQUIRE(std::as_const(s).at(id8(2)) == 30u);
        }
    }

    SECTION("operator[]") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s{{id8(1), 10u}, {id8(3), 30u}};
            REQUIRE(s[id8(1)] == 10u);
            REQUIRE(s[id8(3)] == 30u);

            s[id8(2)] = 20u;
            REQUIRE(s[id8(2)] == 20u);

            const id8 id8_4(4);
            s[id8_4] = 40u;
            REQUIRE(s[id8_4] == 40u);
        }
    }

    SECTION("count") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s{{id8(1), 10u}, {id8(3), 30u}};
            REQUIRE(s.count(id8(1)) == 1u);
            REQUIRE(s.count(id8(2)) == 0u);
            REQUIRE(s.count(id8(3)) == 1u);
            REQUIRE(s.count(id8(4)) == 0u);
        }
    }

    SECTION("contains") {
        using id8 = idgen::id8<struct untagged>;
        using id8_map = idgen::id8_map<struct untagged, std::uint32_t>;
        {
            id8_map s{{id8(1), 10u}, {id8(3), 30u}};
            REQUIRE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(3)));
            REQUIRE_FALSE(s.contains(id8(4)));
        }
    }
}
