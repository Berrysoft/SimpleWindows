#include "gdi.h"
#include "gdi_internal.h"
#include <windowsx.h>

using namespace std;

namespace sw
{
    dev_context::dev_context(dev_context&& dc) SW_NOEXCEPT
        : hDC(dc.hDC),
          oriHBit(dc.oriHBit),
          oriHB(dc.oriHB),
          oriHF(dc.oriHF),
          oriHP(dc.oriHP),
          released(dc.released)
    {
        dc.released = true;
    }
    dev_context& dev_context::operator=(dev_context&& dc) SW_NOEXCEPT
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
    SW_CONSTEXPR void restore_ptr(HDC hDC, T*& orip)
    {
        if (orip)
        {
            SW_ASSERT_EXPR(DeleteObject(SelectObject(hDC, orip)), DELETE_OBJECT_FAILED);
            orip = SW_NULLPTR;
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

    int dev_context::map_mode() SW_NOEXCEPT
    {
        return GetMapMode(hDC);
    }
    void dev_context::map_mode(int mode) SW_NOEXCEPT
    {
        SetMapMode(hDC, mode);
    }
    int dev_context::text_align() SW_NOEXCEPT
    {
        return GetTextAlign(hDC);
    }
    void dev_context::text_align(int mode) SW_NOEXCEPT
    {
        SetTextAlign(hDC, mode);
    }
    int dev_context::back_mode() SW_NOEXCEPT
    {
        return GetBkMode(hDC);
    }
    void dev_context::back_mode(int mode) SW_NOEXCEPT
    {
        SetBkMode(hDC, mode);
    }
    COLORREF dev_context::text_color() SW_NOEXCEPT
    {
        return GetTextColor(hDC);
    }
    void dev_context::text_color(COLORREF color) SW_NOEXCEPT
    {
        SetTextColor(hDC, color);
    }
    COLORREF dev_context::back_color() SW_NOEXCEPT
    {
        return GetBkColor(hDC);
    }
    void dev_context::back_color(COLORREF color) SW_NOEXCEPT
    {
        SetBkColor(hDC, color);
    }

    template <typename T>
    SW_CONSTEXPR gdi_ptr<T> select_ptr(HDC hDC, T*& orip, gdi_ptr<T>&& p) SW_NOEXCEPT
    {
        if (p)
        {
            T* result = static_cast<T*>(SelectObject(hDC, p.release()));
            if (!orip)
            {
                orip = result;
                return SW_NULLPTR;
            }
            else
            {
                return gdi_ptr<T>(result);
            }
        }
        else
        {
            if (!orip)
            {
                return SW_NULLPTR;
            }
            else
            {
                T* result = static_cast<T*>(SelectObject(hDC, orip));
                orip = SW_NULLPTR;
                return gdi_ptr<T>(result);
            }
        }
    }
    bitmap_ptr dev_context::set_bitmap(bitmap_ptr&& p) SW_NOEXCEPT
    {
        return select_ptr(hDC, oriHBit, move(p));
    }
    brush_ptr dev_context::set_brush(brush_ptr&& p) SW_NOEXCEPT
    {
        return select_ptr(hDC, oriHB, move(p));
    }
    font_ptr dev_context::set_font(font_ptr&& p) SW_NOEXCEPT
    {
        return select_ptr(hDC, oriHF, move(p));
    }
    pen_ptr dev_context::set_pen(pen_ptr&& p) SW_NOEXCEPT
    {
        return select_ptr(hDC, oriHP, move(p));
    }

	POINT dev_context::set_org(POINT p) SW_NOEXCEPT
	{
        POINT result;
        SW_ASSERT(SetWindowOrgEx(hDC, p.x, p.y, &result));
        return result;
	}

    void dev_context::draw_arc(RECT r, POINT p1, POINT p2) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(Arc(hDC, r.left, r.top, r.right, r.bottom, p1.x, p1.y, p2.x, p2.y), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_cross(POINT p, int length) SW_NOEXCEPT
    {
        draw_line({ p.x - length / 2, p.y }, { p.x + length / 2, p.y });
        draw_line({ p.x, p.y - length / 2 }, { p.x, p.y + length / 2 });
    }
    void dev_context::draw_ellipse(RECT r) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(Ellipse(hDC, r.left, r.top, r.right, r.bottom), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_ellipse(POINT p, int radius) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(Ellipse(hDC, p.x - radius, p.y - radius, p.x + radius, p.y + radius), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_line(POINT p1, POINT p2) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(MoveToEx(hDC, p1.x, p1.y, SW_NULLPTR) && LineTo(hDC, p2.x, p2.y), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_pie(RECT r, POINT p1, POINT p2) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(Pie(hDC, r.left, r.top, r.right, r.bottom, p1.x, p1.y, p2.x, p2.y), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_polygon(array_view<POINT> ps) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(Polygon(hDC, ps.begin(), static_cast<int>(ps.size())), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_polyline(array_view<POINT> ps) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(Polyline(hDC, ps.begin(), static_cast<int>(ps.size())), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_rect(RECT r) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(Rectangle(hDC, r.left, r.top, r.right, r.bottom), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_roundrect(RECT r, SIZE round) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(RoundRect(hDC, r.left, r.top, r.right, r.bottom, round.cx, round.cy), GDI_DRAWING_FAILED);
    }
    void dev_context::draw_string(POINT p, const string_t& str) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(TextOut(hDC, p.x, p.y, str.c_str(), static_cast<int>(str.length())), GDI_DRAWING_FAILED);
    }

    void dev_context::copy_dc_bit(RECT t, const dev_context& dc, POINT p, DWORD rop) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(BitBlt(hDC, t.left, t.top, t.right, t.bottom, dc.hDC, p.x, p.y, rop), GDI_DRAWING_FAILED);
    }
    void dev_context::strech_dc_bit(RECT r, const dev_context& dc, RECT r1, DWORD rop) SW_NOEXCEPT
    {
        SW_ASSERT_EXPR(StretchBlt(hDC, r.left, r.top, r.right - r.left, r.bottom - r.top, dc.hDC, r1.left, r1.top, r1.right - r1.left, r1.bottom - r1.top, rop), GDI_DRAWING_FAILED);
    }

    window_paint_dc::window_paint_dc(HWND hWnd) SW_NOEXCEPT : dev_context(), hWnd(hWnd)
    {
        if (hWnd)
            hDC = BeginPaint(hWnd, &ps);
    }
    window_paint_dc::~window_paint_dc()
    {
        if (!released)
        {
            dev_context::~dev_context();
            if (hWnd && hDC)
            {
                SW_ASSERT_EXPR(EndPaint(hWnd, &ps), RELEASE_DC_FAILED);
                hWnd = SW_NULLPTR;
            }
        }
    }

    window_dc::window_dc(HWND hWnd) SW_NOEXCEPT : dev_context(), hWnd(hWnd)
    {
        hDC = GetDC(hWnd);
    }
    window_dc::~window_dc()
    {
        if (!released)
        {
            dev_context::~dev_context();
            SW_ASSERT_EXPR(ReleaseDC(hWnd, hDC), RELEASE_DC_FAILED);
        }
    }

    window_compatible_dc::window_compatible_dc(HWND hWnd, int width, int height) SW_NOEXCEPT : dev_context(), hWnd(hWnd)
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
