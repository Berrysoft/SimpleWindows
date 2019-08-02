#pragma once
#include "utility.h"

#include "native.h"
#include "string_type.h"
#include <memory>
#include <wil/resource.h>

namespace sw
{
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
        wil::unique_hbitmap create() const;
    };

    struct solid_brush
    {
        COLORREF color;
        wil::unique_hbrush create() const;
    };
    struct hatch_brush
    {
        int hatch;
        COLORREF color;
        wil::unique_hbrush create() const;
    };

    struct font
    {
        string_t name;
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
        wil::unique_hfont create() const;
    };

    struct pen
    {
        int style;
        int width;
        COLORREF color;
        wil::unique_hpen create() const;
    };
} // namespace sw
