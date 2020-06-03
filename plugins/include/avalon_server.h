#ifndef __AVALONSERVER__
#define __AVALONSERVER__

#include "plugdllx.h"
#include "plugx.h"


void free_tokens(int tokencount, char **tokens);

void sendCommand(PIFilter *iFilter, const char* cmd, char* result);

char *myParser(PIFilter *iFilter, char *recbuffer, size_t response_bufferlength);


void listener(PIFilter* iFilter);

#endif
