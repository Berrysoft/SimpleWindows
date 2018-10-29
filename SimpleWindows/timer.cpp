#include "timer.h"

#include <functional>
#include <map>

using namespace std;

namespace sw
{
    map<UINT_PTR, function<void(DWORD)>> callback_map;

    void CALLBACK timer_callback(HWND, UINT, UINT_PTR idTimer, DWORD dwTime)
    {
        auto it = callback_map.find(idTimer);
        if (it != callback_map.end())
        {
            it->second(dwTime);
        }
    }

    void timer::time_span(UINT e)
    {
        elapse = e;
        if (id)
        {
            stop();
            start();
        }
    }

    void timer::start()
    {
        if (!id)
        {
            id = SetTimer(NULL, 0, elapse, timer_callback);
            callback_map.emplace(id, [this](DWORD t) -> void { timer_proc(t); });
        }
    }

    void timer::stop()
    {
        if (id)
        {
            KillTimer(NULL, id);
            callback_map.erase(id);
            id = 0;
        }
    }

} // namespace sw
