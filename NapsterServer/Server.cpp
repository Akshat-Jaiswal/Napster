/*
 * Server.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#include "Server.h"

Server::Server() {
	// TODO Auto-generated constructor stub
	serverSocket=-1;
}

Server::~Server() {
	// TODO Auto-generated destructor stub
	stopServer();
}
int Server::startServer(int port,int queue=10){
	     struct sockaddr_in serv_addr;
	     serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	     if (serverSocket < 0){
	    	 	 return ERROR_CREAT_SOCKET;
	     }
	     bzero((char *) &serv_addr, sizeof(serv_addr));
	     serv_addr.sin_family = AF_INET;
	     serv_addr.sin_addr.s_addr = INADDR_ANY;
	     serv_addr.sin_port = htons(port);
	     if (bind(serverSocket, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
	    	 	 return ERROR_ON_BINDING;
	     }
	     listen(serverSocket,queue);
	     return SUCCESS;
}
int Server::stopServer(){
	close(serverSocket);
}
int Server::startServices(){
	char buffer[HEADER_SIZE];
	if(serverSocket<0)
		return START_SERVER_FIRST;
	struct sockaddr_in cli_addr;
	int clientSocket;
	// accept a connection to test the functionality
	socklen_t clilen = sizeof(cli_addr);
	clientSocket = accept(serverSocket,(struct sockaddr *) &cli_addr,&clilen);
	if(clientSocket<0){
		return ERROR_ACCEPTING_CONNECTION;
	}
	// read the header
	bzero(buffer,sizeof(buffer));
		/*
			Start the Handshake
		*/
	     // read the header
	int n,code;
	if((n=read(clientSocket,buffer,sizeof(buffer)))<0){
			perror("Server : Reading Header");
	 }
	// add the delemiter
	//buffer[n]='\0';
	sscanf(buffer,"%d",&code);
	bzero(buffer,sizeof(buffer));
	if(code==OP_JOIN){
		sh.handleJoin(clientSocket,inet_ntoa(cli_addr.sin_addr));
	}
	return SUCCESS;
}
