#include "window.h"

#include "button.h"
#include "gdi_internal.h"
#include <sstream>
#include <windowsx.h>

using namespace std;

namespace sw
{
    map<int, string_t> classes;

    window::window(const string_t& name, int style) : control_container()
    {
        current_app.wnd_num++;
        cls.cbSize = sizeof(WNDCLASSEX);
        cls.lpfnWndProc = wnd_callback;
        cls.cbClsExtra = sizeof(native_window*);
        cls.hCursor = LoadCursor(nullptr, IDC_ARROW);
        cls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        cls.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
        auto it = classes.find(style);
        if (it == classes.end())
        {
            cls.hInstance = current_app.hInstance;
            string_t tname = name;
            if (tname.empty())
            {
                wostringstream oss;
                oss << hex << uppercase << style;
                tname = oss.str();
            }
            string_t clsname = TEXT("Windows.") + tname;
            cls.lpszClassName = clsname.c_str();
            SW_ASSERT_EXPR(RegisterClassEx(&cls), CREATE_WND_FAILED);
            classes.emplace(style, clsname);
            create_params.class_name = clsname;
        }
        else
        {
            create_params.class_name = it->second;
        }
        create_params.window_name = name;
        create_params.style = style;
        create_params.x = CW_USEDEFAULT;
        create_params.y = CW_USEDEFAULT;
        create_params.width = CW_USEDEFAULT;
        create_params.height = CW_USEDEFAULT;
        create();
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }

    window::~window()
    {
        native_window::~native_window();
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
    }

    cursor_obs_ptr window::cursor() const
    {
        return reinterpret_cast<cursor_obs_ptr>(GetClassLongPtr(hWnd, GCLP_HCURSOR));
    }

    void window::cursor(cursor_obs_ptr hc)
    {
        SetClassLongPtr(hWnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(hc));
    }

    icon_obs_ptr window::icon() const
    {
        return reinterpret_cast<icon_obs_ptr>(GetClassLongPtr(hWnd, GCLP_HICON));
    }

    void window::icon(icon_obs_ptr hi)
    {
        SetClassLongPtr(hWnd, GCLP_HICON, reinterpret_cast<LONG_PTR>(hi));
    }

    icon_obs_ptr window::small_icon() const
    {
        return reinterpret_cast<icon_obs_ptr>(GetClassLongPtr(hWnd, GCLP_HICONSM));
    }

    void window::small_icon(icon_obs_ptr hi)
    {
        SetClassLongPtr(hWnd, GCLP_HICONSM, reinterpret_cast<LONG_PTR>(hi));
    }

    void window::show() const
    {
        ShowWindow(hWnd, SW_SHOW);
        SW_ASSERT(BringWindowToTop(hWnd));
    }

    void window::show(int show) const
    {
        ShowWindow(hWnd, show);
    }

    dialog_result window::show(const window& owner)
    {
        ismodal = true;
        show();
        HWND ph = owner.hWnd;
        while (ph)
        {
            EnableWindow(ph, FALSE);
            ph = GetParent(ph);
        }
        while (ismodal)
        {
            MSG msg;
            BOOL bRet = take_over_message(msg);
            SW_ASSERT_EXPR(bRet >= 0, MESSAGE_LOOP_ERROR);
            if (!bRet)
                break;
        }
        ph = owner.hWnd;
        while (ph)
        {
            EnableWindow(ph, TRUE);
            ph = GetParent(ph);
        }
        owner.show();
        return dlgresult;
    }

    void window::refresh(bool redraw) const
    {
        SW_ASSERT(InvalidateRect(hWnd, nullptr, redraw));
    }

    void window::close() const
    {
        SendMessage(hWnd, WM_CLOSE, 0, 0);
    }

    dev_context window::get_dc()
    {
        return window_dc(hWnd);
    }

    dev_context window::get_buffered_dc(int width, int height)
    {
        return window_compatible_dc(hWnd, width, height);
    }

