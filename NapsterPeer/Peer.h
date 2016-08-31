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
#define HEADER_SIZE 64
class Peer {
private:
	int serverSocket;
	int port;
	char* ipAddress;
public:
	Peer();
	virtual ~Peer();
	int join(char *ip,int port);
	int publish(char* filename);
	int fetch(char* filename);
};

#endif /* PEER_H_ */
