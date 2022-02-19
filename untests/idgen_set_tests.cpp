/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020-2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <idgen.hpp/idgen_set.hpp>
#include "idgen_untests.hpp"

namespace idgen = idgen_hpp;

TEST_CASE("idgen_set")
{
    SUBCASE("ctor") {
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
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(3)));
        }
        {
            std::vector<id8> v{id8(1), id8(2), id8(3)};
            id8_set s(v.begin(), v.end());
            REQUIRE_FALSE(s.empty());
            REQUIRE(s.size() == 3u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(2)));
            REQUIRE(s.contains(id8(3)));
        }
        {
            id8_set s1{id8(1), id8(2), id8(3)};
            id8_set s2 = s1;

            REQUIRE_FALSE(s1.empty());
            REQUIRE(s1.size() == 3u);
            REQUIRE(s1.contains(id8(1)));
            REQUIRE(s1.contains(id8(2)));
            REQUIRE(s1.contains(id8(3)));

            REQUIRE_FALSE(s2.empty());
            REQUIRE(s2.size() == 3u);
            REQUIRE(s2.contains(id8(1)));
            REQUIRE(s2.contains(id8(2)));
            REQUIRE(s2.contains(id8(3)));
        }
        {
            id8_set s1{id8(1), id8(2), id8(3)};
            id8_set s2 = std::move(s1);

            REQUIRE(s1.empty());
            REQUIRE(s1.size() == 0u);

            REQUIRE_FALSE(s2.empty());
            REQUIRE(s2.size() == 3u);
            REQUIRE(s2.contains(id8(1)));
            REQUIRE(s2.contains(id8(2)));
            REQUIRE(s2.contains(id8(3)));
        }
    }

    SUBCASE("operator=") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s1{id8(1), id8(2), id8(3)};
            id8_set s2;
            s2 = s1;

            REQUIRE_FALSE(s1.empty());
            REQUIRE(s1.size() == 3u);
            REQUIRE(s1.contains(id8(1)));
            REQUIRE(s1.contains(id8(2)));
            REQUIRE(s1.contains(id8(3)));

            REQUIRE_FALSE(s2.empty());
            REQUIRE(s2.size() == 3u);
            REQUIRE(s2.contains(id8(1)));
            REQUIRE(s2.contains(id8(2)));
            REQUIRE(s2.contains(id8(3)));
        }
        {
            id8_set s1{id8(1), id8(2), id8(3)};
            id8_set s2;
            s2 = std::move(s1);

            REQUIRE(s1.empty());
            REQUIRE(s1.size() == 0u);

            REQUIRE_FALSE(s2.empty());
            REQUIRE(s2.size() == 3u);
            REQUIRE(s2.contains(id8(1)));
            REQUIRE(s2.contains(id8(2)));
            REQUIRE(s2.contains(id8(3)));
        }
        {
            id8_set s1{id8(1), id8(2), id8(3)};
            s1 = {id8(4), id8(5)};

            REQUIRE(s1.size() == 2);
            REQUIRE(s1.contains(id8(4)));
            REQUIRE(s1.contains(id8(5)));
        }
    }

    SUBCASE("empty") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s1{};
            id8_set s2{id8(11)};

            REQUIRE(s1.empty());
            REQUIRE_FALSE(s2.empty());
        }
    }

    SUBCASE("size") {
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

    SUBCASE("clear") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s{id8(11), id8(12)};
            REQUIRE(s.size() == 2u);
            REQUIRE(s.contains(id8(11)));
            REQUIRE(s.contains(id8(12)));

            s.clear();
            REQUIRE(s.size() == 0u);
            REQUIRE_FALSE(s.contains(id8(11)));
            REQUIRE_FALSE(s.contains(id8(12)));

            s.insert({id8(1), id8(2), id8(3)});
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

    SUBCASE("insert") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s;
            REQUIRE(s.size() == 0u);
            REQUIRE_FALSE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(3)));

            auto i1 = s.insert(id8(1));
            REQUIRE(i1.first == id8(1));
            REQUIRE(i1.second);

            REQUIRE(s.size() == 1u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE_FALSE(s.contains(id8(3)));

            auto i2 = s.insert(id8(3));
            REQUIRE(i2.first == id8(3));
            REQUIRE(i2.second);

            REQUIRE(s.size() == 2u);
            REQUIRE(s.contains(id8(1)));
            REQUIRE(s.contains(id8(3)));

            auto i1b = s.insert(id8(1));
            REQUIRE(i1b.first == id8(1));
            REQUIRE_FALSE(i1b.second);

            auto i2b = s.insert(id8(3));
            REQUIRE(i2b.first == id8(3));
            REQUIRE_FALSE(i2b.second);

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

    SUBCASE("emplace") {
        using id8 = idgen::id8<struct untagged>;
        using id8_set = idgen::id8_set<struct untagged>;
        {
            id8_set s;

            auto e1 = s.emplace(std::uint8_t(1), std::uint8_t(2));
            REQUIRE(e1.first == id8(1,2));
            REQUIRE(e1.second);

            REQUIRE(s.size() == 1u);
            REQUIRE(s.contains(id8(1,2)));

            auto e2 = s.emplace(id8(1,2));
            REQUIRE(e2.first == id8(1,2));
            REQUIRE_FALSE(e2.second);

            REQUIRE(s.size() == 1u);
            REQUIRE(s.contains(id8(1,2)));
        }
    }

    SUBCASE("erase") {
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

    SUBCASE("swap") {
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

    SUBCASE("count") {
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

    SUBCASE("contains") {
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
