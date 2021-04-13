#include <sys/ioctl.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
 
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "uinput.h"
#define ALOGE(...) \
        printf(__VA_ARGS__); \
        printf("\n")
 
 
#define UINPUT_NAME      "/dev/uinput"
#define VIR_MOUSE_NAME   "DFC Virtual Mouse"
#include "ONScripter.h"

extern ONScripter ons;

static int device_handler = -1;
static struct uinput_user_dev vir_mouse;
 
static int install_uinput_mouse_device()
{
    if (device_handler > 0)
    {
        ALOGE("virtual mouse has been installed already.");
        return device_handler;
    }
 
    //open uinput device
    if ((device_handler = open(UINPUT_NAME, O_WRONLY | O_NDELAY)) <= 0)
    {
        ALOGE("open uinput(%s) failed.", UINPUT_NAME);
        device_handler = -1;
        return device_handler;
    }
 
    //setup our uinput device of virtual mouse
    memset(&vir_mouse, 0, sizeof(struct uinput_user_dev));
    strncpy(vir_mouse.name, VIR_MOUSE_NAME, UINPUT_MAX_NAME_SIZE);
    vir_mouse.id.version = 4;
    vir_mouse.id.bustype = BUS_USB;
 
    //if we don't do these register, it seems like not to work
    ioctl(device_handler, UI_SET_EVBIT, EV_KEY);
    for (int i = 0; i < 256; i++)
    {
        ioctl(device_handler, UI_SET_KEYBIT, i);
    }
 
    //setup mouse coordinate event
    ioctl(device_handler, UI_SET_EVBIT,  EV_REL);
    ioctl(device_handler, UI_SET_RELBIT, REL_X);
    ioctl(device_handler, UI_SET_RELBIT, REL_Y);
 
    //setup mouse button event
    ioctl(device_handler, UI_SET_KEYBIT, BTN_MOUSE);
    ioctl(device_handler, UI_SET_KEYBIT, BTN_TOUCH);
    ioctl(device_handler, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(device_handler, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(device_handler, UI_SET_KEYBIT, BTN_MIDDLE);
 
    //create input device into input subsystem
    write(device_handler, &vir_mouse, sizeof(vir_mouse));
    if (ioctl(device_handler, UI_DEV_CREATE))
    {
        ALOGE("install mouse uinput device failed with error(%s).", strerror(errno));
        close(device_handler);
        device_handler = -1;
    }
 
    return device_handler;
}
 
void uninstall_uinput_mouse_device()
{
    if (device_handler > 0)
    {
        ioctl(device_handler, UI_DEV_DESTROY);
        close(device_handler);
        device_handler = ERROR_DEVICE_HANDLER;
    }
}
 
void mouse_move(struct deltaData* data)
{
    
    if (device_handler <= 0)
    {
        install_uinput_mouse_device();
    }
 
    if (device_handler > 0)
    {
        struct input_event event;
        memset(&event, 0, sizeof(event));
 
        //x coordinate
        gettimeofday(&event.time, 0);
        event.type = EV_REL;
        event.code = REL_X;
        event.value = data->x;
        //lastX+=data->x;
        write(device_handler, &event, sizeof(event));
 
        //y coordinate
        event.type  = EV_REL;
        event.code  = REL_Y;
        event.value = data->y;
        //lastY+=data->y;
        write(device_handler, &event, sizeof(event));
 
        //execute move event
        event.type  = EV_SYN;
        event.code  = SYN_REPORT;
        event.value = 0;
        write(device_handler, &event, sizeof(event));
    }
    else
    {
        ALOGE("invalid device file handler.");
    }
}
