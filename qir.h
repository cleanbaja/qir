#ifndef QIR_H
#define QIR_H

#include <stdint.h>

// Screen color pairs
#define DEFAULT_COLOR 0x1
#define BAR_COLOR     0x2

// Window funcitons =========================================== 
void window_create();
void window_destroy();

void window_write(const char* message);

void window_set_header(const char* header, int x);
void window_set_status(const char* footer, int x);

#endif // QIR_H

