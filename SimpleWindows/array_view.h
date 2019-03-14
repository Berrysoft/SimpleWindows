#pragma once
#include "utility.h"

#include <initializer_list>

namespace sw
{
    template <typename T>
    class array_view
    {
    public:
        using value_type = T;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using size_type = std::size_t;

    private:
        pointer start;
        size_type count;

    public:
        constexpr array_view() noexcept : start(nullptr), count(0) {}
        constexpr array_view(std::initializer_list<value_type> list) noexcept : start(const_cast<pointer>(list.begin())), count(list.size()) {}
        template <typename Container>
        constexpr array_view(Container&& list) : start(list.data()), count(list.size())
        {
        }
        template <size_type N>
        constexpr array_view(value_type (&arr)[N]) noexcept : start(arr), count(N)
        {
        }
        constexpr array_view(pointer p, size_type size) noexcept : start(p), count(size)
        {
        }

        constexpr array_view& operator=(std::initializer_list<value_type> list) noexcept
        {
            start = const_cast<pointer>(list.begin());
            size = list.size();
            return *this;
        }
        template <typename Container>
        constexpr array_view& operator=(Container&& list)
        {
            start = list.data();
            count = std::size(list);
            return *this;
        }
        template <size_type N>
        constexpr array_view& operator=(value_type (&arr)[N]) noexcept
        {
            start = arr;
            count = N;
            return *this;
        }

        constexpr reference operator[](size_type index) noexcept { return start[index]; }
        constexpr const_reference operator[](size_type index) const noexcept { return start[index]; }

        constexpr size_type size() const noexcept { return count; }
        constexpr bool empty() const noexcept { return !count; }

        constexpr pointer data() noexcept { return start; }
        constexpr const_pointer data() const noexcept { return start; }

        constexpr iterator begin() noexcept { return start; }
        constexpr iterator end() noexcept { return start + count; }
        constexpr reference front() { return *start; }
        constexpr reference back() { return start[count - 1]; }

        constexpr const_iterator cbegin() const noexcept { return start; }
        constexpr const_iterator cend() const noexcept { return start + count; }
        constexpr const_reference front() const { return *start; }
        constexpr const_reference back() const { return start[count - 1]; }
    };
} // namespace sw
