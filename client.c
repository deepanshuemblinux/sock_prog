#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>

int main(){
	/*Creating socket*/
	int sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd == -1){
		perror("Error occured while creating socket\n");
		exit(1);
	}

	/*Specify an address to connect to*/
	struct sockaddr_in server_address;
	memset(&server_address,0,sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);  
	server_address.sin_addr.s_addr = INADDR_ANY;

	/*Actual connection to the server*/
	if(connect(sockFd,(struct sockaddr *)&server_address,sizeof(server_address)) == -1){
		perror("Error connecting to server");
		close(sockFd);
		exit(1);
	}

	/*Recieving data from server*/
	char server_response[1024]={'\0'};
	recv(sockFd,&server_response,sizeof(server_response),0);

	printf("We have recieved the following response from server:\n%s\n",server_response);
	close(sockFd);
	return (0);
}
