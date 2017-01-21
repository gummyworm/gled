#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>
#include "window.h"

int gled_init();
void gled_quit();

void gled_redraw();
void gled_clear();
void gled_resize(uint64_t, uint64_t);
void gled_set_mainwin(Window*);

void gled_onmousepress(uint64_t, uint64_t);
void gled_onmouserelease(uint64_t, uint64_t);

#endif
