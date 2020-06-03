/*
A very simple plugin that only sends a message(to pythonwrapper) when tvp starts
..and a message when tvp stops
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
#include "avalon_server.h"

// this is a trick to use global varaibles for all the code involved(see the headerfile..)
#define EXTERN
#include "avalon_vars.h"


#define TXT_NAME "tvp_avalon_startstop"
    




/**************************************************************************************/
// Function called FIRST for the initialization of the filter
int FAR PASCAL
PI_Open( PIFilter* iFilter )
{
    strcpy( iFilter->PIName, TXT_NAME );
    iFilter->PIVersion  = 1;
    iFilter->PIRevision = 0;

    char* request = "START";
    char response[RESPONSEBUFFER_LENGTH] = {0};
    int ret = sendReceiveMessage(request, response, 5);
    if (ret <0)
        log_err("Could not connect to pytvpaint-wrapper!");
    else
        printf("Response from wrapper: %s\n", response);
     

    return  1; // everything went well
}


/**************************************************************************************/
// Aura shutdown: we make all the necessary cleanup

void FAR PASCAL
PI_Close( PIFilter* iFilter )
{
    char* request = "END";
    char response[RESPONSEBUFFER_LENGTH] = {0};
    int ret = sendReceiveMessage(request, response, 5);
    if (ret <0)
        log_err("Shutting down TVPaints python-wrapper did not go as planned..");
    else
        printf("Response from wrapper: %s\n", response);
     
    //niks dus ff
}


/**************************************************************************************/

