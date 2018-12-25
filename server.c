#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>

#include<netinet/in.h>

int main(){
	char file_name[1024] = {'\0'};
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
	server_address.sin_port = htons(9000);  
	server_address.sin_addr.s_addr = INADDR_ANY;

	/*Bind the socket to particular IP and port number*/
	bind(sockFd_server,(struct sockaddr *)&server_address,sizeof(server_address));

	/*Start listening to the requests from clients*/
	listen(sockFd_server,5);
	int sockFd_client;
	/*Accept the connection requests from the client*/
	sockFd_client = accept(sockFd_server,NULL,NULL);

	//Recieve the name of the file to be copied to client
	recv(sockFd_client,&file_name,sizeof(file_name),0);

	//Open the file to be copied and start sending the data

	char server_msg[1024] = {'\0'};
	/*Check if the file asked for, exists*/
	struct stat fileStat;
	if(stat(file_name,&fileStat) < 0){
		strcpy(server_msg,"ERROR:not a valid path"); 
		send(sockFd_client,server_msg,strlen(server_msg),0);
	}
	close(sockFd_server);
	close(sockFd_client);
	return (0);
}
