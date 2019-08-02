#include "gdi.h"
#include "gdi_internal.h"
#include <windowsx.h>

using namespace std;
using namespace wil;

namespace sw
{
    dev_context::dev_context(dev_context&& dc) noexcept
        : hDC(dc.hDC),
          oriHBit(dc.oriHBit),
          oriHB(dc.oriHB),
          oriHF(dc.oriHF),
          oriHP(dc.oriHP),
          released(dc.released)
    {
        dc.released = true;
    }
    dev_context& dev_context::operator=(dev_context&& dc) noexcept
    {
        hDC = dc.hDC;
        oriHBit = dc.oriHBit;
        oriHB = dc.oriHB;
        oriHF = dc.oriHF;
        oriHP = dc.oriHP;
        released = dc.released;
        dc.released = true;
        return *this;
    }

    template <typename T>
    constexpr void restore_ptr(HDC hDC, T*& orip)
    {
        if (orip)
        {
            SW_ASSERT_EXPR(DeleteObject(::SelectObject(hDC, orip)), DELETE_OBJECT_FAILED);
            orip = nullptr;
        }
    }
    dev_context::~dev_context()
    {
        if (!released)
        {
            restore_ptr(hDC, oriHBit);
            restore_ptr(hDC, oriHB);
            restore_ptr(hDC, oriHF);
            restore_ptr(hDC, oriHP);
            released = true;
        }
    }

    int dev_context::map_mode() const noexcept
    {
        return GetMapMode(hDC);
    }
    void dev_context::map_mode(int mode) noexcept
    {
        SetMapMode(hDC, mode);
    }
    int dev_context::text_align() const noexcept
    {
        return GetTextAlign(hDC);
    }
    void dev_context::text_align(int mode) noexcept
    {
        SetTextAlign(hDC, mode);
    }
    int dev_context::back_mode() const noexcept
    {
        return GetBkMode(hDC);
    }
    void dev_context::back_mode(int mode) noexcept
    {
        SetBkMode(hDC, mode);
    }
    COLORREF dev_context::text_color() const noexcept
    {
        return GetTextColor(hDC);
    }
    void dev_context::text_color(COLORREF color) noexcept
    {
        SetTextColor(hDC, color);
    }
    COLORREF dev_context::back_color() const noexcept
    {
        return GetBkColor(hDC);
    }
    void dev_context::back_color(COLORREF color) noexcept
    {
        SetBkColor(hDC, color);
    }

    template <typename T, typename TPtr>
    constexpr TPtr select_ptr(HDC hDC, T*& orip, TPtr&& p) noexcept
    {
        if (p)
        {
            T* result = static_cast<T*>(::SelectObject(hDC, p.release()));
            if (!orip)
            {
                orip = result;
                return nullptr;
            }
            else
            {
                return TPtr(result);
            }
        }
        else
        {
            if (!orip)
            {
                return nullptr;
            }
            else
            {
                T* result = static_cast<T*>(::SelectObject(hDC, orip));
                orip = nullptr;
                return TPtr(result);
            }
        }
    }
    unique_hbitmap dev_context::set_bitmap(unique_hbitmap&& p) noexcept
    {
        return select_ptr(hDC, oriHBit, std::move(p));
    }
    unique_hbrush dev_context::set_brush(unique_hbrush&& p) noexcept
    {
        return select_ptr(hDC, oriHB, std::move(p));
    }
    unique_hfont dev_context::set_font(unique_hfont&& p) noexcept
    {
        return select_ptr(hDC, oriHF, std::move(p));
    }
    unique_hpen dev_context::set_pen(unique_hpen&& p) noexcept
    {
        return select_ptr(hDC, oriHP, std::move(p));
    }

    POINT dev_context::set_org(POINT p) noexcept
    {
        POINT result;
        SW_ASSERT(SetWindowOrgEx(hDC, p.x, p.y, &result));
        return result;
    }

