"""
Space Flapper — a Flappy-Bird-style game built with Pygame Zero.

Controls are isolated in clearly-named functions so you can swap
keyboard input for any other source (AI, network, gamepad, etc.)
without touching the game logic.

Project layout expected by Pygame Zero:
    space_flapper/
    ├── space_flapper.py   ← this file
    └── images/
        ├── spaceship.png
        ├── asteroid_top.png
        ├── asteroid_bottom.png
        └── background.png

Run with:
    pgzrun space_flapper.py
Or install pygame-zero and use:
    python -m pgzero space_flapper.py
"""

import pgzrun
import random
# ─────────────────────────────────────────────────────────────────────────────
# WINDOW & WORLD CONSTANTS
# ─────────────────────────────────────────────────────────────────────────────

TITLE  = "Space Flapper"
WIDTH  = 480
HEIGHT = 640

# Physics
GRAVITY       = 0.45   # pixels / frame²  — increase for snappier feel
FLAP_STRENGTH = -8.5   # negative = upward  — more negative = higher jump

# Obstacles
ASTEROID_SPEED    = 3        # pixels per frame
ASTEROID_GAP      = 170      # vertical gap between top/bottom asteroids (px)
ASTEROID_INTERVAL = 90       # frames between new obstacle pairs
ASTEROID_WIDTH    = 80       # must match your asteroid_top/bottom sprite width

# Scrolling background
BG_SCROLL_SPEED = 1          # slower than obstacles for parallax feel

# ─────────────────────────────────────────────────────────────────────────────
# GAME STATE
# ─────────────────────────────────────────────────────────────────────────────

class GameState:
    """Everything that changes during a play session."""
    def __init__(self):
        self.reset()

    def reset(self):
        self.alive       = False   # False = waiting for first flap / dead
        self.started     = False   # True once the player has flapped once
        self.score       = 0
        self.high_score  = getattr(self, "high_score", 0)  # persist between rounds

        # Ship
        self.ship_y      = HEIGHT // 2
        self.velocity    = 0.0

        # Obstacles: list of dicts with keys 'x', 'gap_y', 'scored'
        self.obstacles   = []
        self.frame_count = 0

        # Background scroll offset
        self.bg_offset   = 0

state = GameState()

# ─────────────────────────────────────────────────────────────────────────────
# ACTOR SETUP
# ─────────────────────────────────────────────────────────────────────────────

ship = Actor("spaceship")
ship.x = WIDTH // 4
ship.y = HEIGHT // 2

# ─────────────────────────────────────────────────────────────────────────────
# CONTROL FUNCTIONS  ← swap these out for any input source you like
# ─────────────────────────────────────────────────────────────────────────────

def action_flap():
    """Make the ship thrust upward.  Call from keyboard, AI, network, etc."""
    if not state.started:
        state.started = True
        state.alive   = True
    if state.alive:
        state.velocity = FLAP_STRENGTH

def action_restart():
    """Reset the game to the initial waiting state."""
    state.reset()
    ship.y = HEIGHT // 2

# ─────────────────────────────────────────────────────────────────────────────
# OBSTACLE HELPERS
# ─────────────────────────────────────────────────────────────────────────────

def spawn_obstacle():
    """Add a new asteroid pair at the right edge of the screen."""
    min_gap_y = ASTEROID_GAP // 2 + 40
    max_gap_y = HEIGHT - ASTEROID_GAP // 2 - 40
    gap_y = random.randint(min_gap_y, max_gap_y)
    state.obstacles.append({
        "x":      WIDTH + ASTEROID_WIDTH // 2,
        "gap_y":  gap_y,
        "scored": False,
    })

def get_asteroid_rects(obs):
    """Return (top_rect, bottom_rect) for collision purposes."""
    half_gap  = ASTEROID_GAP // 2
    half_w    = ASTEROID_WIDTH // 2

    top_bottom    = obs["gap_y"] - half_gap        # bottom edge of top asteroid
    bottom_top    = obs["gap_y"] + half_gap        # top edge of bottom asteroid

    top_rect    = Rect(obs["x"] - half_w, 0,           ASTEROID_WIDTH, top_bottom)
    bottom_rect = Rect(obs["x"] - half_w, bottom_top,  ASTEROID_WIDTH, HEIGHT - bottom_top)
    return top_rect, bottom_rect

# ─────────────────────────────────────────────────────────────────────────────
# COLLISION DETECTION
# ─────────────────────────────────────────────────────────────────────────────

def check_collisions():
    """Return True if the ship has hit an asteroid or a screen edge."""
    # Screen bounds
    if ship.top < 0 or ship.bottom > HEIGHT:
        return True

    ship_rect = Rect(ship.left + 6, ship.top + 6,
                     ship.width - 12, ship.height - 12)  # slight shrink for fairness

    for obs in state.obstacles:
        top_rect, bottom_rect = get_asteroid_rects(obs)
        if ship_rect.colliderect(top_rect) or ship_rect.colliderect(bottom_rect):
            return True
    return False

