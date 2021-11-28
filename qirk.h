#ifndef QIR_H
#define QIR_H

#include <stdint.h>
#include <stdarg.h>

// Screen color pairs
#define DEFAULT_COLOR 0x1
#define BAR_COLOR     0x2

// Window funcitons =========================================== 
void window_create();
void window_destroy();

void window_write(const char* message);

void window_set_header(const char* header, int x);
void window_set_status(const char* footer, int x);

// IRC client funcitons ========================================
void irc_connect(char* server, char* port);
void irc_write(char* fmt, ...);
void irc_run_loop(char* hostname);

#endif // QIR_H

