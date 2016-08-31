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
}

Peer::~Peer() {
	// TODO Auto-generated destructor stub
	//close the connection
	close(serverSocket);
}
int Peer::join(char *ip,int port){
	int code;
	ConnectionManager cm;
	serverSocket=cm.connectTo(ip,port);
	if(serverSocket<0){
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
		return JOIN_FAILED;
	}
	bzero(buffer,sizeof(buffer));
	if(read(serverSocket,buffer,sizeof(buffer))<0){
		return JOIN_FAILED;
	}
	// parse the header returned
	sscanf(buffer,"%d",&code);
	return code;
}

