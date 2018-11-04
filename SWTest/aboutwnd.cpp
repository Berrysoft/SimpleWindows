#include "aboutwnd.h"

using namespace std;
using namespace sw;

aboutwnd::aboutwnd() : window(L"About", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME)
{
    text(L"About SimpleWindowsTest...");

    full_rect({ 200, 200, 800, 600 });
    paint(&aboutwnd::paint_print, this);
}

void aboutwnd::paint_print(window&, dev_context& dc)
{
    dc.set_font(font{ L"Segoe UI", 50 });
    dc.draw_string({ 100, 100 }, L"A SimpleWindows test.");
}
