#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<time.h>

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
}packet;

void INIT()
{
    packet.HEADER.btcp_sport = 29;
    packet.HEADER.btcp_dport = 31;
    packet.HEADER.data_off = 7;
    packet.HEADER.win_size = 10;
    packet.HEADER.btcp_seq = 0;
    packet.HEADER.btcp_ack = 0;
    packet.HEADER.flag = 0;
    return;
}

void RECV(FILE* file)
{
	INIT();
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
    struct sockaddr_in client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sockfd,2);
    int addr_len = sizeof(client_addr);
    int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&addr_len);
    int ACK = 0;
    while(1)
    {
    	struct packets packet;
    	int NUM = read(clientfd, &packet, 64+7);
    	if(packet.HEADER.btcp_seq == ACK)
    	{
    		packet.HEADER.btcp_ack = ACK;
    		send(clientfd, &packet,7,0);// overwrite ACK and send the header back
    		ACK = (ACK + 1) % 256;
    		int j = 0;
    		while(j < NUM - 7)
    		{
    			fwrite(&packet.DATA[j],sizeof(unsigned char),1,file);
    			j++;
			}
			if(NUM != 71)
			{
				fclose(file);
				close(sockfd);
				return;
			}
		}
	}
	return;
}


int main()
{
    FILE* file = fopen("output.bin","wb");
    RECV(file);
    return 0;
}

