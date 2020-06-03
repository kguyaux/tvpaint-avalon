/*
 * Avalon-publish plugin for TVPaint
 *
 * */


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

#define EXTERN
#include "avalon_vars.h"

#define TXT_NAME "tvp_avalon_publish"


int FAR PASCAL
PI_Parameters( PIFilter* iFilter, char* iArg )
{  
    /*
    char result[1024] = {0};
    // block TVPaint
    sendCommand(iFilter, "tv_LockDisplay",  result);
    */
    char* request = "OPEN publish";

    // send request to avalon-host, which should open the worfiles UI, in thsi case:
    char response[RESPONSEBUFFER_LENGTH] = {0};
    int ret = sendReceiveMessage(request, response, 0);

    if (ret <0) {
        log_err("Request for opening workfiles has failed.\n");
    } else {
        // keep listening for commands from the Avalon-host, until finish-cmd is sent..
        listener(iFilter);
    }
    /*
    // unblock TVpaint
    sendCommand(iFilter, "tv_UnLockDisplay", result);
    */
    return 1;

}


////////////////////////////////////////////////////////////////////////////////////////
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

    return  1; // everything went well
}


/**************************************************************************************/
// Aura shutdown: we make all the necessary cleanup

void FAR PASCAL
PI_Close( PIFilter* iFilter )
{
    //niks dus ff
}


/**************************************************************************************/


