#pragma once
#include "gdi.h"

namespace sw
{
    class window_paint_dc : public dev_context
    {
    private:
        HWND hWnd;
        PAINTSTRUCT ps;

    public:
        window_paint_dc(HWND hWnd = SW_NULLPTR) SW_NOEXCEPT;
        ~window_paint_dc() override;
    };

    class window_dc : public dev_context
    {
    private:
        HWND hWnd;

    public:
        window_dc(HWND hWnd = SW_NULLPTR) SW_NOEXCEPT;
        ~window_dc() override;
    };
} // namespace sw
