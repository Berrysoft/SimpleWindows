#include "control.h"

using namespace std;

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
            {
                c->add_control(*this);
            }
            else
                c->remove_control(*this);
            container = c;
        }
    }

    font_obs_ptr common_control::ctrl_font() const
    {
        return font_obs_ptr(SendMessage(hWnd, WM_GETFONT, 0, 0));
    }

    void common_control::ctrl_font(font_ptr&& p)
    {
        SendMessage(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(p.release()), TRUE);
    }

    wstring common_control::text() const
    {
        int count = GetWindowTextLength(hWnd);
        wstring result(count, L'\0');
        GetWindowText(hWnd, &result.front(), count);
        return result;
    }

    void common_control::text(const wstring& s)
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
        SW_ASSERT(MoveWindow(hWnd, rect.left, rect.top, rect.right, rect.bottom, redraw));
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
        RECT nfull =
            {
                full.left - client.left + rect.left,
                full.top - client.top + rect.top,
                full.right - client.right + rect.right,
                full.bottom - client.bottom + client.bottom
            };
        full_rect(nfull, redraw);
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
        SetParent(c.handle(), SW_NULLPTR);
        controls.erase(c.handle());
    }
} // namespace sw