    inline mouse_button get_mouse_button(WPARAM wParam)
    {
        int button = 0;
        if (wParam & MK_LBUTTON)
        {
            button |= left_button;
        }
        if (wParam & MK_RBUTTON)
        {
            button |= right_button;
        }
        if (wParam & MK_MBUTTON)
        {
            button |= middle_button;
        }
        return static_cast<mouse_button>(button);
    }
    inline mod_keys get_mod_keys()
    {
        int keys = 0;
        if (GetKeyState(VK_SHIFT) < 0)
        {
            keys |= shift_key;
        }
        if (GetKeyState(VK_CONTROL) < 0)
        {
            keys |= ctrl_key;
        }
        if (GetKeyState(VK_MENU) < 0)
        {
            keys |= alt_key;
        }
        return static_cast<mod_keys>(keys);
    }

    inline mouse_args get_mouse_args(const message& msg)
    {
        return {
            GET_X_LPARAM(msg.lParam),
            GET_Y_LPARAM(msg.lParam),
            get_mouse_button(msg.wParam),
            get_mod_keys()
        };
    }
    inline mouse_args get_mouse_args(const message& msg, UINT base)
    {
        return {
            GET_X_LPARAM(msg.lParam),
            GET_Y_LPARAM(msg.lParam),
            static_cast<mouse_button>(1 << ((msg.Msg - base) / 3)),
            get_mod_keys()
        };
    }
    inline key_args get_key_args(const message& msg)
    {
        return { (int)msg.wParam, get_mod_keys() };
    }

    LRESULT window::wnd_proc(const message& msg)
    {
        switch (msg.Msg)
        {
        case WM_ACTIVATE:
        {
            activate_args args = { static_cast<activate_state>(msg.wParam) };
            HWND other = reinterpret_cast<HWND>(msg.lParam);
            native_window* wnd = reinterpret_cast<native_window*>(GetWindowLongPtr(other, GWLP_USERDATA));
            args.other = wnd;
            on_activate(*this, args);
            break;
        }
        case WM_SHOWWINDOW:
        {
            state_args args = { msg.wParam ? shown_window : hidden_window };
            switch (msg.lParam)
            {
            case 0:
                args.cause = user_zoom;
                break;
            case SW_PARENTCLOSING:
            case SW_PARENTOPENING:
                args.cause = parent_zoom;
                break;
            case SW_OTHERZOOM:
            case SW_OTHERUNZOOM:
                args.cause = other_zoom;
                break;
            }
            on_state_changed(*this, args);
            break;
        }
        case WM_PAINT:
        {
            window_paint_dc dc(hWnd);
            on_paint(*this, dc);
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        {
            mouse_args args = get_mouse_args(msg, WM_LBUTTONDOWN);
            on_mouse_down(*this, args);
            break;
        }
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            mouse_args args = get_mouse_args(msg, WM_LBUTTONUP);
            on_mouse_up(*this, args);
            break;
        }
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        {
            mouse_args args = get_mouse_args(msg, WM_LBUTTONDBLCLK);
            on_mouse_dbclick(*this, args);
            break;
        }
        case WM_MOUSEMOVE:
        {
            mouse_args args = get_mouse_args(msg);
            on_mouse_move(*this, args);
            break;
        }
        case WM_KEYDOWN:
        {
            key_args args = get_key_args(msg);
            on_key_down(*this, args);
            break;
        }
        case WM_KEYUP:
        {
            key_args args = get_key_args(msg);
            on_key_up(*this, args);
            break;
        }
        case WM_SIZE:
		{
            SIZE args = { LOWORD(msg.lParam), HIWORD(msg.lParam) };
            on_size_changed(*this, args);
            break;
		}
        case WM_COMMAND:
        {
            switch (HIWORD(msg.wParam))
            {
            case BN_CLICKED:
            {
                HWND h = reinterpret_cast<HWND>(msg.lParam);
                button* btn = static_cast<button*>(controls[h]);
                btn->on_click(*this, *btn);
                break;
            }
            case BN_DOUBLECLICKED:
            {
                HWND h = reinterpret_cast<HWND>(msg.lParam);
                button* btn = static_cast<button*>(controls[h]);
                btn->on_dbclick(*this, *btn);
                break;
            }
            }
            break;
        }
        case WM_CLOSE:
        {
            bool handled = false;
            on_closing(*this, handled);
            if (handled)
            {
                return 0;
            }
            ismodal = false;
            break;
        }
        case WM_DESTROY:
            current_app.wnd_num--;
            break;
        }
        return native_window::wnd_proc(msg);
    }
} // namespace sw
