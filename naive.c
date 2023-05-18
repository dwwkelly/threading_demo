#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "prime_factor.h"

#define BUFLEN 64
#define PORT 5000

int main()
{
    struct sockaddr_in si_me, si_other;

	int sock, i, recv_len;
        uint32_t slen = sizeof(si_other);
	char buf[BUFLEN];

	//create a UDP socket
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));

	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind socket to port
	bind(sock , (struct sockaddr*)&si_me, sizeof(si_me));

	//keep listening for data
	while(1)
	{
		fflush(stdout);

		//try to receive some data, this is a blocking call
		recv_len = recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);

                uint64_t input = (uint64_t) buf;
                uint64_t output = prime_factor(input);

                // printf("input = %llu, output = %llu\n", input, output);

		//now reply the client with the same data
		// sendto(sock, buf, recv_len, 0, (struct sockaddr*) &si_other, slen);
	}

	close(sock);
	return 0;

}
