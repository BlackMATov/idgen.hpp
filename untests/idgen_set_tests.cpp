/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include <idgen.hpp/idgen_set.hpp>
namespace idgen = idgen_hpp;

namespace
{
}

TEST_CASE("idgen_set")
{
    SECTION("ctor") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s;
            REQUIRE(s.empty());
            REQUIRE(s.size() == 0u);
        }
        {
            id8_set s{id8(1), id8(2), id8(3)};
            REQUIRE_FALSE(s.empty());
            REQUIRE(s.size() == 3u);
        }
    }

    SECTION("empty") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s1{};
            id8_set s2{id8(11)};

            REQUIRE(s1.empty());
            REQUIRE_FALSE(s2.empty());
        }
    }

    SECTION("size") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s1{};
            id8_set s2{id8(11)};
            id8_set s3{id8(11), id8(12)};
            id8_set s4{id8(11), id8(12), id8(13)};

            REQUIRE(s1.size() == 0u);
            REQUIRE(s2.size() == 1u);
            REQUIRE(s3.size() == 2u);
            REQUIRE(s4.size() == 3u);
        }
    }

    SECTION("clear") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s{id8(11), id8(12)};
            REQUIRE(s.size() == 2u);

            s.clear();
            REQUIRE(s.size() == 0u);

            s.insert({id8(1), id8(2), id8(3)});
            REQUIRE(s.size() == 3u);

            s.clear();
            REQUIRE(s.size() == 0u);
        }
    }

    SECTION("insert") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s;
            REQUIRE(s.size() == 0u);
            REQUIRE_FALSE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(3)));

            REQUIRE(s.insert(id8(1)));
            REQUIRE(s.size() == 1u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(3)));

            REQUIRE(s.insert(id8(3)));
            REQUIRE(s.size() == 2u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(3)));

            REQUIRE_FALSE(s.insert(id8(1)));
            REQUIRE_FALSE(s.insert(id8(3)));
            REQUIRE(s.size() == 2u);
        }
        {
            id8_set s;
            REQUIRE(s.size() == 0u);

            s.insert({id8(1), id8(2), id8(3)});
            REQUIRE(s.size() == 3u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(3)));
        }
    }

    SECTION("emplace") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s;

            REQUIRE(s.emplace(1,2));
            REQUIRE(s.size() == 1u);
            REQUIRE(s.contains(id8(1,2)));

            REQUIRE_FALSE(s.emplace(1,2));
            REQUIRE(s.size() == 1u);
            REQUIRE(s.contains(id8(1,2)));
        }
    }

    SECTION("erase") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s{id8(1), id8(2), id8(4)};
            REQUIRE(s.size() == 3u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(4)));

            REQUIRE(s.erase(id8(3)) == 0u);
            REQUIRE(s.erase(id8(2)) == 1u);
            REQUIRE(s.size() == 2u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(4)));

            REQUIRE(s.erase(id8(1)) == 1u);
            REQUIRE(s.size() == 1u);
            REQUIRE_FALSE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(4)));

            REQUIRE(s.erase(id8(4)) == 1u);
            REQUIRE(s.size() == 0u);
            REQUIRE_FALSE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(2)));
            REQUIRE_FALSE(s.contains(id8(4)));
        }
    }

    SECTION("swap") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s1{id8(1), id8(3)};
            id8_set s2{id8(2), id8(4), id8(6)};

            using std::swap;
            swap(s1, s2);

            REQUIRE(s1.size() == 3u);
            REQUIRE(s2.size() == 2u);

            REQUIRE_FALSE(s1.contains(id8(1)));
            REQUIRE(s1.contains(id8(2)));
            REQUIRE_FALSE(s1.contains(id8(3)));
            REQUIRE(s1.contains(id8(4)));
            REQUIRE(s1.contains(id8(6)));

            REQUIRE(s2.contains(id8(1)));
            REQUIRE_FALSE(s2.contains(id8(2)));
            REQUIRE(s2.contains(id8(3)));
            REQUIRE_FALSE(s2.contains(id8(4)));
            REQUIRE_FALSE(s2.contains(id8(6)));
        }
    }

    SECTION("count") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s{id8(1), id8(3)};
            REQUIRE(s.count(id8(1)) == 1u);
            REQUIRE(s.count(id8(2)) == 0u);
            REQUIRE(s.count(id8(3)) == 1u);
            REQUIRE(s.count(id8(4)) == 0u);
        }
    }

    SECTION("contains") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s{id8(1), id8(3)};
            REQUIRE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(3)));
            REQUIRE_FALSE(s.contains(id8(4)));
        }
    }
}
