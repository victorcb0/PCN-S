# PCN-S (Programming Computer Networks - Sockets)
PCN-S (Programming Computer Networks - Sockets) involves the development of applications that communicate with each other over computer networks using sockets. These are the communication points between the network nodes. This problem helps programmers understand the essential concepts of socket communication and develop practical skills in developing computer network applications.

---
***Problem***

To create a client-server application that can measure the time that passes from the transmission of a message from the client to the server until the response is received. There must be the possibility of sending command messages to the server (where nothing is measured) in order to have a delay before sending the data back to the client. The delay must be greater than or equal to 0, otherwise the server will signal an error. The UDP protocol will be used.

Example (in the client):
```
test
"test" transmission/reception duration: 0.323 seconds
.d 3
test
"test" transmission/reception duration: 3,315 seconds
.d -1
ERROR: delay >= 0
```
