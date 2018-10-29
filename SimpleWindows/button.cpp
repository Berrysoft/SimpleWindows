#include "button.h"

using namespace std;

namespace sw
{
    bool button_is_def(HWND hWnd, int def_style)
    {
        auto style = GetWindowLongPtr(hWnd, GWL_STYLE);
        return (style & def_style) == def_style;
    }
    void button_is_def(HWND hWnd, int def_style, bool def)
    {
        auto style = GetWindowLongPtr(hWnd, GWL_STYLE);
        if (def)
            style |= def_style;
        else if ((style & def_style) == def_style)
        {
            style--;
        }
        SetWindowLongPtr(hWnd, GWL_STYLE, style);
    }

    button::button(int style) : common_control()
    {
        create_params.class_name = TEXT("BUTTON");
        create_params.style = style;
        create_params.x = 0;
        create_params.y = 0;
        create_params.width = 100;
        create_params.height = 50;
    }

    bool button::is_default() const
    {
        return button_is_def(hWnd, BS_DEFPUSHBUTTON);
    }

    void button::is_default(bool b)
    {
        button_is_def(hWnd, BS_DEFPUSHBUTTON, b);
    }

    command_link::command_link() : button(WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_COMMANDLINK)
    {
    }

    bool command_link::is_default() const
    {
        return button_is_def(hWnd, BS_DEFCOMMANDLINK);
    }

    void command_link::is_default(bool b)
    {
        button_is_def(hWnd, BS_DEFCOMMANDLINK, b);
    }

    std::wstring command_link::link_note() const
    {
        std::size_t count = Button_GetNoteLength(hWnd);
        wstring result(count, L'\0');
        Button_GetNote(hWnd, &result.front(), count);
        return result;
    }
    void command_link::link_note(const std::wstring& note)
    {
        Button_SetNote(hWnd, note.c_str());
    }

    void command_link::shield(bool required)
    {
        Button_SetElevationRequiredState(hWnd, required);
    }
} // namespace sw
