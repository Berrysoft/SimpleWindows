#pragma once
#include "utility.h"

#include "native.h"
#include "sw_resources.h"
#include <string>

namespace sw
{
    template <typename T>
    struct gdi_delete
    {
        SW_CONSTEXPR gdi_delete() SW_NOEXCEPT = default;
        void operator()(T* ptr) const
        {
            SW_ASSERT_EXPR(DeleteObject(ptr), DELETE_OBJECT_FAILED);
        }
    };

    template <typename T>
    using gdi_ptr = std::unique_ptr<T, gdi_delete<T>>;

    using bitmap_ptr = gdi_ptr<HBITMAP__>;
    using brush_ptr = gdi_ptr<HBRUSH__>;
    using font_ptr = gdi_ptr<HFONT__>;
    using pen_ptr = gdi_ptr<HPEN__>;

    template <typename T>
    using observer_ptr = T*;

    using font_obs_ptr = observer_ptr<HFONT__>;
    using icon_obs_ptr = observer_ptr<HICON__>;
    using cursor_obs_ptr = icon_obs_ptr;

    class dev_context;

    struct compatible_bitmap
    {
        const dev_context& dc;
        int width;
        int height;
        bitmap_ptr create() const;
    };
    struct window_compatible_bitmap
    {
        const native_window& wnd;
        int width;
        int height;
        bitmap_ptr create() const;
    };

    struct solid_brush
    {
        COLORREF color;
        brush_ptr create() const;
    };
    struct hatch_brush
    {
        int hatch;
        COLORREF color;
        brush_ptr create() const;
    };

    struct font
    {
        std::wstring name;
        int height;
        int width;
        int escapement;
        int orientation;
        int weight;
        bool italic;
        bool underline;
        bool strike_out;
        unsigned char charset;
        unsigned char out_precis;
        unsigned char clip_precis;
        unsigned char quality;
        unsigned char pitch_family;
        font_ptr create() const;
    };

    struct pen
    {
        int style;
        int width;
        COLORREF color;
        pen_ptr create() const;
    };
} // namespace sw
