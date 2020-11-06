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
#include <stdexcept>
#include <utility>
#include <vector>

#include "idgen_id.hpp"
#include "idgen_set.hpp"

namespace idgen_hpp
{
    template < typename Key
             , typename T
             , typename Index = idgen_hpp::index<Key>
             , typename KeyEqual = std::equal_to<Key> >
    class map {
    public:
        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<const Key, T>;
        using indexer = Index;
        using key_equal = KeyEqual;
    public:
        map() = default;

        map(map&& other) = default;
        map& operator=(map&& other) = default;

        map(const map& other) = default;
        map& operator=(const map& other) = default;

        map& operator=(std::initializer_list<value_type> init) {
            map(init).swap(*this);
            return *this;
        }

        map(std::initializer_list<value_type> init) {
            insert(init);
        }

        template < typename InputIterator >
        map(InputIterator first, InputIterator last) {
            insert(first, last);
        }

        bool empty() const noexcept {
            return values_.empty();
        }

        std::size_t size() const noexcept {
            return values_.size();
        }

        void clear() noexcept {
            keys_.clear();
            values_.clear();
        }

        std::pair<mapped_type&, bool> insert(value_type&& value) {
            const auto dense_index_p = keys_.find_dense_index(value.first);
            if ( dense_index_p.second ) {
                return {values_[dense_index_p.first], false};
            }

            values_.push_back(std::move(value.second));

            try {
                keys_.insert(std::move(value.first));
                return {values_.back(), true};
            } catch (...) {
                values_.pop_back();
                throw;
            }
        }

        std::pair<mapped_type&, bool> insert(const value_type& value) {
            const auto dense_index_p = keys_.find_dense_index(value.first);
            if ( dense_index_p.second ) {
                return {values_[dense_index_p.first], false};
            }

            values_.push_back(value.second);

            try {
                keys_.insert(value.first);
                return {values_.back(), true};
            } catch (...) {
                values_.pop_back();
                throw;
            }
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

        template < typename M >
        std::pair<mapped_type&, bool> insert_or_assign(const key_type& key, M&& obj) {
            const auto dense_index_p = keys_.find_dense_index(key);
            if ( dense_index_p.second ) {
                return {values_[dense_index_p.first] = std::forward<M>(obj), false};
            }

            values_.push_back(std::forward<M>(obj));

            try {
                keys_.insert(key);
                return {values_.back(), true};
            } catch (...) {
                values_.pop_back();
                throw;
            }
        }

        template < typename M >
        std::pair<mapped_type&, bool> insert_or_assign(key_type&& key, M&& obj) {
            const auto dense_index_p = keys_.find_dense_index(key);
            if ( dense_index_p.second ) {
                return {values_[dense_index_p.first] = std::forward<M>(obj), false};
            }

            values_.push_back(std::forward<M>(obj));

            try {
                keys_.insert(std::move(key));
                return {values_.back(), true};
            } catch (...) {
                values_.pop_back();
                throw;
            }
        }

        template < typename... Args >
        std::pair<mapped_type&, bool> emplace(Args&&... args) {
            return insert(value_type(std::forward<Args>(args)...));
        }

        template < typename... Args>
        std::pair<mapped_type&, bool> try_emplace(const key_type& key, Args&&... args) {
            const auto dense_index_p = keys_.find_dense_index(key);
            if ( dense_index_p.second ) {
                return {values_[dense_index_p.first], false};
            }

            return emplace(std::piecewise_construct,
                std::forward_as_tuple(key),
                std::forward_as_tuple(std::forward<Args>(args)...));
        }

        template < typename... Args>
        std::pair<mapped_type&, bool> try_emplace(key_type&& key, Args&&... args) {
            const auto dense_index_p = keys_.find_dense_index(key);
            if ( dense_index_p.second ) {
                return {values_[dense_index_p.first], false};
            }

            return emplace(std::piecewise_construct,
                std::forward_as_tuple(std::move(key)),
                std::forward_as_tuple(std::forward<Args>(args)...));
        }

        std::size_t erase(const key_type& key) {
            const auto dense_index_p = keys_.find_dense_index(key);
            if ( !dense_index_p.second ) {
                return false;
            }

            if ( dense_index_p.first != values_.size() - 1 ) {
                using std::swap;
                swap(values_[dense_index_p.first], values_.back());
            }

            values_.pop_back();
            keys_.erase(key);
            return 1u;
        }

        void swap(map& other)
            noexcept(std::is_nothrow_swappable_v<indexer>
                && std::is_nothrow_swappable_v<key_equal>)
        {
            using std::swap;
            swap(keys_, other.keys_);
            swap(values_, other.values_);
        }

        mapped_type& at(const key_type& key) {
            return values_[keys_.get_dense_index(key)];
        }

        const mapped_type& at(const key_type& key) const {
            return values_[keys_.get_dense_index(key)];
        }

        mapped_type& operator[](const key_type& key) {
            return try_emplace(key).first;
        }

        mapped_type& operator[](key_type&& key) {
            return try_emplace(std::move(key)).first;
        }

        std::size_t count(const key_type& key) const {
            return keys_.contains(key) ? 1u : 0u;
        }

        bool contains(const key_type& key) const {
            return keys_.contains(key);
        }

        indexer index_function() const {
            return keys_.index_function();
        }

        key_equal key_eq() const {
            return keys_.key_eq();
        }
    private:
        using keys_type = idgen_hpp::set<key_type, indexer, key_equal>;
        using values_type = std::vector<mapped_type>;
    private:
        keys_type keys_;
        values_type values_;
    };
}

namespace idgen_hpp
{
    template < typename Tag, typename T >
    using id8_map = map<id8<Tag>, T>;

    template < typename Tag, typename T >
    using id16_map = map<id16<Tag>, T>;

    template < typename Tag, typename T >
    using id32_map = map<id32<Tag>, T>;

    template < typename Tag, typename T >
    using id64_map = map<id64<Tag>, T>;
}
