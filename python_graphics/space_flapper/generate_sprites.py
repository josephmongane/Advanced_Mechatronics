"""
Sprite generator for Space Flapper game.
Creates spaceship and asteroid PNG sprites using Pillow.
"""
from PIL import Image, ImageDraw
import math, os

OUT = "/mnt/user-data/outputs/space_flapper/images"
os.makedirs(OUT, exist_ok=True)

# ── helpers ──────────────────────────────────────────────────────────────────

def new(w, h):
    return Image.new("RGBA", (w, h), (0, 0, 0, 0))

def poly(draw, pts, fill, outline=None, width=1):
    draw.polygon(pts, fill=fill, outline=outline)

# ── SPACESHIP (60×60) ─────────────────────────────────────────────────────────

img = new(60, 60)
d = ImageDraw.Draw(img)

# engine glow
d.ellipse([22, 47, 38, 57], fill=(255, 100, 0, 120))
d.ellipse([25, 49, 35, 55], fill=(255, 180, 0, 200))

# main body
poly(d, [(30,4),(44,40),(30,34),(16,40)], fill=(68,136,255,255))
poly(d, [(30,4),(38,38),(30,34)],        fill=(102,170,255,255))

# cockpit
d.ellipse([23,13,37,31], fill=(136,204,255,255))
d.ellipse([25,15,35,29], fill=(180,230,255,200))

# wings
poly(d, [(16,40),(4,52),(18,44)],  fill=(34,102,204,255))
poly(d, [(44,40),(56,52),(42,44)], fill=(34,102,204,255))
poly(d, [(16,40),(7,50),(14,43)],  fill=(51,119,221,255))
poly(d, [(44,40),(53,50),(46,43)], fill=(51,119,221,255))

# engine nozzle
d.rounded_rectangle([24,40,36,47], radius=2, fill=(51,85,153,255))
d.rounded_rectangle([26,44,34,49], radius=1, fill=(255,136,0,255))

# centre line detail
d.line([(30,10),(30,33)], fill=(180,210,255,120), width=1)

img.save(f"{OUT}/spaceship.png")
print("spaceship.png  ✓")

# ── ASTEROID TOP (80×120, opening at bottom) ──────────────────────────────────

def make_asteroid_half(filename, gap_at_bottom: bool, w=80, h=120):
    """
    A chunky, irregular asteroid chunk.
    gap_at_bottom=True  → the gap faces downward (top obstacle, hangs from top)
    gap_at_bottom=False → the gap faces upward  (bottom obstacle, sits on bottom)
    """
    img = new(w, h)
    d = ImageDraw.Draw(img)

    # Jagged silhouette points (clockwise, gap side is the open edge)
    if gap_at_bottom:
        # rock hangs from top; bottom edge is the passage opening (flat-ish)
        pts = [
            (5, 0), (15, 3), (28, 0), (42, 4), (55, 1), (68, 5), (78, 0),
            (80, 15), (76, 30), (80, 48), (75, 65), (78, 82), (72, 100),
            (80, 115), (80, 120),   # right edge down to gap
            (0,  120),              # gap bottom-left corner
            (0,  110), (4, 95), (0, 78), (5, 60), (2, 42), (6, 25), (0, 12),
        ]
    else:
        # rock sits at bottom; top edge is the passage opening
        pts = [
            (0, 0), (80, 0),        # gap top edge
            (80, 8),  (75, 22), (80, 38), (74, 55), (78, 72),
            (72, 90), (76, 108),(68, 120),
            (55, 116),(40, 120),(28, 115),(15, 120),(5, 117),
            (0, 108), (4, 90), (0, 72), (5, 55), (0, 38), (4, 22), (0, 8),
        ]

    # Base rock colour
    poly(d, pts, fill=(90, 75, 60, 255), outline=(50, 40, 30, 255))

    # Crater / texture patches
    craters = [
        (20, 30, 12, 10), (50, 50, 10, 8), (30, 75, 14, 11),
        (60, 20, 9,  7),  (10, 60, 11, 9), (45, 95, 13, 10),
    ] if gap_at_bottom else [
        (20, 90, 12, 10), (50, 70, 10, 8), (30, 45, 14, 11),
        (60, 100, 9, 7),  (10, 60, 11, 9), (45, 25, 13, 10),
    ]
    for (cx, cy, rx, ry) in craters:
        d.ellipse([cx-rx, cy-ry, cx+rx, cy+ry], fill=(70, 58, 44, 255))
        d.ellipse([cx-rx+2, cy-ry+2, cx+rx-1, cy+ry-1], fill=(105, 88, 68, 200))

    # Bright highlight streaks (meteor scarring)
    if gap_at_bottom:
        streaks = [((18,5),(25,35)), ((50,8),(44,40)), ((65,12),(60,50))]
    else:
        streaks = [((18,115),(25,85)), ((50,112),(44,80)), ((65,108),(60,70))]
    for (s, e) in streaks:
        d.line([s, e], fill=(140, 120, 95, 160), width=2)

    # Subtle orange/red heat glow near gap edge
    glow_y = h - 18 if gap_at_bottom else 8
    glow_box = [5, glow_y, w-5, glow_y+10] if gap_at_bottom else [5, glow_y-10, w-5, glow_y]
    for alpha in [40, 25, 12]:
        gb = [glow_box[0]-2, glow_box[1]-2, glow_box[2]+2, glow_box[3]+2]
        d.ellipse(gb, fill=(255, 80, 0, alpha))

    img.save(f"{OUT}/{filename}")
    print(f"{filename}  ✓")

make_asteroid_half("asteroid_top.png",    gap_at_bottom=True)
make_asteroid_half("asteroid_bottom.png", gap_at_bottom=False)

# ── STARS background tile (400×600) ───────────────────────────────────────────
import random
random.seed(42)

bg = Image.new("RGB", (400, 600), (8, 6, 20))
bd = ImageDraw.Draw(bg)

for _ in range(220):
    x, y = random.randint(0,399), random.randint(0,599)
    r     = random.choice([1,1,1,2])
    br    = random.randint(140, 255)
    col   = (br, br, random.randint(br-30, br))
    bd.ellipse([x-r,y-r,x+r,y+r], fill=col)

# A couple of faint nebula smudges
for cx, cy, rx, ry, col in [(100,150,60,30,(20,10,60)), (300,400,50,25,(10,30,50))]:
    for shrink in range(8, 0, -1):
        alpha = 18 - shrink*2
        bd.ellipse([cx-rx+shrink*5, cy-ry+shrink*2,
                    cx+rx-shrink*5, cy+ry-shrink*2],
                   fill=(*col, max(alpha,0)))  # type: ignore

bg.save(f"{OUT}/background.png")
print("background.png  ✓")

print("\nAll sprites generated successfully!")
