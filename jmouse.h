#ifndef JMOUSE_H_
#define JMOUSE_H_

#include <SDL2/SDL_types.h>
#include <SDL2/SDL_events.h>
#include <sys/cdefs.h>

int jmouse_init(SDL_Window* window, int width, int height, Uint16 cursor_x, Uint16 cursor_y, Uint16 divider);
void jmouse_quit();
void jmouse_update(const SDL_Event* event);

#endif
