#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>

#include "qirk.h"

static char* hostname = "irc.libera.chat";
static char* port = "6667";
static char* arg0 = NULL;
static char* username = NULL;

static void version() {
	printf("qirk v0.1.0\n");
	exit(0);
}

static void usage() {
	fprintf(stderr, "USAGE: %s [-u username] [-s server] [-p port]\n", arg0);
	exit(1);
}

static char* get_username() {
	int uid = geteuid();
	struct passwd* pw = getpwuid(uid);

	if (pw) {
		return pw->pw_name;
	} else {
		return "generic_user";
	}
}

int main(int argc, char** argv) {
	arg0 = argv[0];

	int cval;
	while ((cval = getopt(argc, argv, "v:s:p:u")) != -1) {
		switch (cval) {
		case 'v' : version();         break;
		case 'u' : username = optarg; break;
		case 's' : hostname = optarg; break;
		case 'p' : port = optarg;     break;
		case '?' : usage();           break;
		}
	}

	window_create();

	// Attempt to get username (for nick)
	if (!username) {
		username = get_username();
	}	

	char mes[256];
	snprintf(mes, 256, "Connecting to %s:%s as %s", hostname, port, username);
	window_set_status(mes, -1);

	irc_connect(hostname, port);
	irc_write("NICK %s\r\n", username);
	irc_write("USER %s localhost %s :%s\r\n", username, hostname, username);

	// Enter the run loop
	irc_run_loop(hostname);	

	return 0;
}

