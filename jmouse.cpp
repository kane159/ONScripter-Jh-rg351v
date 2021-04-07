#include <SDL2/SDL.h>
#include "uinput.h"
#define	JOYSTICK_NEUTRAL_RANGE_FROM -2000
#define JOYSTICK_NEUTRAL_RANGE_TO 2000
#define X 0
#define Y 1

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static float cursor_position[2] = {0,0};
static float axes_values[2] = {0,0};
static float old_axes_values[2] = {0,0};
static SDL_Joystick* joystick = NULL;
static Uint16 value_divider = 1;
static int screen_size[2] = {0,0};
static Uint32 time = 0;
static unsigned int frame_cnt = 0;
static unsigned int fps = 0;
static SDL_Window* app_window = NULL;
static int deltaXY[2] = {0,0};
int jmouse_init(SDL_Window* window, int width, int height, Uint16 cursor_x, Uint16 cursor_y, Uint16 divider)
{
	app_window = window;
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0 || SDL_NumJoysticks() == 0)
		return -1;
	printf("jmouse_init:%d,%d,%d,%d,%d,%d\n",window,width,height,cursor_x,cursor_y,divider);
	joystick = SDL_JoystickOpen(0);
	if (!joystick || SDL_JoystickNumAxes(joystick) < 2) {
		printf("joystick:%d,num of axes:%d\n", joystick, SDL_JoystickNumAxes(joystick));
		return -1;
	}
	printf("jmouse_open\n");
	screen_size[X] = 0;
	screen_size[Y] = 0;
	value_divider = divider;

	cursor_position[X] = MIN(cursor_x, width);
	cursor_position[Y] = MIN(cursor_y, height);

	return 0;
}

void jmouse_quit()
{
	if (joystick)
		SDL_JoystickClose(joystick);
}

void jmouse_update(const SDL_Event* event)
{
	Uint32 new_time;
	int i;
	int xPostion = SDL_JoystickGetAxis(joystick,0);
	int yPostion = SDL_JoystickGetAxis(joystick,1);

	if(xPostion > JOYSTICK_NEUTRAL_RANGE_FROM && xPostion <  JOYSTICK_NEUTRAL_RANGE_TO  && 
		yPostion > JOYSTICK_NEUTRAL_RANGE_FROM && yPostion <  JOYSTICK_NEUTRAL_RANGE_TO)
		return;
	int deltaX = -xPostion/(32767/2.0);
	int deltaY = -yPostion/(32767/1.33);
	printf("jmouse_update:%d,%d\n",xPostion,yPostion);
	
	if( abs(deltaX) > 0 || abs(deltaY) > 0) {
		struct deltaData data;
		data.x = deltaX ;
		data.y = deltaY ;
		mouse_move(&data);
	}
	
}
