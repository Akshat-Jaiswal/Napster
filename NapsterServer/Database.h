/*
 * Database.h
 *
 *  Created on: Aug 31, 2016
 *      Author: genesis
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <vector>
#include <algorithm>
#include "PeerInfo.h"
#include "FileInfo.h"
#include <ctime>
#include "codes.h"
#include <stdio.h>
class Database{
private:
	std::vector<PeerInfo> peers;
	std::vector<FileInfo> files;
public:
	Database();
	int init();
	int addPeer(char* ip,time_t uptime);
	int updatePeer(char* ip,time_t uptime);
	int addFile(char* ip,char* filename);
	int searchFile(char* filename,char** buffer);
	virtual ~Database();
};

#endif /* DATABASE_H_ */
