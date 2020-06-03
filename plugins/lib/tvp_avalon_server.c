/*
Lib for acting as a server to accept commands from Avalon
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <debug.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>  // sleep, etc

#include "plugdllx.h"
#include "plugx.h"
#include "avalon_client.h"

#include "avalon_vars.h"

#define BUFFERMAX 1024  //length of response-buffer
#define TVpfx "\033[1;32mTVP \033[0m" //colored output prefix

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

static int CONTINUELISTENING = 1;

void free_tokens(int tokencount, char **tokens)
{
    for (size_t i=0; i<tokencount; i++)
        free(tokens[i]);
    free(tokens);
}

void sendCommand(PIFilter *iFilter, const char* cmd, char* result)
{
    printf("%s Sending tvp-command: '%s'\n", TVpfx , cmd);
    int ret = TVSendCmd(iFilter, cmd, result);
    printf("%s %d\n", TVpfx, ret);
    char* retmsg = ret ? "Succes!" : "Fail!";
    printf("%s command-returned: %s\n", TVpfx, retmsg);
    printf("%s command-output: %s\n", TVpfx, result);
    fflush(stdout);
}


char *myParser(PIFilter *iFilter, char *recbuffer, size_t response_bufferlength)
{
    // returns: a returnmessage to be sent to avalon
    char *retmsg = calloc(response_bufferlength, sizeof(char)); // empty resultstring
    char **tokens = (char**) malloc(sizeof(char*)*64); // room for 64 tokens should be enough

    // convert the received message from Avalon as a list of tokens(strings)
    int tokencount = tokenize(tokens, recbuffer, ' ', '\\');
    if (strncmp("<RELEASE>", tokens[0], 9) == 0) {
        CONTINUELISTENING = 0;  
        strcpy(retmsg, "Stopping listenerloop");
    }
    printf("Got tokens:\n");
    for (int i=0; i< tokencount; i++)
        printf("%d\ttoken '%s'\n", i+1, tokens[i]);

    // run full tv_commands(George):
    if (tokencount > 0 && strcmp("tvpcmd:", tokens[0]) == 0) {
        size_t length = 0;
        for (int i=1; i< tokencount; i++) length += strlen(tokens[i]);

        char* cmd = calloc(length + tokencount -2, sizeof(char)); //allocate empty cmdstring
        memset(cmd, 32, length + tokencount -1); // fill with spaces, leave last one zero as NULL terminator
        size_t pos = 0;
        for (int i=1; i < tokencount; i++) {
            size_t tokenlength = strlen(tokens[i]);
            strncpy(cmd + pos, tokens[i], tokenlength);
            pos += tokenlength + 1;
        }

        char result[1024] = {0};
        sendCommand(iFilter, (const char*)cmd, result);
        free(cmd);
        strncpy(retmsg, result, strlen(result) + 1);
    }

    // load tvp-projects:
    if (tokencount > 0 && strcmp("open", tokens[0]) == 0) {
        if (tokencount > 2 && strcmp("tvproject", tokens[1]) == 0) {
            char* path = tokens[2];
            char result[1024] = {0};
            const char* cmd_format = "tv_LoadProject %s";
            char* cmd = calloc(strlen(cmd_format) + strlen(path), sizeof(char));
            sprintf(cmd, cmd_format, path);
            sendCommand(iFilter, (const char*)cmd, result);
            free(cmd);
            strncpy(retmsg, result, strlen(result) + 1);
        }
    }
    if (tokencount > 0 && strcmp("import", tokens[0]) == 0) {
        if (tokencount > 2 && strcmp("imgsequence", tokens[1]) == 0) {
            char* path = tokens[2];
            char result[1024] = {0};
            const char* cmd_format = "tv_LoadSequence \"%s\" STRETCH";
            char* cmd = calloc(strlen(cmd_format) + strlen(path), sizeof(char));
            sprintf(cmd, cmd_format, path);
            sendCommand(iFilter, cmd, result);
            free(cmd);
            strncpy(retmsg, result, strlen(result) + 1);
        }
    }
    if (tokencount > 0 && strcmp("save", tokens[0]) == 0) {
        if (tokencount > 2 && strcmp("tvproject", tokens[1]) == 0) {
            char* path = tokens[2];
            char result[1024] = {0};
            const char* cmd_format = "tv_SaveProject %s";
            char* cmd = calloc(strlen(cmd_format) + strlen(path), sizeof(char));
            sprintf(cmd, cmd_format, path);
            sendCommand(iFilter, cmd, result);
            free(cmd);
            strncpy(retmsg, result, strlen(result) + 1);
        }
    }
    if (tokencount > 0 && strcmp("get", tokens[0]) == 0) {
        if (tokencount > 1 && strcmp("projectID", tokens[1]) == 0) {
            const char *cmd = "tv_ProjectCurrentID";
            char result[1024] = {0};
            sendCommand(iFilter, cmd, result);
            strncpy(retmsg, result, strlen(result) + 1);
        }
        if (tokencount > 1 && strcmp("currentfile", tokens[1]) == 0) {
            const char *cmd = "tv_ProjectCurrentID";
            char result[1024] = {0};
            sendCommand(iFilter, cmd, result);
            strncpy(retmsg, result, strlen(result) + 1);
        }
        if (tokencount > 1 && strcmp("currentstructure", tokens[1]) == 0) {
            if (tokencount > 2) {
                const char* cmd_format = "tv_clipsavestructure \"%s\" \"CSV\" \"csvonly\" 1";
                char* cmd = calloc(strlen(cmd_format) + strlen(tokens[2]), sizeof(char));
                sprintf(cmd, cmd_format, tokens[2]);
                char* result = calloc(response_bufferlength, sizeof(char));
                sendCommand(iFilter, (const char*)cmd, result);
                strncpy(retmsg, result, strlen(result) + 1);
                debug();
                fflush(stdout);
            }
        }
    }
    if (strcmp("set", tokens[0]) == 0) {
        // set userstring
        if (tokencount > 3) {
            char* section_name = tokens[1];
            char* userstring_name = tokens[2];
            char* data = tokens[3];
            int ret = TVWriteUserString(
                    iFilter, 
                    section_name,
                    userstring_name,
                    data);

            if (ret == 0)
                log_err("TVWriteUserString went wrong..");
        }
        strcpy(retmsg, "TVWriteUserString written");
    }

    if (strlen(retmsg) == 0)
        strcpy(retmsg, "No result");

    free_tokens(tokencount, tokens);
    return retmsg;
}


void listener(PIFilter* iFilter)
{
    printf("%s<<<<<<<<<<<<<<<<<<<<<< START LISTENING >>>>>>>>>>>>>>>>>\n", TVpfx);
    int socket_fd , client_sock , c;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_fd = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_fd == -1)
    {
        log_err("Could not create socket");
    }
    log_info("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( SERVERPORT );
     
    //Bind
    if( bind(socket_fd,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        log_err("bind failed. Error");
    } else {
        log_info("bind done");
        
        //Listen
        listen(socket_fd , 3);
        //Accept and incoming connection
        log_info("Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);
        CONTINUELISTENING = 1;  
        while(CONTINUELISTENING == 1) {
            if ((client_sock = accept(socket_fd, (struct sockaddr *)&client, (socklen_t*)&c))) {
                log_info("Connection accepted");
                ssize_t read_size = 0;
                char buff[BUFFERMAX];
                memset(&buff, 0, BUFFERMAX);
                while((read_size = recv(client_sock , buff , BUFFERMAX , 0)) > 0 ) {
                    // read the message from client and copy it in buffer
                    //read(client_sock, buff, sizeof(buff));
                    // print buffer which contains the client contents
                    printf("parsing from client: '%s'\n", buff);
                    char *returnmessage = myParser(iFilter, buff, BUFFERMAX);
                    // clean receiving-buffer
                    memset(&buff, 0, BUFFERMAX);
                    // and send that buffer to client
                    write(client_sock, returnmessage, strlen(returnmessage));
                    free(returnmessage);
                }
                if(read_size == 0)
                {
                    log_info("Client disconnected");
                    fflush(stdout);
                }
                else if(read_size == -1)
                {
                    log_err("recv failed");
                }
                debug("Closing client socket");
                close(client_sock);
                printf("#####################  CONTINUELISTENING = %d\n", CONTINUELISTENING);
                fflush(stdout);
                (CONTINUELISTENING == 1 ? debug("####################Continuelisten"): debug("########################stoplistening") );
            }
        }
    }
    debug("closing main socket");
    close(socket_fd);
}



