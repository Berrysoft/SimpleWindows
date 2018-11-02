#pragma once
#include "utility.h"

#include "array_view.h"
#include "com_ptr.h"
#include "window.h"
#include <ShObjIdl.h>

namespace sw
{
    class item_dlg
    {
    protected:
        com_ptr<IFileDialog> native_dlg;

    public:
        HRESULT show();
        HRESULT show(const window& owner);

    private:
        HRESULT show(HWND owner);

    public:
        void close(HRESULT hr = S_OK);

        void filter(array_view<COMDLG_FILTERSPEC> flt);
        void def_ext(const std::wstring& ext);
        UINT filter_index() const;
        void filter_index(UINT index);

        std::wstring filename() const;
        void filename(const std::wstring& name);
        void filename_label(const std::wstring& str);

        FILEOPENDIALOGOPTIONS options() const;
        void options(FILEOPENDIALOGOPTIONS opts);

        void title(const std::wstring& str);

        std::wstring result_filename() const;
    };

    class open_item_dlg : public item_dlg
    {
    public:
        open_item_dlg();

        std::vector<std::wstring> result_filenames() const;
    };

    class save_item_dlg : public item_dlg
    {
    public:
        save_item_dlg();
    };
} // namespace sw
