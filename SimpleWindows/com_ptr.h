#pragma once
#include "utility.h"

#include <combaseapi.h>
#include <memory>

namespace sw
{
    template <typename T>
    struct com_delete
    {
        SW_CONSTEXPR com_delete() SW_NOEXCEPT = default;
        void operator()(T* ptr) const
        {
            ptr->Release();
        }
    };

    template <typename T>
    using com_ptr = std::unique_ptr<T, com_delete<T>>;

    template <typename T>
    T** operator&(com_ptr<T>& p)
    {
        return &(p._Myptr());
    }

    template <typename T>
    struct co_task_mem_delete
    {
        SW_CONSTEXPR co_task_mem_delete() SW_NOEXCEPT = default;
        void operator()(T* ptr) const
        {
            CoTaskMemFree(ptr);
        }
    };

    template <typename T>
    using co_task_mem_ptr = std::unique_ptr<T, co_task_mem_delete<T>>;

    template <typename T>
    T** operator&(co_task_mem_ptr<T>& p)
    {
        return &(p._Myptr());
    }
} // namespace sw
