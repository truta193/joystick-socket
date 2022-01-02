#define _GNU_SOURCE 
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/joystick.h>
#include <errno.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory.h>

#define PORT 8888

/*    ioctl(uip_fd, UI_SET_KEYBIT, BTN_B);        //Cross
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_C);        //Circle
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_X);        //Triangle
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_A);        //Square
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_Y);        //L1
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_Z);        //R1
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TL);       //L2
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TR);       //R2
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_SELECT);   //L3
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_START);    //R3
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_THUMBL);   //Trackpad
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TR2);    //Options
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_MODE);   //PS Button
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TL2);   //Share*/

const int translate_lookup_btn[13] = {BTN_SOUTH, BTN_EAST, BTN_WEST, BTN_NORTH, BTN_TL, BTN_TR, BTN_SELECT, BTN_START, BTN_TL2, BTN_TR2, BTN_MODE, BTN_THUMBL, BTN_THUMBR};
const int translate_lookup_abs[8] = {ABS_X, ABS_Y, ABS_Z, ABS_RX, ABS_RY, ABS_RZ, ABS_HAT0X, ABS_HAT0Y};

void emit_event(int fd, int type, int number, int val) {
    int  code;
    switch(type) {
        case JS_EVENT_BUTTON: {
            type = EV_KEY;
            code = translate_lookup_btn[number];
            break;
        };
        case JS_EVENT_AXIS: {
            type = EV_ABS;
            code = translate_lookup_abs[number];
            break;
        };
        default: return;
    };
    
    struct input_event ie;
    ie.type = (unsigned short) type;
    ie.value = (signed int) val;
    ie.code = (unsigned short) code;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;
    write(fd, &ie, sizeof(ie));

    ie.type = EV_SYN;
    ie.value = 0;
    ie.code = SYN_REPORT;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;
    write(fd, &ie, sizeof(ie));
};

int main(int atgc, char *argv[]) {
    //Uinput
    int uip_fd = open("/dev/uinput", O_WRONLY|O_NONBLOCK);
    if (uip_fd < 0) {
        printf("Could not open /dev/uinput\n");
        return -1;
    };
    
    //NOTE: FOR DS4 CONTROLLER
    ioctl(uip_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uip_fd, UI_SET_EVBIT, EV_ABS);
    ioctl(uip_fd, UI_SET_EVBIT, EV_SYN);

    
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_THUMBL); //Left joystick press
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_THUMBR); //Right joystick press

    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TL); //Left trigger (small, top)
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TR); //Right trigger (small, top)
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TL2); //Left trigger (big, bottom)
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TR2); //Right trigger (big, bottom)

    ioctl(uip_fd, UI_SET_KEYBIT, BTN_SELECT); //Share button
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_START); //Options button
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_MODE); //PS4 button

    ioctl(uip_fd, UI_SET_KEYBIT, BTN_SOUTH); //Cross
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_EAST); //Circle
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_NORTH); //Triangle
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_WEST); //Square
    
    //NOTE: These output as axis on my controller, no button event
    
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_DPAD_UP); //DPAD up
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_DPAD_DOWN); //DPAD down
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_DPAD_LEFT); //DPAD left
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_DPAD_RIGHT); //DPAD right
    
    /*
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_B);        //Cross
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_C);        //Circle
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_X);        //Triangle
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_A);        //Square
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_Y);        //L1
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_Z);        //R1
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TL);       //L2
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TR);       //R2
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_SELECT);   //L3
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_START);    //R3
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_THUMBL);   //Trackpad
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TR2);    //Options
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_MODE);   //PS Button
    ioctl(uip_fd, UI_SET_KEYBIT, BTN_TL2);   //Share
    */

    struct uinput_abs_setup abs_setup = {
        .code = 0,
        .absinfo = { .minimum = -32767, .maximum = 32767 }
    };
    //Left joystick X axis
    ioctl(uip_fd, UI_SET_ABSBIT, ABS_X);
    abs_setup.code = ABS_X;
    ioctl(uip_fd, UI_ABS_SETUP, &abs_setup);
    //Left joystick Y axis
    ioctl(uip_fd, UI_SET_ABSBIT, ABS_Y);
    abs_setup.code = ABS_Y;
    ioctl(uip_fd, UI_ABS_SETUP, &abs_setup);
    //Right joystick X axis
    ioctl(uip_fd, UI_SET_ABSBIT, ABS_RX);
    abs_setup.code = ABS_RX;
    ioctl(uip_fd, UI_ABS_SETUP, &abs_setup);
    //Right joystick Y axis
    ioctl(uip_fd, UI_SET_ABSBIT, ABS_RY);
    abs_setup.code = ABS_RY;
    ioctl(uip_fd, UI_ABS_SETUP, &abs_setup);
    //TL2 
    ioctl(uip_fd, UI_SET_ABSBIT, ABS_Z);
    abs_setup.code = ABS_Z;
    ioctl(uip_fd, UI_ABS_SETUP, &abs_setup);
    //TR2 
    ioctl(uip_fd, UI_SET_ABSBIT, ABS_RZ);
    abs_setup.code = ABS_RZ;
    ioctl(uip_fd, UI_ABS_SETUP, &abs_setup);
    //DPAD X axis
    ioctl(uip_fd, UI_SET_ABSBIT, ABS_HAT0X);
    abs_setup.code = ABS_HAT0X;
    ioctl(uip_fd, UI_ABS_SETUP, &abs_setup);
    //DPAD Y axis
    ioctl(uip_fd, UI_SET_ABSBIT, ABS_HAT0Y);
    abs_setup.code = ABS_HAT0Y;
    ioctl(uip_fd, UI_ABS_SETUP, &abs_setup);

    struct uinput_setup usetup = {
        .name = "DS4",
        .id = {
            .bustype = BUS_USB,
            .vendor = 0x3,
            .product = 0x3,
            .version = 1
        }
    };

    ioctl(uip_fd, UI_DEV_SETUP, &usetup);
    ioctl(uip_fd, UI_DEV_CREATE);

    //Socket
    int socket_fd;
    char buffer[7];
    struct sockaddr_in server_address, client_address;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) return -1;


    memset(&client_address, 0, sizeof(client_address));
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    
    if (bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) return -1;

    printf("Listening...\n");
    int run = 1;
    short event_value;
    int event_type;
    int event_number;
    int client_len = sizeof(client_address);
    while (run) {
        recvfrom(socket_fd, buffer, 5, 0, (struct sockaddr *)&client_address, &client_len);
        memcpy(&event_value, &buffer[2], 2);
        event_type = (int)(buffer[0]);
        event_number = (int)(buffer[1]);
        emit_event(uip_fd, event_type, event_number, event_value);
    };

    return 0;
};