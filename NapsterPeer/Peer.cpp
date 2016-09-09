/*
 * Peer.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#include "Peer.h"

Peer::Peer(){
	// TODO Auto-generated constructor stub
	serverSocket=-1;
	port=0;
	ipAddress='\0';
	// create a default directory for storing files if it doesn't exist

	struct stat st = {0};
	if (stat("~/Desktop/p2p_files", &st) == -1) {
	    mkdir("~/Desktop/p2p_files", 0700);
	}
	std::system("mkdir ~/Desktop/p2p_files 2>/dev/null");
}

Peer::~Peer() {
	// TODO Auto-generated destructor stub
	//close the connection
	close(serverSocket);
	close(uploadSocket);
}
int Peer::join(char *ip,int port){
	int code;
	ConnectionManager cm;
	serverSocket=cm.connectTo(ip,port);
	if(serverSocket<0){
		perror("Error Connecting To Server");
		return JOIN_FAILED;
	}
	// send a join request
	char buffer[HEADER_SIZE];
	bzero(buffer,sizeof(buffer));
	// build a header
	sprintf(buffer,"%d",OP_JOIN);
	//send the header
	if(write(serverSocket,buffer,sizeof(buffer))<0){
		//write failed
		perror("Error Writing To Server");
		return JOIN_FAILED;
	}
	bzero(buffer,sizeof(buffer));
	if(read(serverSocket,buffer,sizeof(buffer))<0){
		perror("Error Reading From Server");
		return JOIN_FAILED;
	}
	// parse the header returned
	sscanf(buffer,"%d",&code);
	return code;
}
int Peer::publish(char* filepath){
	int code;
	char* filename,command[100];
	if(serverSocket<0){
			perror("Error Connecting To Server");
			return PUBLISH_FAILED;
	}
	// extract filename from filepath
	filename=basename(filepath);
	sprintf(command,"ln --symbolic %s ~/Desktop/p2p_files/%s",filepath,filename);
	// create a symbolic link for the file being published
	std::system(command);
	// send a publish request
		char buffer[HEADER_SIZE];
		bzero(buffer,sizeof(buffer));
		// build a header
		sprintf(buffer,"%d %s",OP_PUBLISH,filename);
		//send the header
		if(write(serverSocket,buffer,sizeof(buffer))<0){
			//write failed
			perror("Error Writing To Server");
			return JOIN_FAILED;
		}
		bzero(buffer,sizeof(buffer));
		if(read(serverSocket,buffer,sizeof(buffer))<0){
			perror("Error Reading From Server");
			return JOIN_FAILED;
		}
		// parse the header returned
		sscanf(buffer,"%d",&code);
		return code;
}
int Peer::fetch(char* filename){
	int code;
		if(serverSocket<0){
				perror("Error Connecting To Server");
				return PUBLISH_FAILED;
		}
		// send a publish request
			char buffer[HEADER_SIZE];
			bzero(buffer,sizeof(buffer));
			// build a header
			sprintf(buffer,"%d %s",OP_FETCH,filename);
			//send the header
			if(write(serverSocket,buffer,sizeof(buffer))<0){
				//write failed
				perror("Error Writing To Server");
				return JOIN_FAILED;
			}
			bzero(buffer,sizeof(buffer));
			if(read(serverSocket,buffer,sizeof(buffer))<0){
				perror("Error Reading From Server");
				return JOIN_FAILED;
			}
			// parse the header returned
			sscanf(buffer,"%d",&code);
			printf("%d %s\n",code,&(buffer[4]));
			return code;
}
int Peer::start_upload_server(int port,int queue){

    struct sockaddr_in serv_addr;
    int opt=true;
    uploadSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (uploadSocket < 0){
   	 	 return ERROR_CREAT_SOCKET;
    }
    if( setsockopt(uploadSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created

	//printf("Listener on port %d \n", port);

    //try to specify maximum of 3 pending connections for the master socket


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(uploadSocket, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
   	 	 perror("bind");
   	 	 return ERROR_ON_BINDING;
    }
    if (listen(uploadSocket, queue) < 0){
    	         perror("listen");
    	         exit(EXIT_FAILURE);
    }
	return SUCCESS;
}
int Peer::start_upload_service(){
	int i=fork();
	int addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
	struct sockaddr_in address;
	// start the upload service as a seperate process
	if(i==0){
		// inside the child process set the uploadServer to accept connections
		while(1){
		 if ((new_socket = accept(uploadSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		 	{
		 	               perror("accept");
		 	               exit(EXIT_FAILURE);
		 	}

		    if (!fork()) { // this is the child process of child
		        close(uploadSocket); // child doesn't need the listener
		        close(new_socket);
		        exit(0);
		    }
		    close(new_fd);
		}

	}
	//close the socket for this master process
	close(uploadSocket);
	return SUCCESS;
}
