#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include <fcntl.h>

#include<sys/types.h>
#include<sys/socket.h>
#include <sys/stat.h>

#include<netinet/in.h>
void usage(){
	printf("Usage: client source_path destination_path\n");
	exit(1);
}
int main(int args, char* argv[]){

	/*Check if user has passed correct number of arguments*/
	if(args < 3){
		printf("ERROR:Please enter correct number of arguments\n");
		usage();
	}

	/*Check if the destination path is valid directory*/
	struct stat fileStat;
	if(stat(argv[2],&fileStat) < 0){
		printf("ERROR:%s is not a valid path\n",argv[2]);
		exit(1);
	}
	else{
		if(!(S_ISDIR(fileStat.st_mode))){
			printf("ERROR:%s is not a directory\n",argv[2]);
			exit(1);
		}
	}  
	//Creating socket
	int sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd == -1){
		perror("ERROR:Error occured while creating socket\n");
		exit(1);
	}

	//Specify an address to connect to
	struct sockaddr_in server_address;
	memset(&server_address,0,sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9000);  
	server_address.sin_addr.s_addr = INADDR_ANY;
	printf("before connect in client\n");
	//Actual connection to the server
	if(connect(sockFd,(struct sockaddr *)&server_address,sizeof(server_address)) == -1){
		perror("ERROR:Error connecting to server");
		close(sockFd);
		exit(1);
	}

	//Creating file to write data into recieved from the server
	char file_name[1024]={'\0'};
	strcat(file_name,argv[2]);
	strcat(file_name,strrchr(argv[1],'/'));
	int fd = open(file_name,O_APPEND|O_CREAT); 

	//Send the name of the file to be copied to server
	send(sockFd,argv[1],strlen(argv[1]),0);

	//Recieving data from server
	char server_response[1024]={'\0'};
	int data_size_rcvd = recv(sockFd,&server_response,sizeof(server_response),0);

	printf("We have recieved the following response size from server:\n%d\n",data_size_rcvd);
	printf("We have recieved the following response from server:\n%s\n",server_response);
	close(sockFd);
	return (0);
}
