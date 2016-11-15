#include <sys/types.h>
#include <sys/socket.h>
#define DATA_LEN 18

int listen_packet(int socket, char *buf, int buf_len){
	return recv(socket, buf, DATA_LEN, MSG_WAITALL);
}