#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "plugdllx.h"
#include "plugx.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>  // sleep, etc
#include <pthread.h>
#include <debug.h>
#include <avalonsocket.h>
#include "jWrite.h"

#define RECEIVEBUFFER_LENGTH 1024
#define TXT_NAME "avalon-serverplugin"
#define BUFFERMAX 1024  //socketbufferlength
#define PORT 8972

pthread_t TID;  //thread identifier
pthread_mutex_t MXQ; /* mutex used as quit flag */


void free_tokens(int tokencount, char **tokens)
{
    for (size_t i=0; i<tokencount; i++)
        free(tokens[i]);
    free(tokens);
}

char *myParser(PIFilter *iFilter, char *recbuffer)
{
    // retmsg op de heap want anders raakt ie de inhoud kwijt als de functie van de stack is.. logisch toch?!?!
    char *retmsg = calloc(BUFFERMAX, sizeof(char)); // empty resultstring
    char **tokens = (char**) malloc(sizeof(char*)*64); // room for 64 tokens should be enough
    int tokencount  = tokenize(tokens, recbuffer, ' ', '\\');
    if (tokencount > 2) {
        if (strcmp("set", tokens[0]) == 0) {
            // set userstring
            if (tokencount == 4) {
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
            // also free content of tokens:
            free_tokens(tokencount, tokens);
            strcpy(retmsg, "TVWriteUserString succesfully written");
            return retmsg;
        }
        if (strcmp("get", tokens[0]) == 0) {
            if (strcmp("projectID", tokens[1]) == 0) {
                const char *cmd = "tv_ProjectCurrentID";
                char *result = calloc(BUFFERMAX, sizeof(char));
                int ret = TVSendCmd(iFilter, cmd, result); //(buffoverflow risk? fixlater)
                printf("ret = %d\n", ret);
                printf("result: %s\n", result);
                strncpy(retmsg, result, strlen(result) + 1);
                free(result);
                free_tokens(tokencount, tokens);
                return retmsg;
            }
        }
    }
    strcpy(retmsg, "syntax error: \n");
    free_tokens(tokencount, tokens);
    return retmsg;
}

// serverloop function that runs on  another thread
void *listener(void *varg)
{
    PIFilter* iFilter = varg;
    printf("Now running in a thread: %s\n", iFilter->ProgName);
    
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
    server.sin_port = htons( PORT );
     
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
        
        while( (client_sock = accept(socket_fd, (struct sockaddr *)&client, (socklen_t*)&c)) )
        {
            log_info("Connection accepted");
            ssize_t read_size = 0;
            char buff[BUFFERMAX];
            memset(&buff, 0, BUFFERMAX);
            while((read_size = recv(client_sock , buff , BUFFERMAX , 0)) > 0 ) {
                // read the message from client and copy it in buffer
                //read(client_sock, buff, sizeof(buff));
                // print buffer which contains the client contents
                printf("readsize: %lu\n", read_size);
                printf("parsing from client: %s\n", buff);
                char *returnmessage = myParser(iFilter, buff);
                // clean receiving-buffer
                memset(&buff, 0, BUFFERMAX);
                // and send that buffer to client
                write(client_sock, returnmessage, strlen(returnmessage)+1);
                debug("free returnmessage");
                fflush(stdout);
                free(returnmessage);
                debug("freed returnmessage");
                fflush(stdout);
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
        }
    }
    debug("closing main socket");
    close(socket_fd);
}

int start_serverthread(PIFilter* iFilter)
{

  /* init and lock the mutex before creating the thread.  As long as the
     mutex stays locked, the thread should keep running.  A pointer to the
     mutex is passed as the argument to the thread function. */

    // set a buffer for sharing data and return pointer adress

    pthread_mutex_init(&MXQ, NULL);
    pthread_mutex_lock(&MXQ);
    pthread_create(&TID, NULL, listener, iFilter);
    // but is never closes
    //close(sockfd);
    if ( ! pthread_detach(TID) ) {
        printf("Thread detached successfully !!!\n");
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

int FAR PASCAL
PI_Parameters( PIFilter* iFilter, char* iArg )
{  
    // plugin doet verder niks met de gebruiker vanuit TVPaint dus laat maar zitten dit
    return 1;

}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

/**************************************************************************************/
// Function called FIRST for the initialization of the filter

int FAR PASCAL
PI_Open( PIFilter* iFilter )
{

    strcpy( iFilter->PIName, TXT_NAME );
    iFilter->PIVersion  = 1;
    iFilter->PIRevision = 0;

    // If this plugin was the one open at TVPaint shutdown, re-open it
    //TVReadUserString( iFilter, iFilter->PIName, "Open", tmp, "0", 255 );
    //if( atoi( tmp ) )
    //{
    //    TVWarning(iFilter, "okiedokie, ik was al open jajajaja");
    //}
    debug("starting avalon-server-plugin");
    int ret = start_serverthread(iFilter);
    debug("avalon-serverplugin is now running and accepting commands.");
    return  1; // everything went well

}


/**************************************************************************************/
// Aura shutdown: we make all the necessary cleanup

void FAR PASCAL
PI_Close( PIFilter* iFilter )
{
    debug("Cleanup...");
    pthread_mutex_unlock(&MXQ); 
    pthread_join(TID,NULL);
    // close(sockfd);
    
}


/**************************************************************************************/

