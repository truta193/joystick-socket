#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

int read_event(int fd, struct js_event *event) {
    ssize_t bytes;
    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event)) return 0;
    return -1;
};

/*
 * argv[1] is server ip
 * argv[2] is port
 * argv[3] is device
*/
int main(int argc, char *argv[]) {
    //Handle controller
    int joystick_fd;
    struct js_event joystick_event;

    //joystick_fd = open(argv[3], O_RDONLY);
    joystick_fd = open(argv[3], O_RDONLY);
    if (joystick_fd == -1) {
        printf("Could not open joystick\n");
        return -1;
    };
    
    //Handle client socket
    int socket_fd;
    char buffer[5];
    struct sockaddr_in server_address;
    
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        printf("Could not open socket\n");
        return -1;
    };

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[2]));
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    

   //Packet structure
   //   1  +   1   +    2   +  1
   // type + number  value  + \0
   
    while (read_event(joystick_fd, &joystick_event) == 0) {
        if (joystick_event.type == JS_EVENT_AXIS || joystick_event.type == JS_EVENT_BUTTON) {
            buffer[0] = joystick_event.type == JS_EVENT_AXIS ? JS_EVENT_AXIS : JS_EVENT_BUTTON;
            buffer[1] = joystick_event.number;
            memcpy(&buffer[2], &joystick_event.value, 2);
            buffer[4] = '\0';
            sendto(socket_fd, buffer, 5, 0, (const struct sockaddr*)&server_address, sizeof(server_address));
        };
    };

    close(joystick_fd);
    close(socket_fd);
    return 0;
}
