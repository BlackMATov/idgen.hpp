/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <initializer_list>
#include <utility>
#include <vector>

#include "idgen.hpp"
#include "detail/idgen_utility.hpp"

namespace idgen_hpp
{
    template < typename Tag
             , typename Key
             , typename Traits = id_value_traits<Key> >
    class basic_id_set {
    public:
        using id_type = basic_id<Tag, Key, Traits>;
        using key_type = id_type;
        using value_type = id_type;
    public:
        basic_id_set() = default;

        basic_id_set(std::initializer_list<value_type> init) {
            insert(init);
        }

        template < typename InputIterator >
        basic_id_set(InputIterator first, InputIterator last) {
            insert(first, last);
        }

        bool empty() const noexcept {
            return dense_.empty();
        }

        std::size_t size() const noexcept {
            return dense_.size();
        }

        void clear() noexcept {
            dense_.clear();
        }

        bool insert(const value_type& value) {
            if ( contains(value) ) {
                return false;
            }

            const auto index = value.index();
            if ( index >= sparse_.size() ) {
                sparse_.resize(detail::next_capacity_size(
                    sparse_.size(), index + 1u, sparse_.max_size()));
            }

            dense_.push_back(value);
            sparse_[index] = dense_.size() - 1u;
            return true;
        }

        void insert(std::initializer_list<value_type> init) {
            insert(init.begin(), init.end());
        }

        template < typename InputIterator >
        void insert(InputIterator first, InputIterator last) {
            for ( ; first != last; ++first ) {
                insert(*first);
            }
        }

        template < typename... Args >
        bool emplace(Args&&... args) {
            return insert(value_type(std::forward<Args>(args)...));
        }

        std::size_t erase(const key_type& key) {
            if ( !contains(key) ) {
                return 0u;
            }
            const auto index = key.index();
            const auto dense_index = sparse_[index];
            if ( dense_index != dense_.size() - 1u ) {
                using std::swap;
                swap(dense_[dense_index], dense_.back());
                sparse_[dense_[dense_index].index()] = dense_index;
            }
            dense_.pop_back();
            return 1u;
        }

        void swap(basic_id_set& other) noexcept {
            using std::swap;
            swap(dense_, other.dense_);
            swap(sparse_, other.sparse_);
        }

        std::size_t count(const key_type& key) const {
            return contains(key) ? 1u : 0u;
        }

        bool contains(const key_type& key) const noexcept {
            const auto index = key.index();
            return index < sparse_.size()
                && sparse_[index] < dense_.size()
                && dense_[sparse_[index]] == key;
        }
    private:
        std::vector<value_type> dense_;
        std::vector<std::size_t> sparse_;
    };

    template < typename Tag, typename Key, typename Traits >
    void swap(
        basic_id_set<Tag, Key, Traits>& l,
        basic_id_set<Tag, Key, Traits>& r)
        noexcept(noexcept(l.swap(r)))
    {
        l.swap(r);
    }
}

namespace idgen_hpp
{
    template < typename Tag >
    using id8_set = basic_id_set<Tag, std::uint8_t>;

    template < typename Tag >
    using id16_set = basic_id_set<Tag, std::uint16_t>;

    template < typename Tag >
    using id32_set = basic_id_set<Tag, std::uint32_t>;

    template < typename Tag >
    using id64_set = basic_id_set<Tag, std::uint64_t>;
}
