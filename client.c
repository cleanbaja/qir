#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>

#include "qirk.h"

static int sock_fd = -1;
static char message_buf[4096] = {0};
static int datalen = 0;

void irc_connect(char* server, char* port) {
	int status = 0;
	struct addrinfo *res, *prt, hints = {
		.ai_family = AF_UNSPEC,
		.ai_socktype = SOCK_STREAM
	};

	if ((status = getaddrinfo(server, port, &hints, &res)) != 0) {
		fprintf(stderr, "qirk: getaddrinfo() failed (%s)\n", gai_strerror(status));
		exit(0);
	}

	for (prt = res; prt != NULL; prt = prt->ai_next) {
		if ((sock_fd = socket(prt->ai_family, prt->ai_socktype, prt->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}
		if (connect(sock_fd, prt->ai_addr, prt->ai_addrlen) == -1) {
			close(sock_fd);
			perror("connect");
			continue;
		}
		break;
	}

	if (prt == NULL) {
		fprintf(stderr, "qirk: Failed to connect to %s:%s\n", server, port);
		exit(1);
	}
	
	int flags = fcntl(sock_fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(sock_fd, F_SETFL, flags);
}

void irc_write(char* fmt, ...) {
	va_list ap;
	char *cmd_str = malloc(512);
	if (!cmd_str) {
		perror("malloc");
		exit(1);
	}
	
	va_start(ap, fmt);
	vsnprintf(cmd_str, 512, fmt, ap);
	va_end(ap);
	
	if (write(sock_fd, cmd_str, strnlen(cmd_str, 512)) < 0) {
		perror("write");
		exit(1);
	}
	free(cmd_str);	
}

static void parse_message() {
	char mbuf[64] = {0};
	sprintf(mbuf, "Message of len %d Recieved!\n", datalen);
	window_write(mbuf);
}

void irc_run_loop(char* hostname) {
	// Default 120 second timeout
	struct timeval t;
	t.tv_sec = 120;
	t.tv_usec = 0;

	fd_set fds;

	int n;
	for(;;) {
		FD_ZERO(&fds);
		FD_SET(0, &fds);         // Wait for stdin
		FD_SET(sock_fd, &fds);   // Wait for the irc conn
		n = select(sock_fd + 1, &fds, 0, 0, &t);

		if ( n < 0 ) {
			if (errno == EINTR)
				continue;

			fprintf(stderr, "qirk: select() failed with %d\n", n);
			exit(1);
		} else if (n == 0) {
			if (time(NULL)>= 300) {
				fprintf(stderr, "qirk: exiting on timeout\n");
				exit(1);
			}

			irc_write("PING %s\r\n", hostname);
			continue;
		} 
		
		if (FD_ISSET(sock_fd, &fds)) {
			if ((datalen = read(sock_fd, message_buf, 4096)) == 0) {
				fprintf(stderr, "qirk: connection closed by server!\n");
				exit(1);
			}

			parse_message();

			goto exit;
		}
		
		if (FD_ISSET(0, &fds)) {
			goto exit;
		}
	}

exit:
	window_destroy();

}

