#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <debug.h>

#include "avalon_vars.h"


int getSocket()
{
    struct sockaddr_in server_info;
    struct hostent *he;
    int socket_fd;

    if ((he = gethostbyname(PYWRAPPER_ADDRESS))==NULL) {
        log_err("Cannot get host name");
        return -1;
    }

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        log_err("Socket Failure!!");
        return -1;
    }

    memset(&server_info, 0, sizeof(server_info)); // clean the info-struct
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);  // convert unsigned short to network byte order
    server_info.sin_addr = *((struct in_addr *)he->h_addr);  // eeuh, cast he->h naar struct in_adr.. ja dat was een struct in struct oid
    if (connect(socket_fd, (struct sockaddr *)&server_info, sizeof(struct sockaddr))<0) {
        log_err("connection Failure");
        return -1;
    }
    return socket_fd;
}


int sendReceiveMessage(char *message, char *receivebuffer, int timeoutseconds)
// send one message and close our (client)socket
{
    //int receivelength;
    int socket_fd = getSocket();
    if (socket_fd < 0) { 
        log_err("Host is down");
        return -1;
    } else {
        //memset(buffer, 0 , sizeof(buffer));
        printf("TVP Sending message: %s\n", message);
        printf("TVP msglength: %lu\n", strlen(message));
        if ((send(socket_fd, message, strlen(message), 0)) == -1) {
            log_err("Failure Sending Message");
            close(socket_fd);
            return -1;
        } else {
            printf("Message sent: %s\n", message);
        }

        if (timeoutseconds > 0) {
            // Uncomment this if timout is needed again:
            struct timeval timeout;      
            timeout.tv_sec = timeoutseconds;
            timeout.tv_usec = 0;
            if (setsockopt (socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                        sizeof(timeout)) < 0) {
                log_err("setsockopt failed\n");
                close(socket_fd);   
                return -1;
            }
            if (setsockopt (socket_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                        sizeof(timeout)) < 0) {
                log_err("setsockopt failed");
                close(socket_fd);   
                return -1;
            } 

            log_info("Waiting for server answer..");
            size_t receivelength;
            if ((receivelength = recv(socket_fd, receivebuffer, RESPONSEBUFFER_LENGTH, 0))== -1) {
                log_err("receiving failure");
                close(socket_fd);   
                return -1;
            }
        }
    }
    close(socket_fd);   
    return 1;
}


// this is for testing:
int main(int argc, char *argv[])
{
    char returnmessage[RESPONSEBUFFER_LENGTH];
    memset(&returnmessage,0, RESPONSEBUFFER_LENGTH);
    char *message = "dofunction arg1 arg2 arg3";
    int ret = sendReceiveMessage(message, returnmessage, 0);
    if (ret <0) {
        //printf("Sending/receiving message has failed.\n");
        return 1;
    }
    if (strcmp(returnmessage, "") == 0)
    {
        printf("er ging iets mis met de host!!\n");
    
    } else {
        printf("resultaat: %s\n", returnmessage);
    }
    return 0;
}