    void dev_context::draw_arc(RECT r, POINT p1, POINT p2) noexcept
    {
        SW_ASSERT_EXPR(Arc(hDC, r.left, r.top, r.right, r.bottom, p1.x, p1.y, p2.x, p2.y), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_cross(POINT p, int length) noexcept
    {
        draw_line({ p.x - length / 2, p.y }, { p.x + length / 2, p.y });
        draw_line({ p.x, p.y - length / 2 }, { p.x, p.y + length / 2 });
    }
    void dev_context::draw_ellipse(RECT r) noexcept
    {
        SW_ASSERT_EXPR(Ellipse(hDC, r.left, r.top, r.right, r.bottom), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_ellipse(POINT p, int radius) noexcept
    {
        SW_ASSERT_EXPR(Ellipse(hDC, p.x - radius, p.y - radius, p.x + radius, p.y + radius), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_line(POINT p1, POINT p2) noexcept
    {
        SW_ASSERT_EXPR(MoveToEx(hDC, p1.x, p1.y, nullptr) && LineTo(hDC, p2.x, p2.y), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_pie(RECT r, POINT p1, POINT p2) noexcept
    {
        SW_ASSERT_EXPR(Pie(hDC, r.left, r.top, r.right, r.bottom, p1.x, p1.y, p2.x, p2.y), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_polygon(array_view<POINT> ps) noexcept
    {
        SW_ASSERT_EXPR(Polygon(hDC, ps.data(), static_cast<int>(ps.size())), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_polyline(array_view<POINT> ps) noexcept
    {
        SW_ASSERT_EXPR(Polyline(hDC, ps.data(), static_cast<int>(ps.size())), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_rect(RECT r) noexcept
    {
        SW_ASSERT_EXPR(Rectangle(hDC, r.left, r.top, r.right, r.bottom), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_roundrect(RECT r, SIZE round) noexcept
    {
        SW_ASSERT_EXPR(RoundRect(hDC, r.left, r.top, r.right, r.bottom, round.cx, round.cy), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_string(POINT p, const string_t& str) noexcept
    {
        SW_ASSERT_EXPR(TextOut(hDC, p.x, p.y, str.c_str(), static_cast<int>(str.length())), GDI_DRAWING_FAILED);
    }

    void dev_context::copy_dc_bit(RECT t, const dev_context& dc, POINT p, DWORD rop) const noexcept
    {
        SW_ASSERT_EXPR(BitBlt(hDC, t.left, t.top, t.right, t.bottom, dc.hDC, p.x, p.y, rop), GDI_DRAWING_FAILED);
    }
    void dev_context::strech_dc_bit(RECT r, const dev_context& dc, RECT r1, DWORD rop) const noexcept
    {
        SW_ASSERT_EXPR(StretchBlt(hDC, r.left, r.top, r.right - r.left, r.bottom - r.top, dc.hDC, r1.left, r1.top, r1.right - r1.left, r1.bottom - r1.top, rop), GDI_DRAWING_FAILED);
    }

    window_paint_dc::window_paint_dc(HWND hWnd) noexcept : dev_context(), hWnd(hWnd)
    {
        if (hWnd)
            hDC = ::BeginPaint(hWnd, &ps);
    }
    window_paint_dc::~window_paint_dc()
    {
        if (!released)
        {
            dev_context::~dev_context();
            if (hWnd && hDC)
            {
                SW_ASSERT_EXPR(EndPaint(hWnd, &ps), RELEASE_DC_FAILED);
                hWnd = nullptr;
            }
        }
    }

    window_dc::window_dc(HWND hWnd) noexcept : dev_context(), hWnd(hWnd)
    {
        hDC = ::GetDC(hWnd);
    }
    window_dc::~window_dc()
    {
        if (!released)
        {
            dev_context::~dev_context();
            SW_ASSERT_EXPR(ReleaseDC(hWnd, hDC), RELEASE_DC_FAILED);
        }
    }

    window_compatible_dc::window_compatible_dc(HWND hWnd, int width, int height) noexcept : dev_context(), hWnd(hWnd)
    {
        window_dc main_dc(hWnd);
        hDC = CreateCompatibleDC(main_dc.handle());
        set_bitmap(compatible_bitmap{ main_dc, width, height }.create());
    }
    window_compatible_dc::~window_compatible_dc()
    {
        if (!released)
        {
            dev_context::~dev_context();
            SW_ASSERT_EXPR(ReleaseDC(hWnd, hDC), RELEASE_DC_FAILED);
        }
    }
} // namespace sw
