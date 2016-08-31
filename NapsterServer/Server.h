/*
 * Server.h
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#ifndef SERVER_H_
#define SERVER_H_

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
#include "ServiceHandler.h"
class Server {
private:
	int serverSocket;
	ServiceHandler sh;
public:
	Server();
	int startServer(int port,int queue);
	int startServices();
	virtual ~Server();
	int stopServer();
};

#endif /* SERVER_H_ */
