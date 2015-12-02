#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/time.h>
#define TALK_PORT 100
#define TIMEOUT_SEC 1
#define TIMEOUT_USEC 0
#define TRUE 1
#define FALSE 0
#define MAX_MESSAGE_LEN 255
struct sockaddr_in peer_addr_sock;
void vm_udp_talk_bootstrap(int argc, char *argv[], 
			  int *rsock, int *ssock)
{
    struct hostent     *peer_addr;
    struct sockaddr_in my_addr;

    if (argc < 2) {
	perror("Few Arguments please enter the IP");
	exit (1);
    }
    peer_addr = gethostbyname(argv[1]);
    peer_addr_sock.sin_family = AF_INET;
    bcopy((char *)peer_addr->h_addr,
	  (char *)&peer_addr_sock.sin_addr.s_addr,
	  peer_addr->h_length);
    peer_addr_sock.sin_port = htons(TALK_PORT);

    bzero((char *) &my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(TALK_PORT);

    if ((*rsock = socket (AF_INET, SOCK_DGRAM,0)) < 0){
	perror ("receive socket");
	exit (1);
    } 
//    fcntl (*rsock, F_SETFL, O_NONBLOCK);
    if ((bind (*rsock, 
	       (struct sockaddr *)&my_addr,
	       sizeof (struct sockaddr))) < 0) {
	perror ("bind");
	exit(1);
    }
    if ((*ssock = socket (AF_INET, SOCK_DGRAM,0)) < 0){
	perror ("send socket");
	exit (1);
    }

    return;
}
int vm_udp_talk_recv(int rsock)
{
    struct timeval     timeout;
    fd_set             socket_set;
    struct sockaddr_in recv_addr;
    socklen_t          recv_addr_len;
    int                numfd;
    char               buf[MAX_MESSAGE_LEN];
    int                ret_val;

    FD_ZERO(&socket_set);
    FD_SET(rsock, &socket_set);

    timeout.tv_sec  = TIMEOUT_SEC;
    timeout.tv_usec = TIMEOUT_USEC;

    if ((numfd = select(rsock+1, &socket_set, NULL, NULL,
			&timeout))< 0) {
	return -1;
    }
    if (numfd == 0) {
	return 0;
    }
    if (numfd) {
	recv_addr_len = sizeof(struct sockaddr_in);
	ret_val = recvfrom(rsock, buf, MAX_MESSAGE_LEN, 
			   0, (struct sockaddr *)&recv_addr,
			   &recv_addr_len);
	printf("Recieved from Xinu: %s\n", buf);
	return ret_val;
    }
    return 1;
}
int vm_udp_talk_send(int ssock)
{
    char buf[MAX_MESSAGE_LEN];
    printf("Enter the message to send:\n");
    gets(buf);
    if (sendto(ssock, buf, MAX_MESSAGE_LEN, 0,
	       (struct sockaddr *)&peer_addr_sock, 
	       sizeof(struct sockaddr)) < 0) {
	perror("sendto: ");
	exit(1);
    }
    return 0;
}
int main(int argc, char *argv[])
{
    int            rsock, ssock;
    
    vm_udp_talk_bootstrap(argc, argv, &rsock, &ssock);
    while (TRUE) {
	vm_udp_talk_recv(rsock);
	vm_udp_talk_send(rsock);
    }
    return 0;

}
