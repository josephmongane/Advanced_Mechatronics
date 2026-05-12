# Space Flapper 🚀

A Flappy Bird-style game where you pilot a spaceship through asteroid fields.

## Setup

1. **Install dependencies**
   ```
   pip install pygame pgzero
   ```

2. **Run the game**
   ```
   pgzrun space_flapper.py
   ```
   Or alternatively:
   ```
   python -m pgzero space_flapper.py
   ```

## Controls

| Input          | Action         |
|----------------|----------------|
| `SPACE` / `↑` / `W` | Flap / Thrust |
| Left click     | Flap / Thrust  |
| Any of the above (after death) | Restart |

## Project Structure

```
space_flapper/
├── space_flapper.py       ← Main game script
├── README.md
└── images/
    ├── spaceship.png      ← Player ship sprite
    ├── asteroid_top.png   ← Top obstacle sprite
    ├── asteroid_bottom.png← Bottom obstacle sprite
    └── background.png     ← Scrolling starfield
```

## Modifying the Game

All tunable values live at the top of `space_flapper.py`:

| Constant            | Effect                                      |
|---------------------|---------------------------------------------|
| `GRAVITY`           | How fast the ship falls                     |
| `FLAP_STRENGTH`     | How high each thrust launches the ship      |
| `ASTEROID_SPEED`    | Horizontal scroll speed                     |
| `ASTEROID_GAP`      | Vertical gap between asteroid pairs         |
| `ASTEROID_INTERVAL` | Frames between spawns (lower = more dense)  |
| `BG_SCROLL_SPEED`   | Parallax background scroll speed            |

## Swapping the Input Source

The game logic only ever calls two functions:

- **`action_flap()`** — thrusts the ship upward
- **`action_restart()`** — resets the game

The keyboard/mouse handlers at the bottom of the file are the only place
that calls these. To drive the ship from an AI agent, a network socket,
or a gamepad, just call `action_flap()` and `action_restart()` from your
own code — no other changes needed.

Example AI hook (add to the `update()` function):
```python
# Simple "flap when falling too fast" bot
if state.alive and state.velocity > 3:
    action_flap()
```
