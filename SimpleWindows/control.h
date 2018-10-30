#pragma once
#include "utility.h"

#include "gdi_ptr.h"
#include "native.h"

namespace sw
{
    class control_container;

    class common_control : public native_window
    {
    private:
        friend class control_container;
        control_container* container;

    public:
        common_control() : native_window() {}

        control_container* parent() const;
        void parent(control_container* c);

        font_obs_ptr ctrl_font() const;
        void ctrl_font(font_ptr&& p);

        std::wstring text() const;
        void text(const std::wstring& s);

        RECT full_rect() const;
        void full_rect(RECT rect, bool redraw = true);

        RECT client_rect() const;
        void client_rect(RECT rect, bool redraw = true);

		void move_center(bool redraw = true);
    };

    class control_container : public common_control
    {
    protected:
        std::map<HWND, common_control*> controls;

    public:
        control_container() : common_control() {}

        void add_control(common_control& c);
        void remove_control(common_control& c);
    };
} // namespace sw
