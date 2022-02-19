/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020-2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cstddef>

#include <algorithm>
#include <stdexcept>

namespace idgen_hpp
{
    template < typename T >
    class index;
}

namespace idgen_hpp::detail
{
    inline std::size_t next_capacity_size(
        std::size_t cur_size,
        std::size_t min_size,
        std::size_t max_size)
    {
        if ( min_size > max_size ) {
            throw std::length_error("idgen_hpp::next_capacity_size");
        }

        if ( cur_size >= max_size / 2 ) {
            return max_size;
        }

        return std::max(cur_size * 2, min_size);
    }
}
