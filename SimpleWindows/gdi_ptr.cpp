#include "gdi_ptr.h"

#include "gdi.h"

using namespace std;
using namespace wil;

namespace sw
{
    unique_hbitmap compatible_bitmap::create() const
    {
        return unique_hbitmap(CreateCompatibleBitmap(dc.handle(), width, height));
    }

    unique_hbrush solid_brush::create() const
    {
        return unique_hbrush(CreateSolidBrush(color));
    }

    unique_hbrush hatch_brush::create() const
    {
        return unique_hbrush(CreateHatchBrush(hatch, color));
    }

    unique_hfont font::create() const
    {
        return unique_hfont(CreateFont(height, width, escapement, orientation, weight, italic, underline, strike_out, charset, out_precis, clip_precis, quality, pitch_family, name.c_str()));
    }

    unique_hpen pen::create() const
    {
        return unique_hpen(CreatePen(style, width, color));
    }
} // namespace sw