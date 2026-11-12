# Pong — C & ncurses

A terminal-based implementation of the classic Pong game written in C using the `ncurses` library.

The project features a player-controlled paddle, an AI-controlled opponent, a moving ball, and a basic scoring system. It uses terminal rendering and time-based movement to create a real-time game experience.

## Features

- **Terminal-based rendering:** Uses `ncurses` to draw the game inside the terminal.
- **Player-controlled paddle:** Move your paddle vertically using keyboard input.
- **AI opponent:** An automated paddle that tracks the ball's vertical movement.
- **Ball physics:** The ball moves across the screen and bounces off the top, bottom, and paddles.
- **Scoring system:** Keeps track of the player's and AI's scores.
- **Time-based game loop:** Uses `CLOCK_MONOTONIC` to regulate ball movement.
- **Non-blocking input:** Allows the game to continue running without waiting for keyboard input.

## Requirements

- GCC or another C compiler
- Linux or a Unix-like operating system
- `ncurses` development library

### Install dependencies (Ubuntu)

```bash
sudo apt update
sudo apt install build-essential libncurses-dev
```

## Compilation

Compile the source file using:

```bash
gcc main.c -o pong -lncurses
```

## Running the Game

```bash
./pong
```

## Controls

| Key | Action |
|:---:|---|
| `W` | Move paddle up |
| `S` | Move paddle down |
| `Q` | Quit the game |

## How It Works

### Game Loop

The game continuously:

1. Reads keyboard input.
2. Updates the player's paddle position.
3. Checks paddle boundaries.
4. Clears the terminal's virtual screen.
5. Draws the ball and score.
6. Updates the ball position after a specified delay.
7. Updates the AI paddle.
8. Draws both paddles.
9. Refreshes the terminal.

### Ball Movement

The ball's movement is controlled by a two-dimensional velocity vector:

```c
int ball_vector[2] = {-1, -1};
```

The vector represents the ball's vertical and horizontal directions.

The ball's coordinates are updated using:

```c
*ball_y += ball_vector[0];
*ball_x += ball_vector[1];
```

The ball reverses its vertical direction when it hits the top or bottom of the terminal.

It reverses its horizontal direction when it collides with a paddle.

### AI Opponent

The AI paddle is controlled programmatically rather than through keyboard input.

It uses the ball's position and movement to determine how the paddle should move vertically.

## Project Structure

```text
pong/
├── main.c
└── README.md
```

## Learning Objectives

This project explores:

- Pointers and pointer arithmetic
- Terminal-based graphics
- Collision detection
- Basic AI movement logic

## Future Improvements

- Intelligent AI tracking
- Ball speed variation
- Pause and restart functionality

## License

This project is open-source and available for educational purposes.

