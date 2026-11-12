#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

void move_ball(int *ball_y, int *ball_x);

int ball_vector[2] = {1, 1};

int main() {
    initscr();              // starts ncurses, enters alternate screen, inializes stdscr
    cbreak();               // keypresses reach program instantly, no ENTER needed
    noecho();               // keys you press don't print on screen
    curs_set(0);            // hides the blinking cursor
    nodelay(stdscr, TRUE);  // non-blocking input, meaning, it returns ERR instead of blocking
			    // the screen

    int paddle_y = 0;       // vertical position
    int paddle_x = 2;       // horizontal position (left side)
    int paddle_h = 4;       // height in rows

    int ball_x = COLS/2;
    int ball_y = LINES/2;
    int ball_delay = 100; 

    struct timespec last, now;
    while (1) {
        int ch = getch();   // reads a keypress; returns ERR if none (because of nodelay above)

        if (ch == 'w' && paddle_y > 0)             paddle_y--;
        if (ch == 's' && paddle_y + paddle_h < LINES) paddle_y++;
        if (ch == 'q') break;

	if (paddle_y + paddle_h >= LINES) paddle_y = LINES - paddle_h;

        clear();            // wipes the entire screen

	// draw ball
	mvprintw(ball_y, ball_x, "O");

	// move ball
	clock_gettime(CLOCK_MONOTONIC, &now);
	double elapsed = (now.tv_sec - last.tv_sec) * 1000.0 + (now.tv_nsec - last.tv_nsec) / 1e6;

	if (elapsed >= ball_delay) {
		move_ball(&ball_y, &ball_x);
		last = now;
	}
	if ((int)elapsed > 10000 && (int)elapsed % 10000 == 0) {
		ball_delay -= 5;
	}

        // draw paddle
        for (int i = 0; i < paddle_h; i++) {
            mvprintw(paddle_y + i, paddle_x, "|"); // mvprintw(y, x, "string")
						   // moves cursor to (y, x) and prints
						   // mv = move
						   // printw = printf for ncurses
        }	

        refresh();          // actually pushes everything to the terminal
			    // ncurses batches draws, nothing shows until you call this
    }

    endwin();               // exits alternate screen, restores terminal to original state
    return 0;
}


void move_ball(int *ball_y, int *ball_x) {
	*ball_y += ball_vector[0];
	*ball_x += ball_vector[1];

	if (*ball_y == 0 || *ball_y == LINES) ball_vector[0] = -ball_vector[0];
	if (*ball_x == 0 || *ball_x == COLS) ball_vector[1] = -ball_vector[1];
}
