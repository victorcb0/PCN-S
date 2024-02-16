#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MY_PORT_ID 8080
#define MAX 1024

int main()
{
    int socket_id;
    struct sockaddr_in server_address, client_address;
    char message[MAX], message1[4], message2[MAX - 3];
    int n, len, delay;
    if ((socket_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Eroare creare socket");
        return -1;
    }

    bzero((char*) &client_address, sizeof(client_address));
    bzero((char*) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(MY_PORT_ID);

    if (bind(socket_id, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("Eroare de asociere");
        return -1;
    }

    len = sizeof(client_address);

    while (1) {
        n = recvfrom(socket_id, (char*) message, MAX, MSG_WAITALL, (struct sockaddr*) &client_address, &len);
        message[n] = '\0';
        printf("Client: %s\n", message);
        strncpy(message1, message, 3);
        message1[3] = '\0';
        if (strcmp(message1, ".d ") == 0) {
            strncpy(message2, &message[3], strlen(message) - 3);
            message2[strlen(message) - 3] = '\0';
            delay = atoi(message2);
            if (delay <= 0) {
                sendto(socket_id, "0", 1, MSG_CONFIRM, (struct sockaddr*) &client_address, len);
                printf("EROARE: delay >= 0\n");
                continue;
            }
        else
        {
            printf("delay - %d\n", delay);
            sendto(socket_id, "1", 1, MSG_CONFIRM, (struct sockaddr*) &client_address, len);
            n = recvfrom(socket_id, (char*) message, MAX, MSG_WAITALL, (struct sockaddr*) &client_address, &len);
            message[n] = '\0';
            printf("Client: %s\n", message);
            sleep(delay);
            sendto(socket_id, message, n, MSG_CONFIRM, (struct sockaddr*) &client_address, len);
            printf("Transmis\n");
            }
        }
        else {
            sendto(socket_id, "2", 1, MSG_CONFIRM, (struct sockaddr*) &client_address, len);
            n = recvfrom(socket_id, (char*) message, MAX, MSG_WAITALL, (struct sockaddr*) &client_address, &len);
            message[n] = '\0';
            sendto(socket_id, message, n, MSG_CONFIRM, (struct sockaddr*) &client_address, len);
            printf("Transmis\n");
        }
    }

    close(socket_id);

    return 0;
}