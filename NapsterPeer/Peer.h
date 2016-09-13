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
#include "FileManager.h"
#include <sys/stat.h>
#include <cstring>
#include <pwd.h>
#include <signal.h>
#define HEADER_SIZE 64
class Peer {
private:
	int serverSocket;
	int port;
	int uploadSocket;
	int uploadport;
	char* ipAddress;
	int downloadHelper(char*filename, char* ip);
	void signal_callback_helper(int signal);
public:
	Peer();
	virtual ~Peer();
	int join(char *ip,int port);
	int publish(char* filename);
	int fetch(char* filename);
	int start_upload_server();
	int start_upload_service();
};

#endif /* PEER_H_ */