# ─────────────────────────────────────────────────────────────────────────────
# UPDATE  (called every frame by Pygame Zero)
# ─────────────────────────────────────────────────────────────────────────────

def update():
    state.frame_count += 1

    # ── Scrolling background ──────────────────────────────────────────────────
    state.bg_offset = (state.bg_offset + BG_SCROLL_SPEED) % HEIGHT

    if not state.started:
        # Gentle hover animation while waiting
        import math
        ship.y = HEIGHT // 2 + int(6 * math.sin(state.frame_count * 0.05))
        return

    if not state.alive:
        return   # waiting for restart

    # ── Physics ───────────────────────────────────────────────────────────────
    state.velocity += GRAVITY
    state.ship_y   += state.velocity
    ship.y          = int(state.ship_y)

    # ── Spawn obstacles ───────────────────────────────────────────────────────
    if state.frame_count % ASTEROID_INTERVAL == 0:
        spawn_obstacle()

    # ── Move obstacles & score ────────────────────────────────────────────────
    for obs in state.obstacles:
        obs["x"] -= ASTEROID_SPEED
        if not obs["scored"] and obs["x"] < ship.x:
            obs["scored"]  = True
            state.score   += 1
            if state.score > state.high_score:
                state.high_score = state.score

    # Remove off-screen obstacles
    state.obstacles = [o for o in state.obstacles if o["x"] > -ASTEROID_WIDTH]

    # ── Collision ─────────────────────────────────────────────────────────────
    if check_collisions():
        state.alive = False

# ─────────────────────────────────────────────────────────────────────────────
# DRAW  (called every frame by Pygame Zero)
# ─────────────────────────────────────────────────────────────────────────────

def draw_background():
    """Tile the star-field background with vertical scrolling."""
    bg = images.background                      # loaded automatically by pgzero
    bw, bh = bg.get_width(), bg.get_height()

    # We need to cover the window; tile vertically using scroll offset
    y = int(state.bg_offset) - bh
    while y < HEIGHT:
        x = 0
        while x < WIDTH:
            screen.blit(bg, (x, y))
            x += bw
        y += bh

def draw_obstacles():
    """Draw each asteroid pair using the top/bottom sprites."""
    top_sprite    = images.asteroid_top
    bottom_sprite = images.asteroid_bottom
    sprite_h      = top_sprite.get_height()   # both sprites same height

    for obs in state.obstacles:
        half_gap = ASTEROID_GAP // 2
        x        = obs["x"] - ASTEROID_WIDTH // 2

        # Top asteroid: anchor its bottom edge to gap_y - half_gap
        top_y = obs["gap_y"] - half_gap - sprite_h
        screen.blit(top_sprite, (x, top_y))

        # Bottom asteroid: anchor its top edge to gap_y + half_gap
        bottom_y = obs["gap_y"] + half_gap
        screen.blit(bottom_sprite, (x, bottom_y))

def draw_hud():
    """Score display and messages."""
    screen.draw.text(str(state.score),
                     center=(WIDTH // 2, 50),
                     fontsize=52, color="white",
                     shadow=(1, 1), scolor="black")

    if not state.started:
        screen.draw.text("SPACE FLAPPER",
                         center=(WIDTH // 2, HEIGHT // 2 - 80),
                         fontsize=38, color=(100, 200, 255),
                         shadow=(2, 2), scolor=(0, 50, 100))
        screen.draw.text("Press SPACE or click to launch",
                         center=(WIDTH // 2, HEIGHT // 2 + 70),
                         fontsize=20, color=(180, 220, 255))

    elif not state.alive:
        screen.draw.text("SHIP DESTROYED",
                         center=(WIDTH // 2, HEIGHT // 2 - 60),
                         fontsize=34, color=(255, 80, 60),
                         shadow=(2, 2), scolor=(80, 0, 0))
        screen.draw.text(f"Score: {state.score}   Best: {state.high_score}",
                         center=(WIDTH // 2, HEIGHT // 2),
                         fontsize=24, color="white")
        screen.draw.text("Press SPACE or click to retry",
                         center=(WIDTH // 2, HEIGHT // 2 + 50),
                         fontsize=20, color=(180, 220, 255))

def draw():
    draw_background()
    draw_obstacles()
    ship.draw()
    draw_hud()

# ─────────────────────────────────────────────────────────────────────────────
# INPUT HANDLERS  (keyboard / mouse → control functions)
# ─────────────────────────────────────────────────────────────────────────────

def on_key_down(key):
    if key == keys.SPACE or key == keys.UP or key == keys.W:
        if state.alive or not state.started:
            action_flap()
        else:
            action_restart()

def on_mouse_down(button):
    if button == mouse.LEFT:
        if state.alive or not state.started:
            action_flap()
        else:
            action_restart()

# ─────────────────────────────────────────────────────────────────────────────
# ENTRY POINT
# ─────────────────────────────────────────────────────────────────────────────

pgzrun.go()
