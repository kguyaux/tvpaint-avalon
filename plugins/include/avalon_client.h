#ifndef __AVALONCLIENT_H__
#define __AVALONCLIENT_H__


int sendReceiveMessage(char *message, char *receivebuffer, int timeoutseconds);
int tokenize(char **result, char* input, char seperator, char escape);

#endif
