/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include <idgen.hpp/idgen.hpp>
namespace idgen = idgen_hpp;

namespace
{
}

TEST_CASE("idgen")
{
    SECTION("id8") {
        using id8 = idgen::id8<struct untagged>;
        STATIC_REQUIRE(sizeof(id8) == 1u);
        STATIC_REQUIRE(id8::index_bits == 5u);
        STATIC_REQUIRE(id8::version_bits == 3u);
        STATIC_REQUIRE(id8::index_mask == 31u);
        STATIC_REQUIRE(id8::version_mask == 7u);
    }

    SECTION("id16") {
        using id16 = idgen::id16<struct untagged>;
        STATIC_REQUIRE(sizeof(id16) == 2u);
        STATIC_REQUIRE(id16::index_bits == 10u);
        STATIC_REQUIRE(id16::version_bits == 6u);
        STATIC_REQUIRE(id16::index_mask == 1023u);
        STATIC_REQUIRE(id16::version_mask == 63u);
    }

    SECTION("id32") {
        using id32 = idgen::id32<struct untagged>;
        STATIC_REQUIRE(sizeof(id32) == 4u);
        STATIC_REQUIRE(id32::index_bits == 20u);
        STATIC_REQUIRE(id32::version_bits == 12u);
        STATIC_REQUIRE(id32::index_mask == 1048575u);
        STATIC_REQUIRE(id32::version_mask == 4095u);
    }

    SECTION("id64") {
        using id64 = idgen::id64<struct untagged>;
        STATIC_REQUIRE(sizeof(id64) == 8u);
        STATIC_REQUIRE(id64::index_bits == 40u);
        STATIC_REQUIRE(id64::version_bits == 24u);
        STATIC_REQUIRE(id64::index_mask == 1099511627775ull);
        STATIC_REQUIRE(id64::version_mask == 16777215ull);
    }

    SECTION("basic_id") {
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
}
