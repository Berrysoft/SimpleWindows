#pragma once
#include "utility.h"

#include "control.h"
#include "event.h"
#include "gdi.h"

namespace sw
{
    enum activate_state
    {
        deactivated,
        activated,
        click_activated
    };
    struct activate_args
    {
        activate_state state;
        const native_window* other;
    };

    enum mod_keys
    {
        none_mod = 0x0,
        shift_key = 0x1,
        ctrl_key = 0x2,
        alt_key = 0x4
    };
    struct key_args
    {
        int key;
        mod_keys mods;
    };

    enum mouse_button
    {
        none_button = 0x0,
        left_button = 0x1,
        right_button = 0x2,
        middle_button = 0x4
    };
    struct mouse_args
    {
        POINT pos;
        mouse_button button;
        mod_keys mods;
    };

    enum window_state
    {
        hidden_window,
        shown_window
    };
    enum state_change_cause
    {
        user_zoom,
        parent_zoom,
        other_zoom
    };
    struct state_args
    {
        window_state state;
        state_change_cause cause;
    };

    using pos_args = RECT;

    enum dialog_result
    {
        none_result,
        ok_result,
        cancel_result,
        abort_result,
        retry_result,
        ignore_result,
        yes_result,
        no_result
    };

    class window : public control_container
    {
    protected:
        WNDCLASSEX cls;

    private:
        dialog_result dlgresult;
        bool ismodal;

    public:
        window(const std::wstring& title = {}, int style = WS_OVERLAPPEDWINDOW);
        virtual ~window() override;

        cursor_obs_ptr cursor() const;
        void cursor(cursor_obs_ptr hc);
        icon_obs_ptr icon() const;
        void icon(icon_obs_ptr hi);
        icon_obs_ptr small_icon() const;
        void small_icon(icon_obs_ptr hi);

        void show() const;
        void show(int show) const;
        dialog_result show(const window& owner);
        void refresh(bool redraw = true) const;
        void close() const;

        template <typename Dialog>
        auto show_dialog(Dialog&& dialog) const -> decltype(dialog.show(*this))
        {
            return dialog.show(*this);
        }

        dev_context get_dc();
        dev_context get_buffered_dc(int width, int height);

        SW_CONSTEXPR dialog_result dlg_result() const SW_NOEXCEPT { return dlgresult; }
        void dlg_result(dialog_result result) SW_NOEXCEPT { dlgresult = result; }
        SW_CONSTEXPR bool is_modal() const SW_NOEXCEPT { return ismodal; }

        EVENT_SENDER_E(activate, window&, const activate_args&)
        EVENT_SENDER_E(closing, window&, bool&)
        EVENT_SENDER_E(key_down, window&, const key_args&)
        EVENT_SENDER_E(key_up, window&, const key_args&)
        EVENT_SENDER_E(mouse_down, window&, const mouse_args&)
        EVENT_SENDER_E(mouse_move, window&, const mouse_args&)
        EVENT_SENDER_E(mouse_up, window&, const mouse_args&)
        EVENT_SENDER_E(mouse_dbclick, window&, const mouse_args&)
        EVENT_SENDER_E(paint, window&, dev_context&)
        EVENT_SENDER_E(pos_changed, window&, const pos_args&)
        EVENT_SENDER_E(state_changed, window&, const state_args&)

    protected:
        virtual LRESULT wnd_proc(const message& msg) override;
    };
} // namespace sw
