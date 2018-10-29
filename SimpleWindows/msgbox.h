#pragma once
#include "utility.h"

#include "window.h"
#include <CommCtrl.h>
#include <functional>
#include <string>

namespace sw
{
    enum msgbox_button
    {
        ok_button,
        ok_cancel_button,
        abort_retry_ignore_button,
        yes_no_cancel_button,
        yes_no_button,
        retry_cancel_button
    };
    enum msgbox_icon
    {
        none_icon = 0x0,
        error_icon = 0x10,
        question_icon = 0x20,
        warning_icon = 0x30,
        information_icon = 0x40
    };
    enum msgbox_def_button
    {
        def_button_1 = 0x0,
        def_button_2 = 0x100,
        def_button_3 = 0x200
    };
    enum msgbox_options
    {
        msgbox_def = 0x0,
        msgbox_def_desktop = 0x20000,
        msgbox_right_align = 0x80000,
        msgbox_rtl_reading = 0x100000,
        msgbox_service = 0x200000
    };
    struct msgbox
    {
        std::wstring text;
        std::wstring caption;
        msgbox_button button;
        msgbox_icon icon;
        msgbox_def_button def_button;
        msgbox_options options;
        bool show_help;

    public:
        dialog_result show() const;
        dialog_result show_dialog(const window& owner) const;

    private:
        dialog_result show(HWND howner) const;
    };

    enum taskdlg_common_button
    {
        taskdlg_ok_button = 0x1,
        taskdlg_yes_button = 0x2,
        taskdlg_no_button = 0x4,
        taskdlg_cancel_button = 0x8,
        taskdlg_retry_button = 0x10,
        taskdlg_close_button = 0x20
    };
    enum taskdlg_common_icon
    {
        taskdlg_custom,
        taskdlg_warning,
        taskdlg_error,
        taskdlg_information,
        taskdlg_shield
    };
    struct taskdlg_icon
    {
        taskdlg_common_icon common_icon;
        icon_obs_ptr custom_icon;
    };
    struct taskdlg_button
    {
        int common_button;
        std::vector<TASKDIALOG_BUTTON> custom_button;
        bool command_link;
        int def_index;
    };
    struct taskdlg_radio_button
    {
        std::vector<TASKDIALOG_BUTTON> radio_button;
        int def_index;
    };
    struct taskdlg_verification
    {
        std::wstring verification;
        bool def_checked;
    };
    struct taskdlg_expanded
    {
        std::wstring expanded;
        std::wstring expanded_control;
        std::wstring collapsed_control;
    };
    struct taskdlg_footer
    {
        std::wstring footer;
        taskdlg_icon icon;
        bool hyperlink;
    };
    struct taskdlg_result
    {
        int button_index;
        int radio_index;
        bool verify;
    };
    struct taskdlg
    {
        std::wstring title;
        std::wstring instruction;
        std::wstring content;
        taskdlg_icon icon;
        taskdlg_button button;
        taskdlg_radio_button radio_button;
        taskdlg_verification verification;
        taskdlg_expanded expanded;
        taskdlg_footer footer;
        std::function<HRESULT(const message&)> callback;
        UINT width;

    public:
        taskdlg_result show() const;
        taskdlg_result show_dialog(const window& owner) const;

    private:
        taskdlg_result show(HWND howner) const;
    };
} // namespace sw
