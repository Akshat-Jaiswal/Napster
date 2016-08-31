/*
 * ConnectionManager.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#include "ConnectionManager.h"

ConnectionManager::ConnectionManager() {
	// TODO Auto-generated constructor stub
	code=SUCCESS;
}

ConnectionManager::~ConnectionManager() {
	// TODO Auto-generated destructor stub
}
int ConnectionManager::connectTo(char *ip,int port){
	int sockfd, portno;
	    struct sockaddr_in serv_addr;
	    portno = port;
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0){
	    	code=SOC_ERROR_CREATION;
	    	return -1;
	    }
	    bzero((char *) &serv_addr, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_addr.s_addr= inet_addr(ip);
	    serv_addr.sin_port = htons(portno);
	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
	    	code=SOC_CONNECTION_REFUSED;
	    	return -2;	    	
	    }
	    else{
	    	code=SUCCESS;
	    	return sockfd;
	    }
	    return 0;
}
int ConnectionManager::disconnect(int socket){
	close(socket);
	return 0;
}
