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
#include <functional>
#include <initializer_list>
#include <utility>
#include <vector>

#include "idgen_id.hpp"
#include "idgen_utility.hpp"

namespace idgen_hpp
{
    template < typename Key
             , typename Index = index<Key>
             , typename KeyEqual = std::equal_to<Key> >
    class set {
    public:
        using key_type = Key;
        using value_type = Key;
        using indexer = Index;
        using key_equal = KeyEqual;
    public:
        set() = default;

        set(set&& other) = default;
        set& operator=(set&& other) = default;

        set(const set& other) = default;
        set& operator=(const set& other) = default;

        set(std::initializer_list<value_type> init) {
            insert(init);
        }

        template < typename InputIterator >
        set(InputIterator first, InputIterator last) {
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

        bool insert(value_type&& value) {
            if ( contains(value) ) {
                return false;
            }

            const auto index = indexer_(value);
            if ( index >= sparse_.size() ) {
                sparse_.resize(detail::next_capacity_size(
                    sparse_.size(), index + 1u, sparse_.max_size()));
            }

            dense_.push_back(std::move(value));
            sparse_[index] = dense_.size() - 1u;
            return true;
        }

        bool insert(const value_type& value) {
            if ( contains(value) ) {
                return false;
            }

            const auto index = indexer_(value);
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

            const auto index = indexer_(key);
            const auto dense_index = sparse_[index];
            const auto back_dense_index = indexer_(dense_.back());

            if ( dense_index != dense_.size() - 1u ) {
                using std::swap;
                swap(dense_[dense_index], dense_.back());
                sparse_[back_dense_index] = dense_index;
            }

            dense_.pop_back();
            return 1u;
        }

        void swap(set& other)
            noexcept(std::is_nothrow_swappable_v<indexer>
                && std::is_nothrow_swappable_v<key_equal>)
        {
            using std::swap;
            swap(indexer_, other.indexer_);
            swap(key_equal_, other.key_equal_);
            swap(dense_, other.dense_);
            swap(sparse_, other.sparse_);
        }

        std::size_t count(const key_type& key) const {
            return contains(key) ? 1u : 0u;
        }

        bool contains(const key_type& key) const {
            const auto index = indexer_(key);
            return index < sparse_.size()
                && sparse_[index] < dense_.size()
                && key_equal_(dense_[sparse_[index]], key);
        }
    private:
        indexer indexer_;
        key_equal key_equal_;
        std::vector<value_type> dense_;
        std::vector<std::size_t> sparse_;
    };
}

namespace idgen_hpp
{
    template < typename Key, typename Index, typename KeyEqual >
    void swap(
        set<Key, Index, KeyEqual>& l,
        set<Key, Index, KeyEqual>& r)
        noexcept(noexcept(l.swap(r)))
    {
        l.swap(r);
    }
}

namespace idgen_hpp
{
    template < typename Tag >
    using id8_set = set<id8<Tag>>;

    template < typename Tag >
    using id16_set = set<id16<Tag>>;

    template < typename Tag >
    using id32_set = set<id32<Tag>>;

    template < typename Tag >
    using id64_set = set<id64<Tag>>;
}
