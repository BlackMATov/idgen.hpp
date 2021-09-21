/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020-2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <idgen.hpp/idgen_id.hpp>
#include "doctest/doctest.hpp"

#include <unordered_map>
#include <unordered_set>

namespace idgen = idgen_hpp;

TEST_CASE("idgen_id")
{
    SUBCASE("id8") {
        using id8 = idgen::id8<struct untagged>;
        STATIC_CHECK(sizeof(id8) == 1u);
        STATIC_CHECK(id8::index_bits == 5u);
        STATIC_CHECK(id8::version_bits == 3u);
        STATIC_CHECK(id8::index_mask == 31u);
        STATIC_CHECK(id8::version_mask == 7u);
    }

    SUBCASE("id16") {
        using id16 = idgen::id16<struct untagged>;
        STATIC_CHECK(sizeof(id16) == 2u);
        STATIC_CHECK(id16::index_bits == 10u);
        STATIC_CHECK(id16::version_bits == 6u);
        STATIC_CHECK(id16::index_mask == 1023u);
        STATIC_CHECK(id16::version_mask == 63u);
    }

    SUBCASE("id32") {
        using id32 = idgen::id32<struct untagged>;
        STATIC_CHECK(sizeof(id32) == 4u);
        STATIC_CHECK(id32::index_bits == 20u);
        STATIC_CHECK(id32::version_bits == 12u);
        STATIC_CHECK(id32::index_mask == 1048575u);
        STATIC_CHECK(id32::version_mask == 4095u);
    }

    SUBCASE("id64") {
        using id64 = idgen::id64<struct untagged>;
        STATIC_CHECK(sizeof(id64) == 8u);
        STATIC_CHECK(id64::index_bits == 40u);
        STATIC_CHECK(id64::version_bits == 24u);
        STATIC_CHECK(id64::index_mask == 1099511627775ull);
        STATIC_CHECK(id64::version_mask == 16777215ull);
    }

    SUBCASE("id") {
        using id8 = idgen::id8<struct untagged>;
        {
            id8 id;
            REQUIRE(id.value() == 0b000'00000);
            REQUIRE(id.index() == 0u);
            REQUIRE(id.version() == 0u);
        }
        {
            id8 id(31u);
            REQUIRE(id.value() == 0b000'11111);
            REQUIRE(id.index() == 31u);
            REQUIRE(id.version() == 0u);
        }
        {
            id8 id(21u, 2u);
            REQUIRE(id.value() == 0b010'10101);
            REQUIRE(id.index() == 21u);
            REQUIRE(id.version() == 2u);
        }
        {
            REQUIRE(id8() == id8());
            REQUIRE(id8(1, 3) == id8(1, 3));

            REQUIRE_FALSE(id8(1, 3) == id8(1, 4));
            REQUIRE_FALSE(id8(1, 3) == id8(2, 3));
            REQUIRE_FALSE(id8(1, 3) == id8(2, 4));
        }
        {
            REQUIRE_FALSE(id8() != id8());
            REQUIRE_FALSE(id8(1, 3) != id8(1, 3));

            REQUIRE(id8(1, 3) != id8(1, 4));
            REQUIRE(id8(1, 3) != id8(2, 3));
            REQUIRE(id8(1, 3) != id8(2, 4));
        }
        {
            REQUIRE_FALSE(id8(1, 3) < id8(1, 3));
            REQUIRE(id8(1, 3) <= id8(1, 3));

            REQUIRE(id8(1, 3) < id8(2, 3));
            REQUIRE(id8(1, 3) < id8(1, 4));
            REQUIRE(id8(2, 3) < id8(1, 4));
            REQUIRE(id8(1, 3) <= id8(2, 3));
            REQUIRE(id8(1, 3) <= id8(1, 4));
            REQUIRE(id8(2, 3) <= id8(1, 4));

            REQUIRE_FALSE(id8(2, 3) < id8(1, 3));
            REQUIRE_FALSE(id8(1, 4) < id8(1, 3));
            REQUIRE_FALSE(id8(1, 4) < id8(2, 3));
            REQUIRE_FALSE(id8(2, 3) <= id8(1, 3));
            REQUIRE_FALSE(id8(1, 4) <= id8(1, 3));
            REQUIRE_FALSE(id8(1, 4) <= id8(2, 3));
        }
    }

    SUBCASE("id_index") {
        using id8 = idgen::id8<struct untagged>;
        {
            REQUIRE(idgen::index<id8>()(id8(1,2)) == 1u);
            REQUIRE(idgen::index<id8>()(id8(2,3)) == 2u);
            REQUIRE(idgen::index<id8>()(id8(3,4)) == 3u);
        }
    }

    SUBCASE("id_hash") {
        using id8 = idgen::id8<struct untagged>;
        using id8_hash_set = std::unordered_set<id8>;
        using id8_hash_map = std::unordered_map<id8, id8>;
        {
            id8_hash_set s;
            REQUIRE(s.insert(id8(2)).second);
            REQUIRE(s.count(id8(2)) == 1u);
        }
        {
            id8_hash_map m;
            REQUIRE(m.emplace(id8(2), id8(4)).second);
            REQUIRE(m.at(id8(2)) == id8(4));
        }
    }
}
