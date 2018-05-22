#include <conio.h>
#include <tchar.h>
#include <windows.h>
#include "communication_external.h"
#include "communication_internal.h"

#define EXIT_IF_FAILED if(status) return status; 




void communication__zfill(char*str, int pad) //approved
{	//padding 0s to beginning of string until the size of the string is the pad arguemnt. the function assums there is enough memory
	int len = strlen(str), to_add, i;
	if(len >=pad)
		return;
	to_add = pad-len;
	
	for(i=len-1;i>=0;i--) //shifting the string
		*(str+to_add+i) = *(str+i);

	for(i=0;i<to_add;i++)//define the start as 000...
		*(str+i) = '0';
	
}


int communication__try_accept_connection(CHANDLE** h, CHANDLE**client) //approved
{	//Server-side function return -1 if used on client
	//try to accept connection if everything is fine return 0 
	//if there isn't any connection avalibale return 1
	//if the client disconnected and the server didn't use disconnect 2
	//if there is a connection problem return 3
	int status;
	CHANDLE* tmp_h;
	if(((*h)->type)=='c')
		return -1;
	*client = *h;
	ConnectNamedPipe((*h)->h_pipe, NULL);
	status = GetLastError();
	switch(status)
	{
		case ERROR_PIPE_LISTENING:
			return 1;
		case ERROR_NO_DATA:
			return 2;
		case ERROR_PIPE_CONNECTED:
			*h = communication__create((*h)->type, (*h)->source);
			return 0;
		default:
			return 3;
	}
}

CHANDLE* communication__create(char type, char source)
{	// create new communication handle
	//type - server or client
	//source - identify the source of the communication this argument identify the client
	//a client must include the source argument and a server would enter it as 0
	
	int fsuccess,i, size = sizeof(CHANDLE);
	CHANDLE *p = (CHANDLE*) malloc(size); //alloc new handle
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	
	//initialize the handle
	p->type = type;
	p->source = source;
	
	if(type == 's')
	{
		//create server
		p->h_pipe = CreateNamedPipe(PIPE_NAME,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
		PIPE_UNLIMITED_INSTANCES,
		BUFSIZE,
		BUFSIZE,
		0, NULL);
		if(p->h_pipe == INVALID_HANDLE_VALUE)
		{
			communication__close(p);
			return 0;
		}
		
	}
	else
	{
		
		//create client
		if(source == 0) // source is'nt defined
			return 0;
		while (1) 
		{
			p->h_pipe = CreateFile(PIPE_NAME,GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			
			if (p->h_pipe != INVALID_HANDLE_VALUE) 
				break;
			
			if (GetLastError() != ERROR_PIPE_BUSY)
			{
				communication__close(p);
				return 0;
			}
			if (!WaitNamedPipe(PIPE_NAME, 1000)) 
			{
				communication__close(p);
				return 0; 
					
			}
				
		}
		//change to message mode 
		dwMode = PIPE_READMODE_MESSAGE;
	    fsuccess = SetNamedPipeHandleState(p->h_pipe, &dwMode, NULL, NULL);
		if (!fsuccess) 
			return 0;
	
	}
		
	return p;
}


int communication__send_single_message(CHANDLE* handle, long size, char*data)
{	// send data to the other side of the pipe return 0 for success and 1 for failure
	int fsuccess;
	long byWritten;
	
	fsuccess = WriteFile(handle->h_pipe, data, size , &byWritten, NULL);//send data
	
	if (!fsuccess || size != byWritten)// check if failed
		return 1;
	return 0;
	
}

void communication__close(CHANDLE* h)
{	//clean all the data for this object

	if (h->type == 's')
		DisconnectNamedPipe(h);
	else
		;//disconnect from the client side
	CloseHandle(h->h_pipe);
	free(h);
	
}


int communication__receive_single_message(CHANDLE* handle, int buf_len, char *buf)
{	//receive one message
	//read message from the other side of the pipe into buf
	//return value

	//return 0 - success
	//return 1 - no data yet
	//return 2 - failue

	int fsuccess, error;
	long byRead;
	
	fsuccess = ReadFile(handle->h_pipe, buf, buf_len, &byRead, NULL);
	error = GetLastError();
	if(error == ERROR_NO_DATA)
		return 1;
	if (!fsuccess || byRead == 0)
		return 2;
	return 0;
}


int communication__send(CHANDLE* handle, long size, char *buf)
{	//send the message according to the protocol
	//return value
	//0 - success
	//1 - failure
	int i, status;
	HEADER head; 
	const int chunks = size/BUFSIZE, last_chunk = size%BUFSIZE;

	//send fragmentation header
	itoa(chunks, head.chunks, 10);//number of chunks
	communication__zfill(head.chunks,8);
	
	itoa(last_chunk, head.last, 10);//size of the last chunk
	communication__zfill(head.last, 4);
	
	status = communication__send_single_message(handle, HEADERSIZE, (char*) &head);
	EXIT_IF_FAILED
	
	//send fragments
	for(i=0; i< chunks; i++)
	{
		communication__send_single_message(handle, BUFSIZE, buf + (i*BUFSIZE));
		EXIT_IF_FAILED
	}
	
		
	//send last fragment
	communication__send_single_message(handle, last_chunk, buf+chunks*BUFSIZE);
	EXIT_IF_FAILED


	return 0;
}


int communication__get_message_length(CHANDLE* handle, long *len)
{  	//receive the length of the message
	//return value
	//0 - success 
	//1 - no data yet
	//2 - failed
	int status, chunks, last;
	HEADER head;
	char tmp;
	
	status = communication__receive_single_message(handle, HEADERSIZE, (char*) &head);
	EXIT_IF_FAILED
	tmp = head.last[0];
	head.last[0]=0;
	chunks = atoi(head.chunks);
	head.ter = 0;
	head.last[0] = tmp;
	last = atoi(head.last);
	*len = chunks * BUFSIZE + last;
	return 0;
	
}


int communication__receive(CHANDLE* handle, char*buf, long length)
{	//receive the the message fregment by fregment
	//return value:
	// 0 - success
	// 1 - no data yet
	// 2 - failure
	int chunks = length / BUFSIZE, last = length % BUFSIZE, i, status;
	
	if(length<BUFSIZE)//if only one fragment
	{
		return communication__receive_single_message(handle, length, buf);
	}		
	
	
	for(i=0;i<chunks;i++)
	{
		status = communication__receive_single_message(handle, BUFSIZE, (buf + i*BUFSIZE));
		EXIT_IF_FAILED
	}
	return communication__receive_single_message(handle, last, (buf +chunks*BUFSIZE));
	
}



