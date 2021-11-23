#include <ncurses.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

static char* hostname = "irc.libera.chat";
static char* port = "6667";

static void version() {
	printf("qir v0.1.0\n");
	exit(0);
}

static void usage() {
	printf("qir: Help text isn't ready :-(\n");
	exit(1);
}

int main(int argc, char** argv) {
	int cval;
	while ((cval = getopt(argc, argv, "v:s:p:h")) != -1) {
		switch (cval) {
		case 'v' : version();         break;
		case 's' : hostname = optarg; break;
		case 'p' : port = optarg;     break;
		case 'h' : usage();           break;
		}
	}

	initscr();
	
	printw("qir - The Quick IRc client (v0.1.0)\n\n");
	printw("Connecting to %s:%s", hostname, port);	
	
	refresh();			
	getch();			
	endwin();			

	return 0;
}
