#include <SDL2/SDL.h>

#define	JOYSTICK_NEUTRAL_RANGE_FROM -2000
#define JOYSTICK_NEUTRAL_RANGE_TO 2000
#define MAX_W 640
#define MAX_H 480
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
	screen_size[X] = MAX_W;
	screen_size[Y] = MAX_H;
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
	if (event->type == SDL_JOYAXISMOTION) {
		printf("jmouse_update:%d,%d\n",event->jaxis.axis,event->jaxis.value);
		if (event->jaxis.value < JOYSTICK_NEUTRAL_RANGE_FROM ||
				event->jaxis.value > JOYSTICK_NEUTRAL_RANGE_TO) {
			axes_values[event->jaxis.axis] = event->jaxis.value;
			int tmpDelta = abs(axes_values[event->jaxis.axis]) - abs(old_axes_values[event->jaxis.axis]);
			printf("tmpDelta:%d\n",tmpDelta);
			deltaXY[event->jaxis.axis] = 0;
			if(event->jaxis.axis == 0) {
				if(event->jaxis.value>0 && tmpDelta<0) {
					deltaXY[event->jaxis.axis] = tmpDelta;
				}
				else if(event->jaxis.value<0 && tmpDelta>0) {
					deltaXY[event->jaxis.axis] = tmpDelta;
				}
			}
			else if(event->jaxis.axis == 1) {                                                                                                                                                                                           if(event->jaxis.value>0 && tmpDelta<0) {
                                       deltaXY[event->jaxis.axis] = tmpDelta;
				}
				else if(event->jaxis.value<0 && tmpDelta>0) { 
				       deltaXY[event->jaxis.axis] = tmpDelta;
				}
			}				                         
												                        
			printf("deltaXY:%d,%d\n",event->jaxis.axis,deltaXY[event->jaxis.axis]);
		}

		for (i = 0; i < 2; i++) {

			cursor_position[i] += (deltaXY[i]/ value_divider/ 10.0);
			printf("cursor_position[i]:%f\n",cursor_position[i]);
			if (cursor_position[i] < 0)
				cursor_position[i] = 0;
			else if (cursor_position[i] > screen_size[i])
				cursor_position[i] = screen_size[i];

			old_axes_values[i] = axes_values[i];
			if(deltaXY[i] > 0) {
				SDL_WarpMouseInWindow(app_window,cursor_position[X], cursor_position[Y]);
				//TODO: use uinput https://blog.csdn.net/dayong419/article/details/9099023
			}
		}

			printf("SDL_WarpMouseInWindow:%d,%f,%f\n",fps,cursor_position[X], cursor_position[Y]); 
			
			/*SDL_Event event = {0};
                	event.type = SDL_MOUSEMOTION;
                	event.motion.x = cursor_position[X];
                	event.motion.y = cursor_position[Y];
       	         	event.motion.xrel = cursor_position[X];
       	         	event.motion.yrel = cursor_position[Y];
	                SDL_PushEvent(&event);*/
	}

	++frame_cnt;
	new_time = SDL_GetTicks();
	if (new_time - time > 1000) {
		time = new_time;
		fps = frame_cnt;
		frame_cnt = 0;
	}
	
}
