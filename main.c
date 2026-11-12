#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define BALL_INIT_POS_X COLS/2
#define BALL_INIT_POS_Y LINES/2

void move_ball(int *ball_y, int *ball_x, int *paddle_y, int *paddle_h, int *paddle_x, int *ai_y, int *ai_x, int *ai_h);
void move_ai(int *ball_y, int *ball_x, int *ai_y, int *ai_x, int *ai_h);

int ball_vector[2] = {-1, -1};
int ball_delay = 70;
int your_score = 0;
int ai_score = 0;

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

    int ai_y = 0;
    int ai_x = COLS - 2;
    int ai_h = 4;

    int ball_x = BALL_INIT_POS_X;
    int ball_y = BALL_INIT_POS_Y;

    struct timespec last, now;
    while (1) {
        int ch = getch();   // reads a keypress; returns ERR if none (because of nodelay above)

        if (ch == 'w' && paddle_y > 0)             paddle_y--;
        if (ch == 's' && paddle_y + paddle_h < LINES) paddle_y++;
        if (ch == 'q') break;

	if (paddle_y + paddle_h >= LINES) paddle_y = LINES - paddle_h;
	if (ai_y + ai_h >= LINES) ai_y = LINES - ai_h;

        clear();            // wipes the entire screen

	// draw ball
	mvprintw(ball_y, ball_x, "O");

	// draw score
	mvprintw(LINES-1, 0, "%d:%d", your_score, ai_score);

	// move ball
	clock_gettime(CLOCK_MONOTONIC, &now);
	double elapsed = (now.tv_sec - last.tv_sec) * 1000.0 + (now.tv_nsec - last.tv_nsec) / 1e6;

	// change ball position every {ball_delay} time; this time decreases every time
	// ball hits the top or bottom - see move_ball function
	if (elapsed >= ball_delay) {
		move_ball(&ball_y, &ball_x, &paddle_y, &paddle_h, &paddle_x, &ai_y, &ai_x, &ai_h);

		move_ai(&ball_y, &ball_x, &ai_y, &ai_x, &ai_h);

		last = now;
	}	

        // draw paddle
        for (int i = 0; i < paddle_h; i++) {
            mvprintw(paddle_y + i, paddle_x, "|"); // mvprintw(y, x, "string")
						   // moves cursor to (y, x) and prints
						   // mv = move
						   // printw = printf for ncurses
        }
	for (int i = 0; i < ai_h; i++) mvprintw(ai_y + i, ai_x, "|");

        refresh();          // actually pushes everything to the terminal
			    // ncurses batches draws, nothing shows until you call this
    }

    endwin();               // exits alternate screen, restores terminal to original state
    return 0;
}


void move_ball(int *ball_y, int *ball_x, int *paddle_y, int *paddle_h, int *paddle_x, int *ai_y, int *ai_x, int *ai_h) {
	*ball_y += ball_vector[0];
	*ball_x += ball_vector[1];

	if (*ball_y <= 0 || *ball_y >= LINES) {
		ball_vector[0] = -ball_vector[0];
	}

	if (*ball_x <= 0) ai_score++, *ball_x = BALL_INIT_POS_X, ball_vector[1] = -ball_vector[1];
	if (*ball_x >= COLS) your_score++, *ball_x = BALL_INIT_POS_X, ball_vector[1] = -ball_vector[1];

	if (*ball_x == *ai_x && (*ball_y <= *ai_y + *ai_h && *ball_y >= *ai_y))
		ball_vector[1] = -ball_vector[1];
	if (*ball_x == *paddle_x && (*ball_y <= *paddle_y + *paddle_h && *ball_y >= *paddle_y))
		ball_vector[1] = -ball_vector[1];
}

void move_ai(int *ball_y, int *ball_x, int *ai_y, int *ai_x, int *ai_h) {
	if (ball_vector[1] > 0 && *ball_x >= COLS/2) {
		if (*ai_y > 0 && ball_vector[0] < 0)
			(*ai_y) -= 1;
		if (ball_vector[0] > 0)
			(*ai_y) += 1;
	}
}
