/*
 * ServiceHandler.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#include "ServiceHandler.h"

ServiceHandler::ServiceHandler() {
	// TODO Auto-generated constructor stub

}

ServiceHandler::~ServiceHandler() {
	// TODO Auto-generated destructor stub
}
int ServiceHandler::handleJoin(int clientSocket,char* ip){
	// add the ip address to live connections list
	// reply back to client for successful join operation
	db.addPeer(ip, time(0));

	char buffer[HEADER_SIZE];
	bzero(buffer,sizeof(buffer));
	sprintf(buffer,"%d",SUCCESS);
	write(clientSocket,buffer,sizeof(buffer));
	// close the client connection for now for testing purpose
	std::cout<<"Added Peer:"<<ip<<"\n";
	close(clientSocket);
	return SUCCESS;
}

