"""Rasterise the Hugeicons (MIT, stroke-rounded, 24x24 grid) icons we need into
a single RGBA atlas that the renderer uploads as one D3D11 texture."""

import io
import os
import re
import sys

from PIL import Image, ImageDraw

from svgpathtools import parse_path

CELL = 20          # atlas cell size in pixels (drawn 1:1)
SS = 6             # supersampling factor
STROKE = 1.5       # Hugeicons default stroke width on the 24 grid
GRID = 24.0

ICONS = [
    ('PLAY', 'PlayIcon'),
    ('CODE', 'SourceCodeIcon'),
    ('TERMINAL', 'TerminalIcon'),
    ('SETTINGS', 'Settings01Icon'),
    ('DELETE', 'Delete02Icon'),
    ('CLIPBOARD', 'ClipboardPasteIcon'),
    ('COPY', 'Copy01Icon'),
    ('PLUS', 'PlusSignIcon'),
    ('STOP', 'StopIcon'),
    ('CLOSE', 'Cancel01Icon'),
]


def elements(js):
    """Pull the path/circle primitives out of an icon module."""
    paths = re.findall(r'd:\s*"([^"]+)"', js)
    circles = re.findall(r'\["circle",\s*\{([^}]*)\}', js)
    out_paths = [p for p in paths]
    out_circles = []
    for body in circles:
        cx = float(re.search(r'cx:\s*([-\d.]+)', body).group(1))
        cy = float(re.search(r'cy:\s*([-\d.]+)', body).group(1))
        r = float(re.search(r'r:\s*([-\d.]+)', body).group(1))
        out_circles.append((cx, cy, r))
    return out_paths, out_circles


def flatten(d, steps_per_unit=1.2):
    """Sample an SVG path into polylines (icon fonts are all stroke based)."""
    lines = []
    path = parse_path(d)
    for sub in path.continuous_subpaths():
        points = []
        for seg in sub:
            count = max(2, int(seg.length() * steps_per_unit) + 1)
            for i in range(count):
                pt = seg.point(i / (count - 1))
                points.append((pt.real, pt.imag))
        if len(points) > 1:
            lines.append(points)
    return lines


def render(name):
    js = io.open(os.path.join(ICON_DIR, '%s.js' % name), encoding='utf-8').read()
    paths, circles = elements(js)

    size = CELL * SS
    img = Image.new('L', (size, size), 0)
    draw = ImageDraw.Draw(img)
    k = size / GRID
    width = max(1, int(round(STROKE * k)))

    for d in paths:
        for line in flatten(d):
            pts = [(x * k, y * k) for x, y in line]
            draw.line(pts, fill=255, width=width, joint='curve')
            # round caps, matching the stroke-linecap="round" attribute
            r = width / 2.0
            for (x, y) in (pts[0], pts[-1]):
                draw.ellipse([x - r, y - r, x + r, y + r], fill=255)

    for (cx, cy, r) in circles:
        box = [(cx - r) * k, (cy - r) * k, (cx + r) * k, (cy + r) * k]
        draw.ellipse(box, outline=255, width=width)

    return img.resize((CELL, CELL), Image.LANCZOS)


ICON_DIR = '.tmp_icons'


def main():
    atlas = Image.new('RGBA', (CELL * len(ICONS), CELL), (0, 0, 0, 0))
    for index, (key, name) in enumerate(ICONS):
        icon = render(name)
        atlas.paste(Image.merge('RGBA', (icon, icon, icon, icon)), (index * CELL, 0))

    atlas.save(os.path.join(ICON_DIR, 'hugeicons_atlas.png'))

    raw = atlas.tobytes()
    lines = []
    for i in range(0, len(raw), 16):
        lines.append('    ' + ', '.join('0x%02X' % b for b in raw[i:i + 16]) + ',')
    body = '\n'.join(lines)

    header = '''#pragma once

// Generated from the Hugeicons free stroke-rounded set (MIT licence), 24x24
// grid, 1.5 stroke. Rebuild with .tmp_icons/build_atlas.py.
namespace module::core::graphics::ui
{
    enum icon_id
    {
        ICON_PLAY,
        ICON_CODE,
        ICON_TERMINAL,
        ICON_SETTINGS,
        ICON_DELETE,
        ICON_CLIPBOARD,
        ICON_COPY,
        ICON_PLUS,
        ICON_STOP,
        ICON_CLOSE,
        ICON_COUNT
    };

    inline constexpr int icon_atlas_cell = %d;
    inline constexpr int icon_atlas_width = %d;
    inline constexpr int icon_atlas_height = %d;

    inline const unsigned char icon_atlas_rgba[] = {
%s
    };
}
''' % (CELL, CELL * len(ICONS), CELL, body)

    io.open('Module/Renderer/ui/icons_atlas.h', 'w', encoding='utf-8', newline='\n').write(header)
    print('atlas %dx%d, %d icons, header %d bytes' % (CELL * len(ICONS), CELL, len(ICONS), len(header)))


main()
