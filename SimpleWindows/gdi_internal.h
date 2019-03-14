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
        window_paint_dc(HWND hWnd = nullptr) noexcept;
        ~window_paint_dc() override;
    };

    class window_dc : public dev_context
    {
    private:
        HWND hWnd;

    public:
        window_dc(HWND hWnd = nullptr) noexcept;
        ~window_dc() override;
    };

    class window_compatible_dc : public dev_context
    {
    private:
        HWND hWnd;

    public:
        window_compatible_dc(HWND hWnd = nullptr, int width = 0, int height = 0) noexcept;
        ~window_compatible_dc() override;
    };
} // namespace sw
