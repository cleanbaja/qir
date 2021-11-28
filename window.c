#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>

#include "qirk.h"

static int scr_width, scr_height = 0;

static WINDOW* header_win;
static WINDOW* status_bar_win;
static WINDOW* main_win;

void window_create() {
	initscr();

	// Check for color support before doing anything else
	if (has_colors() == FALSE) {
		endwin();
		fprintf(stderr, "qirk: terminal dosen't support colors!\n");
		exit(1);
	}

	// Setup pairs
	start_color();
	init_pair(DEFAULT_COLOR, COLOR_WHITE, COLOR_BLACK);
	init_pair(BAR_COLOR, COLOR_WHITE, COLOR_BLUE);
	
	// Get window dimensions
	struct winsize size;
	ioctl(0, TIOCGWINSZ, (char *) &size);
	scr_width = size.ws_col;
	scr_height = size.ws_row;

	// Create the header and status bar, and create the main section
	header_win = newwin(1, scr_width, 0, 0);
	status_bar_win = newwin(1, scr_width, scr_height - 1, 0);
	main_win = newwin(scr_height - 2, scr_width, 1, 0);

	// Enable scrolling for the main window
	scrollok(main_win, TRUE);
	// Paint bar
	wattron(header_win, COLOR_PAIR(BAR_COLOR));
	for (int i = 0; i < (scr_width); i++) {
		waddch(header_win, ' ');
	}

	// Then set the header
	char* header_message = "qirk - The Quick IRc Klient (v0.1.0)";
	int begin_pos = (scr_width / 2) - (strlen(header_message) / 2);
	
	mvwprintw(header_win, 0, begin_pos, header_message);
}

void window_destroy() {
	wrefresh(main_win);
	wrefresh(header_win);
	wrefresh(status_bar_win);
	wgetch(main_win);

	// Free all windows
	delwin(main_win);
	delwin(header_win);
	delwin(status_bar_win);

	endwin();
}

void window_write(const char* message) {
	wprintw(main_win, message);
}

void window_set_header(const char* header, int x) {
	// TODO: Assert that message is only 1 line long
	if (x == -1) {
		// Center align
		int begin_pos = (scr_width / 2) - (strlen(header) / 2);
		mvwprintw(header_win, 0, begin_pos, header);
	} else {
		mvwprintw(header_win, 0, x, header);
	}

	wrefresh(header_win);
}

void window_set_status(const char* footer, int x) {
	// TODO: Assert that message is only 1 line long
	if (x == -1) {
		// Don't move the cursor to the beginning
		wprintw(status_bar_win, footer);
	} else {
		mvwprintw(status_bar_win, 0, x, footer);
	}

	wrefresh(status_bar_win);
}

