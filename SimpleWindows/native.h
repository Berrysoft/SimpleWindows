#pragma once
#include "utility.h"

#include "string_type.h"

namespace sw
{
    LRESULT CALLBACK wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    BOOL take_over_message(MSG& msg);

    struct message
    {
        HWND hWnd;
        UINT Msg;
        WPARAM wParam;
        LPARAM lParam;
    };

    class native_window;

    class app
    {
    private:
        HINSTANCE hInstance;
        string_t cline;
        int cshow;

    public:
        app() = default;
        app(HINSTANCE hInstance, LPTSTR lpCmdLine, int nCmdShow) : hInstance(hInstance), cline(lpCmdLine), cshow(nCmdShow), wnd_num(0) {}

        int run() const;
        int run(const native_window& wnd) const;

        SW_CONSTEXPR HINSTANCE instance() const SW_NOEXCEPT { return hInstance; }
        SW_CONSTEXPR const string_t& cmd_line() const SW_NOEXCEPT { return cline; }
        SW_CONSTEXPR int cmd_show() const SW_NOEXCEPT { return cshow; }

    private:
        int wnd_num;
        friend class window;
    };

    struct window_create_params
    {
        string_t class_name;
        string_t window_name;
        DWORD style;
        DWORD ex_style;
        int x;
        int y;
        int width;
        int height;
        native_window* parent;
    };

    class native_window
    {
    protected:
        HWND hWnd;

    public:
        native_window() = default;
        virtual ~native_window();

        SW_CONSTEXPR HWND handle() const SW_NOEXCEPT { return hWnd; }

    protected:
        window_create_params create_params;
        void create();

        virtual LRESULT wnd_proc(const message& msg);

    private:
        friend LRESULT CALLBACK wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    };

    extern sw::app current_app;

    void init_app(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow);
} // namespace sw
