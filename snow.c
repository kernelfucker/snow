/* See LICENSE file for license details */
/* snow - port scanner */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define version "0.2"

const int af = AF_INET;
const int ss = SOCK_STREAM;
const int optl = SOL_SOCKET;
const int srcv = SO_RCVTIMEO;
const int ssnd = SO_SNDTIMEO;

int scport(const char *ip, int port){
	int sockfd;
	struct sockaddr_in t;
	sockfd = socket(af, ss, 0);
	if(sockfd < 0) return 0;

	memset(&t, 0, sizeof(t));
	t.sin_family = af;
	t.sin_port = htons(port);
	inet_pton(af, ip, &t.sin_addr);

	struct timeval tout = {1, 0};
	setsockopt(sockfd, optl, srcv, &tout, sizeof(tout));
	setsockopt(sockfd, optl, ssnd, &tout, sizeof(tout));

	int r = connect(sockfd, (struct sockaddr *)&t, sizeof(t));
	close(sockfd);

	return(r == 0);
}

void usage(const char *snow){
	printf("usage: %s [ip] [input-port] [output-port]\n", snow);
	printf("try '%s -h' for more information\n", snow);
}

void help(const char *snow){
	printf("usage: %s [options]..\n", snow);
	printf("options:\n");
	printf("  -h	display this\n");
	printf("  -v	show version information\n");
	exit(1);
}

void show_version(){
	printf("snow-%s\n", version);
	exit(1);
}

int main(int argc, char *argv[]){
	if(argc == 2){
		if(strcmp(argv[1], "-h") == 0){
			help(argv[0]);
		}

		if(strcmp(argv[1], "-v") == 0){
			show_version();
		}
	}

	if(argc != 4){
		usage(argv[0]);
		return 1;
	}

	const char *ip = argv[1];
	int input_port = atoi(argv[2]);
	int output_port = atoi(argv[3]);
	if(input_port <= 0 || output_port <0 || output_port < input_port){
		fprintf(stderr, "invalid port range mismatch\n");
		return 1;
	}

	printf("scanning %s from port %d to %d\n", ip, input_port, output_port);

	for(int port = input_port; port <= output_port; port++){
		int r = scport(ip, port);
		printf("port %d -> %s\n", port, r ? "open" : "closed");
	}

	printf("scan completed\n");
	return 0;
}
