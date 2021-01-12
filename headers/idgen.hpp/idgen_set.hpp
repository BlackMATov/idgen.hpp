/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/idgen.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020-2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

#include "idgen_id.hpp"
#include "idgen_utility.hpp"

namespace idgen_hpp
{
    template < typename Key
             , typename Index = index<Key>
             , typename KeyEqual = std::equal_to<Key> >
    class set : Index, KeyEqual {
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

        set& operator=(std::initializer_list<value_type> init) {
            set(init).swap(*this);
            return *this;
        }

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

        std::pair<value_type&, bool> insert(value_type&& value) {
            const auto dense_index_p = find_dense_index(value);
            if ( dense_index_p.second ) {
                return {dense_[dense_index_p.first], false};
            }

            const auto index = static_cast<indexer&>(*this)(value);
            if ( index >= sparse_.size() ) {
                sparse_.resize(detail::next_capacity_size(
                    sparse_.size(), index + 1u, sparse_.max_size()));
            }

            dense_.push_back(std::move(value));
            sparse_[index] = dense_.size() - 1u;
            return {dense_.back(), true};
        }

        std::pair<value_type&, bool> insert(const value_type& value) {
            const auto dense_index_p = find_dense_index(value);
            if ( dense_index_p.second ) {
                return {dense_[dense_index_p.first], false};
            }

            const auto index = static_cast<indexer&>(*this)(value);
            if ( index >= sparse_.size() ) {
                sparse_.resize(detail::next_capacity_size(
                    sparse_.size(), index + 1u, sparse_.max_size()));
            }

            dense_.push_back(value);
            sparse_[index] = dense_.size() - 1u;
            return {dense_.back(), true};
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
        std::pair<value_type&, bool> emplace(Args&&... args) {
            return insert(value_type(std::forward<Args>(args)...));
        }

        std::size_t erase(const key_type& key) {
            const auto dense_index_p = find_dense_index(key);
            if ( !dense_index_p.second ) {
                return 0u;
            }

            const auto dense_index = dense_index_p.first;
            const auto back_dense_index = static_cast<indexer&>(*this)(dense_.back());

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
            swap(static_cast<indexer&>(*this), static_cast<indexer&>(other));
            swap(static_cast<key_equal&>(*this), static_cast<key_equal&>(other));
            swap(dense_, other.dense_);
            swap(sparse_, other.sparse_);
        }

        std::size_t count(const key_type& key) const {
            return find_dense_index(key).second ? 1u : 0u;
        }

        bool contains(const key_type& key) const {
            return find_dense_index(key).second;
        }

        indexer index_function() const {
            return *this;
        }

        key_equal key_eq() const {
            return *this;
        }

        std::size_t get_dense_index(const key_type& key) const {
            const auto dense_index_p = find_dense_index(key);
            if ( dense_index_p.second ) {
                return dense_index_p.first;
            }
            throw std::logic_error("idgen_hpp::set (key not found)");
        }

        std::pair<std::size_t, bool> find_dense_index(const key_type& key) const {
            const auto index = static_cast<const indexer&>(*this)(key);

            if ( index < sparse_.size()
                && sparse_[index] < dense_.size()
                && static_cast<const key_equal&>(*this)(dense_[sparse_[index]], key) )
            {
                return {sparse_[index], true};
            }

            return {std::size_t(-1), false};
        }
    private:
        using dense_type = std::vector<value_type>;
        using sparse_type = std::vector<std::size_t>;
    private:
        dense_type dense_;
        sparse_type sparse_;
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
