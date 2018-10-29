#pragma once
#include "utility.h"

#include "event.h"

namespace sw
{
    class timer
    {
    private:
        UINT_PTR id;
        UINT elapse;

    public:
        timer(UINT elapse = 1) : id(0), elapse(elapse) {}
        ~timer() { stop(); }

        constexpr operator bool() { return id; }
        constexpr UINT time_span() const { return elapse; }
        void time_span(UINT e);

        void start();
        void stop();

    protected:
        virtual void timer_proc(DWORD dwTime) { on_elapsed(*this, dwTime); }
        friend void CALLBACK timer_callback(HWND hWnd, UINT message, UINT_PTR idTimer, DWORD dwTime);

        EVENT_SENDER_E(elapsed, timer&, DWORD) //计时器触发事件
    };
} // namespace sw
