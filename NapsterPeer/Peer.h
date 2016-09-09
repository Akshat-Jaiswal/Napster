/*
 * Peer.h
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#ifndef PEER_H_
#define PEER_H_

#include "codes.h"
#include "ConnectionManager.h"
#include <sys/stat.h>
#define HEADER_SIZE 64
class Peer {
private:
	int serverSocket;
	int port;
	int uploadSocket;
	char* ipAddress;
public:
	Peer();
	virtual ~Peer();
	int join(char *ip,int port);
	int publish(char* filename);
	int fetch(char* filename);
	int start_upload_server(int port,int queue);
	int start_upload_service();
};

#endif /* PEER_H_ */
