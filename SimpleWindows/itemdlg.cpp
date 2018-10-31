#include "itemdlg.h"

#ifndef SW_ASSERT_HRESULT
#define SW_ASSERT_HRESULT(exp) SW_ASSERT((exp) == S_OK)
#endif // !SW_ASSERT_HRESULT

using namespace std;

namespace sw
{
    HRESULT item_dlg::show()
    {
        return show(SW_NULLPTR);
    }
    HRESULT item_dlg::show_dialog(const window& owner)
    {
        return show(owner.handle());
    }
    HRESULT item_dlg::show(HWND owner)
    {
        return native_dlg->Show(owner);
    }

    void item_dlg::close(HRESULT hr)
    {
        SW_ASSERT_HRESULT(native_dlg->Close(hr));
    }

    void item_dlg::filter(array_view<COMDLG_FILTERSPEC> flt)
    {
        SW_ASSERT_HRESULT(native_dlg->SetFileTypes(static_cast<UINT>(flt.size()), flt.cbegin()));
    }
    void item_dlg::def_ext(const wstring& ext)
    {
        SW_ASSERT_HRESULT(native_dlg->SetDefaultExtension(ext.c_str()));
    }
    UINT item_dlg::filter_index() const
    {
        UINT result;
        SW_ASSERT_HRESULT(native_dlg->GetFileTypeIndex(&result));
        return result;
    }
    void item_dlg::filter_index(UINT index)
    {
        SW_ASSERT_HRESULT(native_dlg->SetFileTypeIndex(index));
    }

    wstring item_dlg::filename() const
    {
        co_task_mem_ptr<WCHAR> name;
        SW_ASSERT_HRESULT(native_dlg->GetFileName(&name));
        wstring result = name.get();
        return result;
    }
    void item_dlg::filename(const wstring& name)
    {
        SW_ASSERT_HRESULT(native_dlg->SetFileName(name.c_str()));
    }
    void item_dlg::filename_label(const wstring& str)
    {
        SW_ASSERT_HRESULT(native_dlg->SetFileNameLabel(str.c_str()));
    }

    FILEOPENDIALOGOPTIONS item_dlg::options() const
    {
        FILEOPENDIALOGOPTIONS result;
        SW_ASSERT_HRESULT(native_dlg->GetOptions(&result));
        return result;
    }
    void item_dlg::options(FILEOPENDIALOGOPTIONS opts)
    {
        SW_ASSERT_HRESULT(native_dlg->SetOptions(opts));
    }

    void item_dlg::title(const wstring& str)
    {
        SW_ASSERT_HRESULT(native_dlg->SetTitle(str.c_str()));
    }

    wstring item_dlg::result_filename() const
    {
        com_ptr<IShellItem> item;
        SW_ASSERT_HRESULT(native_dlg->GetResult(&item));
        co_task_mem_ptr<WCHAR> name;
        SW_ASSERT_HRESULT(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
        wstring result = name.get();
        return result;
    }

    open_item_dlg::open_item_dlg() : item_dlg()
    {
        SW_ASSERT_HRESULT(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&native_dlg)));
    }

    vector<wstring> open_item_dlg::result_filenames() const
    {
        vector<wstring> result;
        com_ptr<IShellItemArray> items;
        SW_ASSERT_HRESULT(static_cast<IFileOpenDialog*>(native_dlg.get())->GetResults(&items));
        DWORD count;
        SW_ASSERT_HRESULT(items->GetCount(&count));
        for (DWORD i = 0; i < count; i++)
        {
            com_ptr<IShellItem> item;
            SW_ASSERT_HRESULT(items->GetItemAt(i, &item));
            co_task_mem_ptr<WCHAR> name;
            SW_ASSERT_HRESULT(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
            result.push_back(name.get());
        }
        return result;
    }

    save_item_dlg::save_item_dlg() : item_dlg()
    {
        SW_ASSERT_HRESULT(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&native_dlg)));
    }
} // namespace sw
