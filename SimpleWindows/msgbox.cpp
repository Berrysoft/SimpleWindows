#include "msgbox.h"

using namespace std;

namespace sw
{
    dialog_result msgbox::show() const
    {
        return show(SW_NULLPTR);
    }
    dialog_result msgbox::show_dialog(const window& owner) const
    {
        return show(owner.handle());
    }
    dialog_result msgbox::show(HWND howner) const
    {
        return dialog_result(MessageBox(
            howner,
            text.c_str(),
            caption.c_str(),
            button | icon | def_button | options | (show_help ? 0x4000 : 0)));
    }

    taskdlg_result taskdlg::show() const
    {
        return show(SW_NULLPTR);
    }

    taskdlg_result taskdlg::show_dialog(const window& owner) const
    {
        return show(owner.handle());
    }

    HRESULT CALLBACK taskdlg_callback(HWND hWnd, UINT notification, WPARAM wParam, LPARAM lParam, LONG_PTR data)
    {
        auto* callback = reinterpret_cast<std::function<HRESULT(const message&)>*>(data);
        if (callback && *callback)
        {
            auto& f = *callback;
            message msg = { hWnd, notification, wParam, lParam };
            return f(msg);
        }
        return S_FALSE;
    }
    taskdlg_result taskdlg::show(HWND howner) const
    {
        TASKDIALOGCONFIG config = {};
        config.cbSize = sizeof(TASKDIALOGCONFIG);
        config.hwndParent = howner;
        config.dwFlags = TDF_POSITION_RELATIVE_TO_WINDOW | TDF_SIZE_TO_CONTENT;
        if (button.command_link)
            config.dwFlags |= TDF_USE_COMMAND_LINKS;
        config.dwCommonButtons = button.common_button;
        if (!title.empty())
            config.pszWindowTitle = title.c_str();
        if (icon.common_icon)
        {
            config.pszMainIcon = MAKEINTRESOURCE(-icon.common_icon);
        }
        else
        {
            config.dwFlags |= TDF_USE_HICON_MAIN;
            config.hMainIcon = icon.custom_icon;
        }
        if (!instruction.empty())
            config.pszMainInstruction = instruction.c_str();
        if (!content.empty())
            config.pszContent = content.c_str();
        config.cButtons = static_cast<UINT>(button.custom_button.size());
        if (!button.custom_button.empty())
            config.pButtons = button.custom_button.cbegin();
        config.nDefaultButton = button.def_index;
        config.cRadioButtons = static_cast<UINT>(radio_button.radio_button.size());
        if (!radio_button.radio_button.empty())
            config.pRadioButtons = radio_button.radio_button.cbegin();
        config.nDefaultRadioButton = radio_button.def_index;
        if (!verification.verification.empty())
            config.pszVerificationText = verification.verification.c_str();
        if (verification.def_checked)
            config.dwFlags |= TDF_VERIFICATION_FLAG_CHECKED;
        if (!expanded.expanded.empty())
            config.pszExpandedInformation = expanded.expanded.c_str();
        if (!expanded.expanded_control.empty())
            config.pszExpandedControlText = expanded.expanded_control.c_str();
        if (!expanded.collapsed_control.empty())
            config.pszCollapsedControlText = expanded.collapsed_control.c_str();
        if (footer.icon.common_icon)
        {
            config.pszFooterIcon = MAKEINTRESOURCE(-footer.icon.common_icon);
        }
        else
        {
            config.dwFlags |= TDF_USE_HICON_FOOTER;
            config.hFooterIcon = footer.icon.custom_icon;
        }
        if (!footer.footer.empty())
            config.pszFooter = footer.footer.c_str();
        if (footer.hyperlink)
            config.dwFlags |= TDF_ENABLE_HYPERLINKS;
        if (callback)
        {
            config.pfCallback = taskdlg_callback;
            config.lpCallbackData = reinterpret_cast<LONG_PTR>(&callback);
        }
        config.cxWidth = width;
        int ibutton, iradio;
        BOOL verify;
        HRESULT result = TaskDialogIndirect(&config, &ibutton, &iradio, &verify);
        SW_ASSERT(result == S_OK);
        return { ibutton, iradio, static_cast<bool>(verify) };
    }
} // namespace sw
