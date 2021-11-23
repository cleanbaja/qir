#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include "qir.h"

static char* hostname = "irc.libera.chat";
static char* port = "6667";
static char* arg0 = NULL;

static void version() {
	printf("qir v0.1.0\n");
	exit(0);
}

static void usage() {
	fprintf(stderr, "USAGE: %s [-s server] [-p port]\n", arg0);
	exit(1);
}

int main(int argc, char** argv) {
	arg0 = argv[0];

	int cval;
	while ((cval = getopt(argc, argv, "v:s:p:h")) != -1) {
		switch (cval) {
		case 'v' : version();         break;
		case 's' : hostname = optarg; break;
		case 'p' : port = optarg;     break;
		case '?' : usage();           break;
		}
	}

	window_create();

	char mes[250];
	snprintf(mes, 250, "Connecting to %s:%s", hostname, port);
	window_set_status(mes, -1);
	
	window_destroy();

	return 0;
}

