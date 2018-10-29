#pragma once
#include <window.h>

class aboutwnd : public sw::window
{
public:
    aboutwnd();

private:
    void paint_print(sw::window&, sw::dev_context& dc);
};
