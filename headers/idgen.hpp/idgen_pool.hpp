/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020-2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <stdexcept>
#include <utility>
#include <vector>

#include "idgen_id.hpp"

namespace idgen_hpp
{
    template < typename Tag
             , typename Value
             , typename Traits = id_value_traits<Value> >
    class pool {
    public:
        using id_type = id<Tag, Value, Traits>;
        using id_value_type = typename id_type::value_type;
        using id_traits_type = typename id_type::traits_type;
    public:
        pool() = default;

        pool(pool&& other) = default;
        pool& operator=(pool&& other) = default;

        pool(const pool& other) = default;
        pool& operator=(const pool& other) = default;

        id_type acquire() {
            if ( available_ != id_type::index_mask ) {
                const auto index = available_;
                const auto version = acquired_ids_[index].version();
                available_ = acquired_ids_[index].index();
                return acquired_ids_[index] = id_type(index, version);
            }

            if ( acquired_ids_.size() < id_type::index_mask ) {
                return acquired_ids_.emplace_back(
                    static_cast<id_value_type>(acquired_ids_.size()),
                    0u);
            }

            throw std::logic_error("idgen_hpp::pool (id index overflow)");
        }

        void release(id_type id) noexcept {
            assert(valid(id));
            acquired_ids_[id.index()] = id_type(
                available_ & id_type::index_mask,
                (id.version() + 1u) & id_type::version_mask);
            available_ = id.index();
        }

        bool valid(id_type id) const noexcept {
            return id.index() < acquired_ids_.size()
                && acquired_ids_[id.index()] == id;
        }

        std::size_t alive() const noexcept {
            std::size_t result = acquired_ids_.size();
            for ( auto i = available_; i != id_type::index_mask; --result ) {
                i = acquired_ids_[i].index();
            }
            return result;
        }
    private:
        std::vector<id_type> acquired_ids_;
        id_value_type available_{id_type::index_mask};
    };
}

namespace idgen_hpp
{
    template < typename Tag >
    using id8_pool = pool<Tag, std::uint8_t>;

    template < typename Tag >
    using id16_pool = pool<Tag, std::uint16_t>;

    template < typename Tag >
    using id32_pool = pool<Tag, std::uint32_t>;

    template < typename Tag >
    using id64_pool = pool<Tag, std::uint64_t>;
}
