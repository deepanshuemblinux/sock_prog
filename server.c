#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>

int main(){
	char welcome_msg[] = "Welcome to the server";
	/*Creating socket*/
	int sockFd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd_server == -1){
		perror("Error occured while creating socket\n");
		exit(1);
	}

	/*Specify an address to bind the socket to*/
	struct sockaddr_in server_address;
	memset(&server_address,0,sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);  
	server_address.sin_addr.s_addr = INADDR_ANY;

	/*Bind the socket to particular IP and port number*/
	bind(sockFd_server,(struct sockaddr *)&server_address,sizeof(server_address));

	/*Start listening to the requests from clients*/
	listen(sockFd_server,5);

	/*Accept the connection requests from the client*/
	int sockFd_client = accept(sockFd_server,NULL,NULL);

	/*Send the data to client socket */
	send(sockFd_client,welcome_msg,sizeof(welcome_msg),0);
	close(sockFd_server);
	return (0);
}
