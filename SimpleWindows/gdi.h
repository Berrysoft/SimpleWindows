#pragma once
#include "utility.h"

#include "gdi_ptr.h"
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace sw
{
    class dev_context
    {
    protected:
        HDC hDC;
        HBITMAP oriHBit;
        HBRUSH oriHB;
        HFONT oriHF;
        HPEN oriHP;
        bool released;

    public:
        SW_CONSTEXPR dev_context(HDC hDC = SW_NULLPTR) SW_NOEXCEPT : hDC(hDC), oriHBit(SW_NULLPTR), oriHB(SW_NULLPTR), oriHF(SW_NULLPTR), oriHP(SW_NULLPTR), released(false) {}
        dev_context(const dev_context& dc) = delete;
        dev_context(dev_context&& dc) SW_NOEXCEPT;

        dev_context& operator=(const dev_context& dc) = delete;
        dev_context& operator=(dev_context&& dc) SW_NOEXCEPT;

        virtual ~dev_context();

        SW_CONSTEXPR HDC handle() const SW_NOEXCEPT { return hDC; }

        int map_mode() SW_NOEXCEPT;
        void map_mode(int mode) SW_NOEXCEPT;

        bitmap_ptr set_bitmap(bitmap_ptr&& p) SW_NOEXCEPT;
        brush_ptr set_brush(brush_ptr&& p) SW_NOEXCEPT;
        font_ptr set_font(font_ptr&& p) SW_NOEXCEPT;
        pen_ptr set_pen(pen_ptr&& p) SW_NOEXCEPT;

        void draw_arc(RECT r, POINT p1, POINT p2) SW_NOEXCEPT;
        void draw_ellipse(RECT r) SW_NOEXCEPT;
        void draw_line(POINT p1, POINT p2) SW_NOEXCEPT;
        void draw_pie(RECT r, POINT p1, POINT p2) SW_NOEXCEPT;
        void draw_polygon(std::initializer_list<POINT> ps) SW_NOEXCEPT;
        void draw_polygon(const std::vector<POINT>& ps) SW_NOEXCEPT;
        void draw_polyline(std::initializer_list<POINT> ps) SW_NOEXCEPT;
        void draw_polyline(const std::vector<POINT>& ps) SW_NOEXCEPT;
        void draw_rect(RECT r) SW_NOEXCEPT;
        void draw_roundrect(RECT r, SIZE round) SW_NOEXCEPT;
        void draw_string(POINT p, std::wstring str) SW_NOEXCEPT;
    };
} // namespace sw
