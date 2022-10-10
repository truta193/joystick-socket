## joystick-socket

Made for DS4 controllers on linux, configured for Hollow Knight default layout. Meant to transmit controller inputs from one device to another via the network. 
My original use case was to wirelessly transmit input to my computer which had no bluetooth adaptor, using my laptop as a middleman.

The reciever should run the server w/o arguments:
```bash
./server
```

The sender should run the client with the following arguments:
```bash
./client RECV_IP PORT DEVICE
```

RECV_IP - the IP of the recieving computer

PORT - the port for communications (hardcoded to 8888)

DEVICE - the device path from /dev/
