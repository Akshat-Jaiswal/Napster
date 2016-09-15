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
	     int opt=true;
	     serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	     if (serverSocket < 0){
	    	 	 return ERROR_CREAT_SOCKET;
	     }
	     if( setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
	     {
	         perror("setsockopt");
	         exit(EXIT_FAILURE);
	     }

	     //type of socket created

	     //bind the socket to localhost port 8888

	 	printf("Listener on port %d \n", port);

	     //try to specify maximum of 3 pending connections for the master socket


	     bzero((char *) &serv_addr, sizeof(serv_addr));
	     serv_addr.sin_family = AF_INET;
	     serv_addr.sin_addr.s_addr = INADDR_ANY;
	     serv_addr.sin_port = htons(port);
	     if (bind(serverSocket, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
	    	 	 perror("bind");
	    	 	 return ERROR_ON_BINDING;
	     }
	     if (listen(serverSocket, queue) < 0){
	     	         perror("listen");
	     	         exit(EXIT_FAILURE);
	     }
return SUCCESS;
}
int Server::stopServer(){
	close(serverSocket);
	return SUCCESS;
}
int Server::startServices(){
	int addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
	int max_sd,code;
	fd_set readfds;
	struct sockaddr_in address;
	//char buffer[1025];//data buffer of 1K
	char buffer[HEADER_SIZE],field[HEADER_SIZE];
	if(serverSocket<0)
		return START_SERVER_FIRST;
	/*
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
	/*
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
	*/
	//initialise all client_socket[] to 0 so not checked
	for (i = 0; i < max_clients; i++) {
	        client_socket[i] = 0;
	}
	addrlen = sizeof(address);
	puts("Waiting for connections ...");

	while(true) {
	//clear the socket set
		FD_ZERO(&readfds);

	//add master socket to set
	FD_SET(serverSocket, &readfds);
	max_sd = serverSocket;
		//add child sockets to set
		for ( i = 0 ; i < max_clients ; i++) {
			//socket descriptor
			sd = client_socket[i];

			//if valid socket descriptor then add to read list
			if(sd > 0)
			FD_SET( sd , &readfds);
	        //highest file descriptor number, need it for the select function
	            if(sd > max_sd)
	                max_sd = sd;
	    }
	        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
	        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	        if ((activity < 0) && (errno!=EINTR))
	        {
	            printf("select error");
	        }

	        //If something happened on the master socket , then its an incoming connection
	        if (FD_ISSET(serverSocket, &readfds))
	        {
	            if ((new_socket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
	            {
	                perror("accept");
	                exit(EXIT_FAILURE);
	            }

	            //inform user of socket number - used in send and receive commands
	            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
	            //add new socket to array of sockets
	            for (i = 0; i < max_clients; i++)
	            {
	                //if position is empty
	                if( client_socket[i] == 0 )
	                {
	                    client_socket[i] = new_socket;
	                    printf("Adding to list of sockets as %d\n" , i);
	                    break;
	                }
	            }
	        }

	        //else its some IO operation on some other socket :)
	        for (i = 0; i < max_clients; i++)
	        {
	            sd = client_socket[i];

	            if (FD_ISSET( sd , &readfds))
	            {
	                //Check if it was for closing , and also read the incoming message
	                if ((valread = read( sd , buffer, sizeof(buffer))) == 0)
	                {
	                    //Somebody disconnected , get his details and print
	                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
	                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
	                    //Close the socket and mark as 0 in list for reuse
	                    close( sd );
	                    client_socket[i] = 0;
	                }

	                //Handle the request according to the operation requested
	                else
	                {
	                		sscanf(buffer,"%d",&code);
	                		strcpy(field,&(buffer[4]));
	                		bzero(buffer,sizeof(buffer));
                			// get his details and store them
	                		getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
	                		if(code==OP_JOIN){
	                			sh.handleJoin(sd,inet_ntoa(address.sin_addr));
	                		}else if(code==OP_PUBLISH){
	                			sh.handlePublish(sd,field,inet_ntoa(address.sin_addr));
	                		}else if(code==OP_FETCH){
	                			sh.handleFetch(sd,field);
	                		}
	                }
	            }
	        }
	    }return SUCCESS;
}
