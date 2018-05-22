#define PIPE_NAME "\\\\.\\pipe\\antiransom-proxy-pipe"
#define BUFSIZE 512
#define HEADERSIZE 12

typedef struct {
	char chunks[8];
	char last[4];
	char ter;
} HEADER;




void communication__zfill(char*str, int pad);
int communication__receive_single_message(CHANDLE* handle, int buf_len, char *buf);
int communication__send_single_message(CHANDLE* handle, long size, char*data);
