/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020-2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <idgen.hpp/idgen_pool.hpp>
#include "doctest/doctest.hpp"

namespace idgen = idgen_hpp;

TEST_CASE("idgen_pool")
{
    SUBCASE("acquire/release") {
        using id8 = idgen::id8<struct untagged>;
        using id8_pool = idgen::id8_pool<struct untagged>;
        {
            id8_pool p;

            const id8 i0 = p.acquire();
            REQUIRE(i0 == id8(0u, 0u));

            const id8 i1 = p.acquire();
            REQUIRE(i1 == id8(1u, 0u));

            const id8 i2 = p.acquire();
            REQUIRE(i2 == id8(2u, 0u));
        }
        {
            id8_pool p;

            const id8 i0_0 = p.acquire();
            REQUIRE(i0_0 == id8(0u, 0u));

            p.release(i0_0);

            const id8 i0_1 = p.acquire();
            REQUIRE(i0_1 == id8(0u, 1u));

            p.release(i0_1);

            const id8 i0_2 = p.acquire();
            REQUIRE(i0_2 == id8(0u, 2u));
        }
        {
            id8_pool p;

            std::vector<id8> ids;

            for ( id8::value_type i = 0; i < 31; ++i ) {
                ids.push_back(p.acquire());
                REQUIRE(ids.back() == id8(i, 0u));
            }

            for ( id8 id : ids ) {
                REQUIRE(p.valid(id));
            }

            REQUIRE_THROWS_AS(p.acquire(), std::logic_error);

            for ( id8 id : ids ) {
                p.release(id);
            }

            for ( id8 id : ids ) {
                REQUIRE_FALSE(p.valid(id));
            }

            ids.clear();

            for ( id8::value_type i = 0; i < 31; ++i ) {
                ids.push_back(p.acquire());
                REQUIRE(ids.back() == id8(30u - i, 1u));
            }

            for ( id8 id : ids ) {
                REQUIRE(p.valid(id));
            }

            REQUIRE_THROWS_AS(p.acquire(), std::logic_error);
        }
    }

    SUBCASE("valid") {
        using id8 = idgen::id8<struct untagged>;
        using id8_pool = idgen::id8_pool<struct untagged>;
        {
            id8_pool p;

            const id8 i0_0 = p.acquire();
            REQUIRE(i0_0 == id8(0u, 0u));
            REQUIRE(p.valid(i0_0));

            p.release(i0_0);
            REQUIRE_FALSE(p.valid(i0_0));

            const id8 i0_1 = p.acquire();
            REQUIRE(i0_1 == id8(0u, 1u));
            REQUIRE(p.valid(i0_1));
            REQUIRE_FALSE(p.valid(i0_0));

            p.release(i0_1);
            REQUIRE_FALSE(p.valid(i0_0));
            REQUIRE_FALSE(p.valid(i0_1));
        }
    }

    SUBCASE("alive") {
        using id8 = idgen::id8<struct untagged>;
        using id8_pool = idgen::id8_pool<struct untagged>;
        {
            id8_pool p;
            REQUIRE(p.alive() == 0u);

            {
                id8 i1 = p.acquire();
                REQUIRE(p.alive() == 1u);

                p.release(i1);
                REQUIRE(p.alive() == 0u);
            }

            {
                id8 i1 = p.acquire();
                REQUIRE(p.alive() == 1u);

                id8 i2 = p.acquire();
                REQUIRE(p.alive() == 2u);

                p.release(i2);
                REQUIRE(p.alive() == 1u);

                i2 = p.acquire();
                REQUIRE(p.alive() == 2u);

                p.release(i1);
                REQUIRE(p.alive() == 1u);

                p.release(i2);
                REQUIRE(p.alive() == 0u);
            }
        }
    }
}
