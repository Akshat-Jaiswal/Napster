/*
 * ServiceHandler.h
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#ifndef SERVICEHANDLER_H_
#define SERVICEHANDLER_H_

#include <unistd.h>
#include "codes.h"
#include "cstring"
#include <cstdio>
#include <ctime>
#include "Database.h"

#define HEADER_SIZE 64
class ServiceHandler {
private:
	Database db;
public:
	ServiceHandler();
	int handleJoin(int clientSocket,char* ip);
	int handlePublish(int clientSocket,char* filename,char* ip);
	int handleFetch(int clientSocket, char* filename);
	virtual ~ServiceHandler();
};

#endif /* SERVICEHANDLER_H_ */
