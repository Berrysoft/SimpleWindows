#include "control.h"

using namespace std;
using namespace wil;

namespace sw
{
    control_container* common_control::parent() const
    {
        return container;
    }
    void common_control::parent(control_container* c)
    {
        if (container != c)
        {
            if (c)
                c->add_control(*this);
            else
                c->remove_control(*this);
            container = c;
        }
    }

    font_obs_ptr common_control::ctrl_font() const
    {
        return font_obs_ptr(SendMessage(hWnd, WM_GETFONT, 0, 0));
    }

    void common_control::ctrl_font(unique_hfont&& p)
    {
        SendMessage(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(p.release()), TRUE);
    }

    string_t common_control::text() const
    {
        int count = GetWindowTextLength(hWnd);
        string_t result(count, L'\0');
        GetWindowText(hWnd, result.data(), count);
        return result;
    }

    void common_control::text(const string_t& s)
    {
        SW_ASSERT(SetWindowText(hWnd, s.c_str()));
    }

    RECT common_control::full_rect() const
    {
        RECT result;
        SW_ASSERT(GetWindowRect(hWnd, &result));
        return result;
    }

    void common_control::full_rect(RECT rect, bool redraw)
    {
        SW_ASSERT(MoveWindow(hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, redraw));
    }

    RECT common_control::client_rect() const
    {
        RECT result;
        SW_ASSERT(GetClientRect(hWnd, &result));
        return result;
    }

    void common_control::client_rect(RECT rect, bool redraw)
    {
        RECT full = full_rect();
        RECT client = client_rect();
        int woff = full.right - full.left - (client.right - client.left);
        int hoff = full.bottom - full.top - (client.bottom - client.top);
        RECT nfull = {
            full.left,
            full.top,
            rect.right - rect.left + woff + full.left,
            rect.bottom - rect.top + hoff + full.top
        };
        full_rect(nfull, redraw);
    }

    void common_control::move_center(bool redraw)
    {
        HWND h;
        if (container)
        {
            h = container->handle();
        }
        else
        {
            h = GetDesktopWindow();
        }
        RECT rp;
        GetWindowRect(h, &rp);
        RECT rs = full_rect();
        int width = rs.right - rs.left;
        int height = rs.bottom - rs.top;
        int left = (rp.right - rp.left - width) / 2;
        int top = (rp.bottom - rp.top - height) / 2;
        full_rect({ left, top, left + width, top + height }, redraw);
    }

    void control_container::add_control(common_control& c)
    {
        if (c.parent())
            c.parent()->remove_control(c);
        if (!c.handle())
        {
            c.create_params.parent = this;
            c.create();
        }
        SetParent(c.handle(), hWnd);
        controls.emplace(c.handle(), &c);
    }
    void control_container::remove_control(common_control& c)
    {
        SetParent(c.handle(), nullptr);
        controls.erase(c.handle());
    }
} // namespace sw
