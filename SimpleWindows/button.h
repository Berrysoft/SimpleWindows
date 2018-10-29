#pragma once
#include "utility.h"

#include "control.h"
#include "event.h"
#include "window.h"
#include <CommCtrl.h>
#include <string>

namespace sw
{
    class button : public common_control
    {
    private:
        friend class window;

    public:
        button(int style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON);

        virtual bool is_default() const;
        virtual void is_default(bool b);

        EVENT_SENDER_E(click, window&, button&)
        EVENT_SENDER_E(dbclick, window&, button&)
    };

    class command_link : public button
    {
    public:
        command_link();

        bool is_default() const override;
        void is_default(bool b) override;

        std::wstring link_note() const;
        void link_note(const std::wstring& note);

        void shield(bool required);
    };
} // namespace sw
