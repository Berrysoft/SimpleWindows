#include "native.h"

#include <wil/result.h>

using namespace std;

namespace sw
{
    LRESULT CALLBACK def_callback(const message& msg)
    {
        return DefWindowProc(msg.hWnd, msg.Msg, msg.wParam, msg.lParam);
    }

    LRESULT CALLBACK wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        message msg = { hWnd, Msg, wParam, lParam };
        native_window* wnd = reinterpret_cast<native_window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (wnd)
            return wnd->wnd_proc(msg);
        else
            return def_callback(msg);
    }

    BOOL take_over_message(MSG& msg)
    {
        BOOL bRet = GetMessage(&msg, nullptr, 0, 0);
        if (bRet > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return bRet;
    }

#pragma warning(push)
#pragma warning(disable : 4706)
    int app::run() const
    {
        MSG msg;
        BOOL bRet;
        while (bRet = take_over_message(msg))
        {
            THROW_HR_IF(E_FAIL, bRet < 0);
            if (!wnd_num)
                PostQuitMessage(0);
        }
        return (int)msg.wParam;
    }
#pragma warning(pop)

    int app::run(const native_window& wnd) const
    {
        ShowWindow(wnd.handle(), cshow);
        THROW_IF_WIN32_BOOL_FALSE(UpdateWindow(wnd.handle()));
        return run();
    }

    native_window::~native_window()
    {
        SendMessage(hWnd, WM_CLOSE, 0, 0);
    }

    void native_window::create()
    {
        const window_create_params& params = create_params;
        hWnd = CreateWindowEx(params.ex_style, params.class_name.c_str(), params.window_name.c_str(),
                              params.style, params.x, params.y, params.width, params.height,
                              params.parent ? params.parent->hWnd : nullptr,
                              nullptr, current_app.instance(), nullptr);
        THROW_IF_NULL_ALLOC(hWnd);
    }

    LRESULT native_window::wnd_proc(const message& msg)
    {
        return def_callback(msg);
    }

    app current_app;

    void init_app(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow)
    {
        current_app = app(hInstance, lpCmdLine, nCmdShow);
    }
} // namespace sw
