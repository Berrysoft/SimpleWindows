#include "gdi_ptr.h"

#include "gdi.h"

using namespace std;

namespace sw
{
    bitmap_ptr compatible_bitmap::create() const
    {
        return bitmap_ptr(CreateCompatibleBitmap(dc.handle(), width, height));
    }

    brush_ptr solid_brush::create() const
    {
        return brush_ptr(CreateSolidBrush(color));
    }

    brush_ptr hatch_brush::create() const
    {
        return brush_ptr(CreateHatchBrush(hatch, color));
    }

    font_ptr font::create() const
    {
        return font_ptr(CreateFont(height, width, escapement, orientation, weight, italic, underline, strike_out, charset, out_precis, clip_precis, quality, pitch_family, name.c_str()));
    }

    pen_ptr pen::create() const
    {
        return pen_ptr(CreatePen(style, width, color));
    }
} // namespace sw