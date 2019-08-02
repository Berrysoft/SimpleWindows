#include "itemdlg.h"

#include <wil/resource.h>
#include <wil/result.h>

using namespace std;
using namespace wil;

namespace sw
{
    HRESULT item_dlg::show()
    {
        return show(nullptr);
    }
    HRESULT item_dlg::show(const window& owner)
    {
        return show(owner.handle());
    }
    HRESULT item_dlg::show(HWND owner)
    {
        return native_dlg->Show(owner);
    }

    void item_dlg::close(HRESULT hr)
    {
        THROW_IF_FAILED(native_dlg->Close(hr));
    }

    void item_dlg::filter(array_view<COMDLG_FILTERSPEC> flt)
    {
        THROW_IF_FAILED(native_dlg->SetFileTypes(static_cast<UINT>(flt.size()), flt.data()));
    }
    void item_dlg::def_ext(const wstring& ext)
    {
        THROW_IF_FAILED(native_dlg->SetDefaultExtension(ext.c_str()));
    }
    UINT item_dlg::filter_index() const
    {
        UINT result;
        THROW_IF_FAILED(native_dlg->GetFileTypeIndex(&result));
        return result;
    }
    void item_dlg::filter_index(UINT index)
    {
        THROW_IF_FAILED(native_dlg->SetFileTypeIndex(index));
    }

    wstring item_dlg::filename() const
    {
        unique_cotaskmem_string name;
        THROW_IF_FAILED(native_dlg->GetFileName(&name));
        wstring result = name.get();
        return result;
    }
    void item_dlg::filename(const wstring& name)
    {
        THROW_IF_FAILED(native_dlg->SetFileName(name.c_str()));
    }
    void item_dlg::filename_label(const wstring& str)
    {
        THROW_IF_FAILED(native_dlg->SetFileNameLabel(str.c_str()));
    }

    FILEOPENDIALOGOPTIONS item_dlg::options() const
    {
        FILEOPENDIALOGOPTIONS result;
        THROW_IF_FAILED(native_dlg->GetOptions(&result));
        return result;
    }
    void item_dlg::options(FILEOPENDIALOGOPTIONS opts)
    {
        THROW_IF_FAILED(native_dlg->SetOptions(opts));
    }

    void item_dlg::title(const wstring& str)
    {
        THROW_IF_FAILED(native_dlg->SetTitle(str.c_str()));
    }

    wstring item_dlg::result_filename() const
    {
        com_ptr<IShellItem> item;
        THROW_IF_FAILED(native_dlg->GetResult(&item));
        unique_cotaskmem_string name;
        THROW_IF_FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
        wstring result = name.get();
        return result;
    }

    open_item_dlg::open_item_dlg() : item_dlg()
    {
        THROW_IF_FAILED(::CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&native_dlg)));
    }

    vector<wstring> open_item_dlg::result_filenames() const
    {
        vector<wstring> result;
        com_ptr<IShellItemArray> items;
        THROW_IF_FAILED(static_cast<IFileOpenDialog*>(native_dlg.get())->GetResults(&items));
        DWORD count;
        THROW_IF_FAILED(items->GetCount(&count));
        for (DWORD i = 0; i < count; i++)
        {
            com_ptr<IShellItem> item;
            THROW_IF_FAILED(items->GetItemAt(i, &item));
            unique_cotaskmem_string name;
            THROW_IF_FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
            result.push_back(name.get());
        }
        return result;
    }

    save_item_dlg::save_item_dlg() : item_dlg()
    {
        THROW_IF_FAILED(::CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&native_dlg)));
    }
} // namespace sw
