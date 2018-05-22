#include <windows.h>


typedef struct st_CHANDLE{
	char type;
	char source;
	HANDLE h_pipe;
} CHANDLE;


CHANDLE* communication__create(char type, char source);
int communication__try_accept_connection(CHANDLE** h, CHANDLE**client);
int communication__send(CHANDLE* handle, long size, char*data);
void communication__close(CHANDLE* h);
int communication__receive(CHANDLE* handle, char*buf, long length);
int communication__get_message_length(CHANDLE* handle, long *len);
