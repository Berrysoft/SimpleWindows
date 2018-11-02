#include "mainwnd.h"

#include "aboutwnd.h"
#include <msgbox.h>
#include <sstream>

using namespace std;
using namespace sw;

mainwnd::mainwnd() : window(L"Test"), x(300), y(300)
{
    text(L"SimpleWindowsTest");

    paint(&mainwnd::draw_rect, this);
    paint(&mainwnd::draw_circ, this);
    paint(&mainwnd::draw_cross, this);
    mouse_up(&mainwnd::click_title, this);

    mouse_down(&mainwnd::begin_drawing, this);
    mouse_move(&mainwnd::drawing, this);
    mouse_up(&mainwnd::end_drawing, this);

    mouse_up(&mainwnd::start_new, this);

    closing(&mainwnd::wnd_closing, this);

    add_control(test_button);
    test_button.is_default(true);
    test_button.text(L"ÄãºÃ£¡");
    test_button.link_note(L"Show a message box.");
    test_button.full_rect({ 100, 100, 400, 200 });
    test_button.shield(true);
    test_button.click([](window& wnd, button&) {
        wnd.show_dialog(msgbox{ L"Hello!", L"Hello msgbox", ok_button, information_icon });
    });
}

void mainwnd::draw_rect(window&, dev_context& dc)
{
    dc.set_pen(pen{ PS_SOLID, 3, RGB(192, 192, 0) }.create());
    auto b = dc.set_brush(solid_brush{ RGB(192, 192, 0) }.create());
    dc.draw_rect({ x - 100, y - 100, x + 100, y + 100 });
    dc.set_brush(move(b));
}

void mainwnd::draw_circ(window&, dev_context& dc)
{
    dc.set_pen(pen{ PS_SOLID, 2, RGB(0, 192, 192) }.create());
    auto b = dc.set_brush(hatch_brush{ HS_DIAGCROSS, RGB(0, 192, 192) }.create());
    dc.draw_ellipse({ x - 100, y - 100, x + 100, y + 100 });
    dc.set_brush(move(b));
}

void mainwnd::draw_cross(window&, dev_context& dc)
{
    dc.set_pen(pen{ PS_SOLID, 1, RGB(192, 0, 192) }.create());
    dc.draw_line({ x - 100, y }, { x + 100, y });
    dc.draw_line({ x, y - 100 }, { x, y + 100 });
    dc.draw_polyline(initializer_list<POINT>{ { x, y - 100 }, { x - 100, y + 100 }, { x + 100, y + 100 }, { x, y - 100 } });
}

void mainwnd::click_title(window& wnd, const mouse_args& args)
{
    wostringstream oss;
    oss << L"You click at x = " << args.pos.x << L", y = " << args.pos.y << L'.';
    if (args.button == right_button)
    {
        x = args.pos.x;
        y = args.pos.y;
    }
    wnd.text(oss.str());
    wnd.refresh(false);
}

void mainwnd::begin_drawing(window&, const mouse_args& args)
{
    if (args.button == left_button)
    {
        prev = args.pos;
    }
}

void mainwnd::drawing(window& wnd, const mouse_args& args)
{
    if (args.button & left_button)
    {
        dev_context dc = wnd.get_dc();
        dc.set_pen(pen{ PS_SOLID, 1, RGB(192, 0, 192) }.create());
        dc.draw_line(prev, args.pos);
        prev = args.pos;
    }
}

void mainwnd::end_drawing(window& wnd, const mouse_args& args)
{
    if (args.button == left_button)
    {
        dev_context dc = wnd.get_dc();
        dc.set_pen(pen{ PS_SOLID, 1, RGB(192, 0, 192) }.create());
        dc.draw_line(prev, args.pos);
    }
}

void mainwnd::start_new(window&, const mouse_args& args)
{
    if (args.button == middle_button)
    {
        aboutwnd w;
        show_dialog(w);
    }
}

void mainwnd::wnd_closing(window& wnd, bool& handled)
{
    auto result = wnd.show_dialog(taskdlg{
        L"Closing",
        L"Are you going to close?",
        L"This is a Task Dialog.",
        { taskdlg_information },
        taskdlg_button{
            (taskdlg_yes_button | taskdlg_no_button),
            {},
            false,
            IDNO } });
    handled = result.button_index == IDNO;
}
