/*
    Simple udp server
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include <unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <sys/time.h>
 
#define BUFLEN 10000// length of buffer

 
void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(int argc, char **argv)
{
    struct sockaddr_in si_me, si_other;
    int s, i, slen = sizeof(si_other) , recv_len, received;
    char buf[BUFLEN];
    if (argc < 2) {
        printf("podaj numer portu jako parametr (provide port number in command line)\n");
        return 1;
    }
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(atoi(argv[1]));
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    //keep listening for data
    for(i=0; i<100; i++)
    {
        //printf("Waiting for data...");
        fflush(stdout);
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        //printf("Data: %s\n" , buf);
         
        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }
	close(s);

    return 0;
}
