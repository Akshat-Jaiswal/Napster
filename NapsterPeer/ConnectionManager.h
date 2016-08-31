/*
 * ConnectionManager.h
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_


#include "codes.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

class ConnectionManager {
private:
	int code;
public:
	ConnectionManager();
	int connectTo(char* ip,int port);
	int disconnect(int socket);
	char* getErrorMessage();
	virtual ~ConnectionManager();
};

#endif /* CONNECTIONMANAGER_H_ */
