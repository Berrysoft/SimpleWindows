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
        constexpr dev_context(HDC hDC = nullptr) noexcept : hDC(hDC), oriHBit(nullptr), oriHB(nullptr), oriHF(nullptr), oriHP(nullptr), released(false) {}
        dev_context(const dev_context& dc) = delete;
        dev_context(dev_context&& dc) noexcept;

        dev_context& operator=(const dev_context& dc) = delete;
        dev_context& operator=(dev_context&& dc) noexcept;

        virtual ~dev_context();

        constexpr HDC handle() const noexcept { return hDC; }

        int map_mode() const noexcept;
        void map_mode(int mode) noexcept;
        int text_align() const noexcept;
        void text_align(int mode) noexcept;
        int back_mode() const noexcept;
        void back_mode(int mode) noexcept;
        COLORREF text_color() const noexcept;
        void text_color(COLORREF color) noexcept;
        COLORREF back_color() const noexcept;
        void back_color(COLORREF color) noexcept;

        wil::unique_hbitmap set_bitmap(wil::unique_hbitmap&& p) noexcept;
        template <typename Creater>
        wil::unique_hbitmap set_bitmap(Creater&& c) noexcept
        {
            return set_bitmap(c.create());
        }

        wil::unique_hbrush set_brush(wil::unique_hbrush&& p) noexcept;
        template <typename Creater>
        wil::unique_hbrush set_brush(Creater&& c) noexcept
        {
            return set_brush(c.create());
        }

        wil::unique_hfont set_font(wil::unique_hfont&& p) noexcept;
        template <typename Creater>
        wil::unique_hfont set_font(Creater&& c) noexcept
        {
            return set_font(c.create());
        }

        wil::unique_hpen set_pen(wil::unique_hpen&& p) noexcept;
        template <typename Creater>
        wil::unique_hpen set_pen(Creater&& c) noexcept
        {
            return set_pen(c.create());
        }

        POINT set_org(POINT p);

        void draw_arc(RECT r, POINT p1, POINT p2);
        void draw_cross(POINT p, int length);
        void draw_ellipse(RECT r);
        void draw_ellipse(POINT p, int radius);
        void draw_line(POINT p1, POINT p2);
        void draw_pie(RECT r, POINT p1, POINT p2);
        void draw_polygon(array_view<POINT> ps);
        void draw_polyline(array_view<POINT> ps);
        void draw_rect(RECT r);
        void draw_roundrect(RECT r, SIZE round);
        void draw_string(POINT p, const string_t& str);

        void copy_dc_bit(RECT r, const dev_context& dc, POINT p = { 0, 0 }, DWORD rop = SRCCOPY) const;
        void strech_dc_bit(RECT r, const dev_context& dc, RECT r1, DWORD rop = SRCCOPY) const;
    };
} // namespace sw
