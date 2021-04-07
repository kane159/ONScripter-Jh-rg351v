#ifndef UINPUT_H_
#define UINPUT_H_
struct deltaData
{
    signed int x;
    signed int y;
};
#define ERROR_DEVICE_HANDLER -1;
void uninstall_uinput_mouse_device();
void mouse_move(struct deltaData* data);

#endif