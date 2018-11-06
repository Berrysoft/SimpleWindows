#pragma once
#include "utility.h"

#include "array_view.h"
#include "gdi_ptr.h"
#include "string_type.h"
#include <initializer_list>
#include <memory>
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
        int text_align() SW_NOEXCEPT;
        void text_align(int mode) SW_NOEXCEPT;
        int back_mode() SW_NOEXCEPT;
        void back_mode(int mode) SW_NOEXCEPT;
        COLORREF text_color() SW_NOEXCEPT;
        void text_color(COLORREF color) SW_NOEXCEPT;
        COLORREF back_color() SW_NOEXCEPT;
        void back_color(COLORREF color) SW_NOEXCEPT;

        bitmap_ptr set_bitmap(bitmap_ptr&& p) SW_NOEXCEPT;
        template <typename Creater>
        bitmap_ptr set_bitmap(Creater&& c) SW_NOEXCEPT
        {
            return set_bitmap(c.create());
        }

        brush_ptr set_brush(brush_ptr&& p) SW_NOEXCEPT;
        template <typename Creater>
        brush_ptr set_brush(Creater&& c) SW_NOEXCEPT
        {
            return set_brush(c.create());
        }

        font_ptr set_font(font_ptr&& p) SW_NOEXCEPT;
        template <typename Creater>
        font_ptr set_font(Creater&& c) SW_NOEXCEPT
        {
            return set_font(c.create());
        }

        pen_ptr set_pen(pen_ptr&& p) SW_NOEXCEPT;
        template <typename Creater>
        pen_ptr set_pen(Creater&& c) SW_NOEXCEPT
        {
            return set_pen(c.create());
        }

        void draw_arc(RECT r, POINT p1, POINT p2) SW_NOEXCEPT;
        void draw_cross(POINT p, int length) SW_NOEXCEPT;
        void draw_ellipse(RECT r) SW_NOEXCEPT;
        void draw_ellipse(POINT p, int radius) SW_NOEXCEPT;
        void draw_line(POINT p1, POINT p2) SW_NOEXCEPT;
        void draw_pie(RECT r, POINT p1, POINT p2) SW_NOEXCEPT;
        void draw_polygon(array_view<POINT> ps) SW_NOEXCEPT;
        void draw_polyline(array_view<POINT> ps) SW_NOEXCEPT;
        void draw_rect(RECT r) SW_NOEXCEPT;
        void draw_roundrect(RECT r, SIZE round) SW_NOEXCEPT;
        void draw_string(POINT p, const string_t& str) SW_NOEXCEPT;

        void copy_dc_bit(RECT r, const dev_context& dc, POINT p = { 0, 0 }, DWORD rop = SRCCOPY) SW_NOEXCEPT;
    };
} // namespace sw
