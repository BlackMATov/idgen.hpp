/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

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
    class basic_id {
    public:
        using tag_type = Tag;
        using value_type = Value;
        using traits_type = Traits;
    public:
        static constexpr int index_bits = static_cast<int>(Traits::index_bits);
        static constexpr int version_bits = static_cast<int>(Traits::version_bits);

        static constexpr value_type index_mask = (value_type(1) << index_bits) - value_type(1);
        static constexpr value_type version_mask = (value_type(1) << version_bits) - value_type(1);
    public:
        basic_id() = default;

        explicit basic_id(value_type index)
        : value_(index & index_mask) {
            assert(index <= index_mask);
        }

        basic_id(value_type index, value_type version)
        : value_((index & index_mask) | ((version & version_mask) << index_bits)) {
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
    bool operator<(basic_id<Tag, Value, Traits> l, basic_id<Tag, Value, Traits> r) noexcept {
        return l.value() < r.value();
    }

    template < typename Tag, typename Value, typename Traits >
    bool operator<=(basic_id<Tag, Value, Traits> l, basic_id<Tag, Value, Traits> r) noexcept {
        return l.value() <= r.value();
    }

    template < typename Tag, typename Value, typename Traits >
    bool operator==(basic_id<Tag, Value, Traits> l, basic_id<Tag, Value, Traits> r) noexcept {
        return l.value() == r.value();
    }

    template < typename Tag, typename Value, typename Traits >
    bool operator!=(basic_id<Tag, Value, Traits> l, basic_id<Tag, Value, Traits> r) noexcept {
        return l.value() != r.value();
    }
}

namespace idgen_hpp
{
    template < typename Tag >
    using id8 = basic_id<Tag, std::uint8_t>;

    template < typename Tag >
    using id16 = basic_id<Tag, std::uint16_t>;

    template < typename Tag >
    using id32 = basic_id<Tag, std::uint32_t>;

    template < typename Tag >
    using id64 = basic_id<Tag, std::uint64_t>;
}
