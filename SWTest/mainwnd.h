#pragma once
#include <button.h>
#include <window.h>

class mainwnd : public sw::window
{
private:
    int x, y;
    POINT prev;
    sw::command_link test_button;

public:
    mainwnd();

private:
    void draw_rect(sw::window&, sw::dev_context& dc);
    void draw_circ(sw::window&, sw::dev_context& dc);
    void draw_cross(sw::window&, sw::dev_context& dc);
    void click_title(sw::window& wnd, const sw::mouse_args& args);

    void begin_drawing(sw::window&, const sw::mouse_args& args);
    void drawing(sw::window& wnd, const sw::mouse_args& args);
    void end_drawing(sw::window& wnd, const sw::mouse_args& args);

    void start_new(sw::window&, const sw::mouse_args& args);

    void wnd_closing(sw::window& wnd, bool& handled);
};
