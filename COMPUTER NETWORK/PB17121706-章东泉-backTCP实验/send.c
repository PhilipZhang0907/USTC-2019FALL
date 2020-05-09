#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<time.h>

#define window_size 10
#define N 21000000

typedef uint8_t tcp_seq;

struct tcphdr{
	uint8_t btcp_sport;
	uint8_t btcp_dport;
	tcp_seq btcp_seq;
	tcp_seq btcp_ack;
	uint8_t data_off;
	uint8_t win_size;
	uint8_t flag;
}BTcpHeader;

struct packets{
	struct tcphdr HEADER;
	unsigned char DATA[64];
}packet[N];

void INIT(int sockfd, struct sockaddr_in addr)
{
    int i = 0;
    while(i < window_size)
    {
    	packet[i].HEADER.btcp_sport = 31;
    	packet[i].HEADER.btcp_dport = 29;
    	packet[i].HEADER.data_off = 7;
    	packet[i].HEADER.win_size = 10;
    	packet[i].HEADER.btcp_seq = 0;
    	packet[i].HEADER.btcp_ack = 0;
    	packet[i].HEADER.flag = 0;
    	i++;
	}
	return;
}

int UNBLOCK(int fd)
{
    int flags = fcntl (fd, F_GETFL, 0);
    if (flags == -1)
	{
        return -1;
    }
    if(fcntl (fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
        return -1;
    }
    return 0;
}// make receive procedure unblocking, from Internet

void GO_BACK_N(FILE* file, int sockfd, struct sockaddr_in server_addr)
{
	INIT(sockfd, server_addr);
	int i = 0;
	int base = 0;
	int NUM = 64;
	clock_t TIME = clock();
	while(1)
	{
		clock_t time = clock();
		if((double)(time - TIME) / CLOCKS_PER_SEC >= 0.5)
		{
			int j = base;
			while(j <= i)
			{
				packet[j].HEADER.flag = 1;
				if(NUM != 64)
					send(sockfd, packet + j, 7 + NUM, 0);
				else
					send(sockfd, packet + j, 7 + 64, 0);
				j++;
			}
			TIME = time;
		}// resend file
		unsigned char buffer[10];
		int val = recv(sockfd, buffer, 7, MSG_DONTWAIT);
		if(val == 7 && buffer[3] == base % 256)
		{
			base++;
			printf("ACK:%d\n",base);
			TIME = clock();
		}// receive the right ack
		if(i == base + window_size || NUM != 64)
			continue;
		packet[i].HEADER.btcp_seq = i % 256;
        packet[i].HEADER.flag = 0;
		NUM = fread((void*)packet[i].DATA, sizeof(unsigned char), 64, file);
		write(sockfd, packet + i, 7 + NUM);
		i++;
	}
	return;
}

void SEND(FILE* file)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    struct sockaddr_in client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(2000);
    client_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    bind(sockfd,(struct sockaddr *)&client_addr,sizeof(struct sockaddr));
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    UNBLOCK(sockfd);
    GO_BACK_N(file, sockfd, server_addr);
    return;
}

int main()
{
	FILE* file = fopen("input.bin","rb");
	SEND(file);
	return 0;
}


