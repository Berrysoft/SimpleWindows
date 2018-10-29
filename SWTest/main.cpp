#include <window.h>

#include "mainwnd.h"
#include <native.h>

using namespace sw;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
    init_app(hInstance, lpCmdLine, nCmdShow);
    mainwnd wnd;
    return current_app.run(wnd);
}
