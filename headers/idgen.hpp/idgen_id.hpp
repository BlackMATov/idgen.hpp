/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020-2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>

#include <utility>

#include "idgen_utility.hpp"

namespace idgen_hpp
{
    template < typename Value >
    class id_value_traits;

    template <>
    class id_value_traits<std::uint8_t> {
    public:
        static constexpr int index_bits = 5;
        static constexpr int version_bits = 3;
    };

    template <>
    class id_value_traits<std::uint16_t> {
    public:
        static constexpr int index_bits = 10;
        static constexpr int version_bits = 6;
    };

    template <>
    class id_value_traits<std::uint32_t> {
    public:
        static constexpr int index_bits = 20;
        static constexpr int version_bits = 12;
    };

    template <>
    class id_value_traits<std::uint64_t> {
    public:
        static constexpr int index_bits = 40;
        static constexpr int version_bits = 24;
    };
}

namespace idgen_hpp
{
    template < typename Tag
             , typename Value
             , typename Traits = id_value_traits<Value> >
    class id final {
    public:
        using tag_type = Tag;
        using value_type = Value;
        using traits_type = Traits;
    public:
        static constexpr int index_bits = static_cast<int>(Traits::index_bits);
        static constexpr int version_bits = static_cast<int>(Traits::version_bits);

        static constexpr value_type index_mask = (value_type{1} << index_bits) - value_type{1};
        static constexpr value_type version_mask = (value_type{1} << version_bits) - value_type{1};
    public:
        id() = default;

        explicit id(value_type index)
        : value_{static_cast<value_type>(index & index_mask)} {
            assert(index <= index_mask);
        }

        id(value_type index, value_type version)
        : value_{static_cast<value_type>((index & index_mask) | ((version & version_mask) << index_bits))} {
            assert(index <= index_mask);
            assert(version <= version_mask);
        }

        value_type value() const noexcept {
            return value_;
        }

        value_type index() const noexcept {
            return value_ & index_mask;
        }

        value_type version() const noexcept {
            return (value_ >> index_bits) & version_mask;
        }
    private:
        value_type value_{};
    };
}

namespace idgen_hpp
{
    template < typename Tag, typename Value, typename Traits >
    bool operator<(id<Tag, Value, Traits> l, id<Tag, Value, Traits> r) noexcept {
        return l.value() < r.value();
    }

    template < typename Tag, typename Value, typename Traits >
    bool operator<=(id<Tag, Value, Traits> l, id<Tag, Value, Traits> r) noexcept {
        return l.value() <= r.value();
    }

    template < typename Tag, typename Value, typename Traits >
    bool operator==(id<Tag, Value, Traits> l, id<Tag, Value, Traits> r) noexcept {
        return l.value() == r.value();
    }

    template < typename Tag, typename Value, typename Traits >
    bool operator!=(id<Tag, Value, Traits> l, id<Tag, Value, Traits> r) noexcept {
        return l.value() != r.value();
    }
}

namespace idgen_hpp
{
    template < typename Tag, typename Value, typename Traits >
    class index<id<Tag, Value, Traits>> {
        using id_type = idgen_hpp::id<Tag, Value, Traits>;
    public:
        static_assert(
            Traits::index_bits >= 0 &&
            Traits::index_bits <= sizeof(std::size_t) * CHAR_BIT);
        std::size_t operator()(const id_type& id) const noexcept {
            return static_cast<std::size_t>(id.index());
        }
    };
}

namespace std
{
    template < typename Tag, typename Value, typename Traits >
    class hash<idgen_hpp::id<Tag, Value, Traits>> {
        using id_type = idgen_hpp::id<Tag, Value, Traits>;
    public:
        std::size_t operator()(const id_type& id) const noexcept {
            return hash<typename id_type::value_type>()(id.value());
        }
    };
}

namespace idgen_hpp
{
    template < typename Tag >
    using id8 = id<Tag, std::uint8_t>;

    template < typename Tag >
    using id16 = id<Tag, std::uint16_t>;

    template < typename Tag >
    using id32 = id<Tag, std::uint32_t>;

    template < typename Tag >
    using id64 = id<Tag, std::uint64_t>;
}
