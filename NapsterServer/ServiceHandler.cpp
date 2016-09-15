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
	//close(clientSocket);
	return SUCCESS;
}
int ServiceHandler::handlePublish(int clientSocket,char *filename,char* ip){
	// add an entry in the database
	db.addFile(ip,filename);
	// reply back to client for successful publish
	char buffer[HEADER_SIZE];
	bzero(buffer,sizeof(buffer));
	sprintf(buffer,"%d Publish Operation Successful",SUCCESS);
	write(clientSocket,buffer,sizeof(buffer));
	// close the client connection for now for testing purpose
	std::cout<<"Peer:"<<ip<<" Published: "<<filename<<"\n";
	//close(clientSocket);
	return SUCCESS;
}
int ServiceHandler::handleFetch(int clientSocket,char* filename){
	// find the ip address for the file

	char *ip=NULL,message[HEADER_SIZE];
	int code=db.searchFile(filename,&ip);
	// build a header and send it to client
	char buffer[HEADER_SIZE];
	bzero(buffer,sizeof(buffer));
	sprintf(buffer,"%d %s",code,ip);
	write(clientSocket,buffer,sizeof(buffer));
	// close the client connection for now for testing purpose
	std::cout<<"Query: "<<filename<<" Answer: "<<ip<<"\n";
	//close(clientSocket);
	return SUCCESS;
}
