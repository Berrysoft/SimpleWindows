#pragma once
#include "utility.h"

#include <iterator>

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
        SW_CONSTEXPR array_view() SW_NOEXCEPT : start(SW_NULLPTR), count(0) {}
        SW_CONSTEXPR array_view(std::initializer_list<value_type> list)
            : start(const_cast<pointer>(list.begin())), count(list.size()) {}
        template <typename Container>
        SW_CONSTEXPR array_view(Container&& list) : start(const_cast<pointer>(&*std::begin(list))), count(std::size(list))
        {
        }
        template <size_type N>
        SW_CONSTEXPR array_view(value_type (&arr)[N]) SW_NOEXCEPT : start(arr), count(N)
        {
        }

        SW_CONSTEXPR array_view& operator=(std::initializer_list<value_type> list)
        {
            start = const_cast<pointer>(list.begin());
            size = list.size();
            return *this;
        }
        template <typename Container>
        SW_CONSTEXPR array_view& operator=(Container&& list)
        {
            start = const_cast<pointer>(&*std::begin(list));
            count = std::size(list);
            return *this;
        }
        template <size_type N>
        SW_CONSTEXPR array_view& operator=(value_type (&arr)[N]) SW_NOEXCEPT
        {
            start = arr;
            count = N;
            return *this;
        }

        SW_CONSTEXPR reference operator[](size_type index) SW_NOEXCEPT { return start[index]; }
        SW_CONSTEXPR const_reference operator[](size_type index) const SW_NOEXCEPT { return start[index]; }

        SW_CONSTEXPR reference at(size_type index)
        {
            if (index >= count)
                throw std::out_of_range();
            return start[index];
        }
        SW_CONSTEXPR const_reference at(size_type index) const
        {
            if (index >= count)
                throw std::out_of_range();
            return start[index];
        }

        SW_CONSTEXPR size_type size() const SW_NOEXCEPT { return count; }
        SW_CONSTEXPR bool empty() const SW_NOEXCEPT { return !count; }

        SW_CONSTEXPR iterator begin() SW_NOEXCEPT { return start; }
        SW_CONSTEXPR iterator end() SW_NOEXCEPT { return start + count; }
        SW_CONSTEXPR reference front() { return *start; }
        SW_CONSTEXPR reference back() { return start[count - 1]; }

        SW_CONSTEXPR const_iterator cbegin() const SW_NOEXCEPT { return start; }
        SW_CONSTEXPR const_iterator cend() const SW_NOEXCEPT { return start + count; }
        SW_CONSTEXPR const_reference front() const { return *start; }
        SW_CONSTEXPR const_reference back() const { return start[count - 1]; }

        SW_CONSTEXPR iterator rbegin() { return start + count - 1; }
        SW_CONSTEXPR iterator rend() { return start - 1; }

        SW_CONSTEXPR const_iterator crbegin() const { return start + count - 1; }
        SW_CONSTEXPR const_iterator crend() const { return start - 1; }
    };
} // namespace sw
