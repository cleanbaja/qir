#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define MAX_EVENTS 2
static char* msg_buf = NULL;
static int sock_fd = -1;

int irc_connect(char* host, char* port) {
	struct addrinfo hints = {0};
	struct addrinfo *res;
	int st, ret_fd;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((st = getaddrinfo(host, port, &hints, &res)) != 0) {
		printf("qirk: can't connect to socket (%s)\n", gai_strerror(st));
		goto fail;
	}

	ret_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (ret_fd < 0) {
		printf("qirk: error opening socket!\n");
		goto fail;
	}

	st = connect(ret_fd, res->ai_addr, res->ai_addrlen);
	if (st != 0) {
		printf("qirk: error connecting to socket!\n");
		goto fail;
	}

	return ret_fd;
fail:
	return -1;
}

void irc_write(char* fmt, ...) {
	va_list va;
	va_start(va, fmt);

	char sbuf[256];
	int len = vsprintf(sbuf, fmt, va);

	write(sock_fd, sbuf, len);

	va_end(va);
	memset(sbuf, 0, len);
}

static void parse_command(char* str) {
	char *prefix, *msg, *suffix, *nick, *cmd, *params;
	char *save_ptr, *save_ptr2, *save_ptr3;

	prefix = strtok(str, " ") + 1;
	suffix = strtok(NULL, ":");
	msg    = strtok(NULL, "\r");
	nick   = strtok(prefix, "!");
	cmd    = strtok(suffix, "#& ");
	
	strtok(NULL, " \r");
	params = strtok(NULL, ":\r");

	if (strcmp(cmd, "NOTICE") == 0) {
		printf("NOTICE: %s\n", msg);
	} else {
		if (atoi(cmd) <= 3) {
			printf("-> %s\n", msg);
			goto out;
		}
	}
out:	
	// printf("<%s> (%s) %s\n", nick, cmd, msg);
}

static void parse_message() {
	char* save_ptr;
	char* nix = strtok_r(msg_buf, "\n", &save_ptr);
	while (nix != NULL) {
		parse_command(nix);
		nix = strtok_r(NULL, "\n", &save_ptr);
	}
}

int main(int argc, char** argv) {
	puts("Hello from qirk!");
	msg_buf = (char*)malloc(2048);

	sock_fd = irc_connect("irc.libera.chat", "6667");
	if (sock_fd < 0) {
		goto out;
	}

	struct epoll_event event, events[MAX_EVENTS];
	int epoll_fd = epoll_create(2); // Watch stdin and sock_fd
	if (epoll_fd < 0) {
		printf("qirk: epoll_create() failed\n");
		goto out;
	}

	// Add Standard Input to the list of file descriptors to watch
	event.events = EPOLLIN;
	event.data.fd = 0;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event)) {
		printf("qirk: unable to add socket to watchlist!\n");
		goto out;
	}

	// Add the Socket Descriptor to the list of file descriptors to watch
	event.events = EPOLLIN;
	event.data.fd = sock_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &event)) {
		printf("qirk: unable to add stdin to watchlist!\n");
		goto out;
	}

	irc_write("NICK %s\r\n", "cleanbaja");
	irc_write("USER %s - - :%s\r\n", "cleanbaja", "cleanbaja");

	int nfds;
	for(;;) {
		nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000); // 30 second timeout
		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == sock_fd) {
				int msglen = read(sock_fd, msg_buf, 2048);
				if (msglen < 0) {
					printf("qirk: connection timed out!\n");
					goto out;
				}

				parse_message();

				memset(msg_buf, 0, msglen);
			} else if (events[i].data.fd == 0) {
				if (getc(stdin) == 'q') {
					free(msg_buf);
					return 0;
				}
			} else {
				printf("qirk: unknown epoll fd %d\n", events[i].data.fd);
				goto out;
			}
		}
	}

out:
	if (msg_buf)
		free(msg_buf);
	return EXIT_FAILURE;
}

