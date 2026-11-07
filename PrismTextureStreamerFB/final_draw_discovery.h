#pragma once

// Pure, platform-independent screen-source filters.  The old slot-6 RGBA
// profile is deliberately unchanged. Broader exploration is opt-in and never
// authorizes replacement: users must preview/select a physical draw group.
namespace final_draw_discovery
{
    enum class mode_t { strict, broad, wide };

    inline bool rgba8(unsigned format)
    {
        return format == 27 || format == 28 || format == 29;
    }

    inline bool color_family(unsigned format)
    {
        return rgba8(format) || format == 87 || format == 90 ||
            format == 91 || (format >= 70 && format <= 78) ||
            (format >= 97 && format <= 99);
    }

    inline bool same_family(unsigned resource, unsigned view)
    {
        if (view == 0) return true; // UNKNOWN on a default typed SRV
        if (rgba8(resource)) return rgba8(view);
        if (resource == 87 || resource == 90 || resource == 91)
            return view == 87 || view == 91;
        if (resource >= 70 && resource <= 72)
            return view == 71 || view == 72;
        if (resource >= 73 && resource <= 75)
            return view == 74 || view == 75;
        if (resource >= 76 && resource <= 78)
            return view == 77 || view == 78;
        if (resource >= 97 && resource <= 99)
            return view == 98 || view == 99;
        return false;
    }

    inline bool power_of_two(unsigned v) { return v && !(v & (v - 1)); }

    inline bool potential_draw(mode_t mode, unsigned count)
    {
        if (mode == mode_t::strict) return true; // preserve original behavior
        const unsigned maxCount = mode == mode_t::wide ? 4096 : 256;
        return count >= 3 && count <= maxCount && count % 3 == 0;
    }

    inline bool eligible_source(
        mode_t mode, unsigned slot, unsigned width, unsigned height,
        unsigned resourceFormat, unsigned viewFormat,
        unsigned viewDimension, unsigned bindFlags)
    {
        // D3D11_SRV_DIMENSION_TEXTURE2D == 4 and D3D11_BIND_SHADER_RESOURCE == 8.
        if (mode == mode_t::strict)
            return slot == 6 && width == 1024 && height == 1024 &&
                rgba8(resourceFormat) &&
                (viewFormat == 0 || viewFormat == 28 || viewFormat == 29);

        if (slot >= 16 || viewDimension != 4 || !(bindFlags & 8)) return false;
        if (width < 256 || height < 256 || width > 2048 || height > 2048)
            return false;
        if (!power_of_two(width) || !power_of_two(height)) return false;
        if (width > height * 4 || height > width * 4) return false;
        return color_family(resourceFormat) &&
            same_family(resourceFormat, viewFormat);
    }
}
