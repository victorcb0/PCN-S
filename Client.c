#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MY_PORT_ID 8080
#define MAX 1024

int main()
{
    int socket_id;
    struct sockaddr_in server_address;
    char message[MAX], buffer[MAX], delay[3];
    int n, len;
    struct timeval start, end;
    double time;

    if ((socket_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Eroare creare socket");
        return -1;
    }

    bzero((char*) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(MY_PORT_ID);

    while (1) {
        printf("Mesaj: ");
        fgets(message, MAX, stdin);
        message[strlen(message)-1] = '\0';
        sendto(socket_id, message, strlen(message), MSG_CONFIRM, (struct sockaddr*) &server_address, sizeof(server_address));
        n = recvfrom(socket_id, delay, 1, MSG_WAITALL, (struct sockaddr*) &server_address, &len);
        delay[n] = '\0';
        if (strcmp(delay, "1\0") == 0) {
            printf("Mesaj: ");
            fgets(message, MAX, stdin);
            message[strlen(message)-1] = '\0';
        }
        else if (strcmp(delay, "0\0") == 0)
        {
            printf("EROARE: delay >= 0\n");
            continue;
        }
        gettimeofday(&start, NULL);
        sendto(socket_id, message, strlen(message), MSG_CONFIRM, (struct sockaddr*) &server_address, sizeof(server_address));
        n = recvfrom(socket_id, buffer, MAX, MSG_WAITALL, (struct sockaddr*) &server_address, &len);
        buffer[n] = '\0';
        gettimeofday(&end, NULL);
        time = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec - start.tv_usec / 1e6;
        printf("durata transmitere/receptie \"%s\": %f\n", buffer, time);
    }

    close(socket_id);

    return 0;
}