/*
 * PeerInfo.h
 *
 *  Created on: Aug 31, 2016
 *      Author: genesis
 */

#ifndef PEERINFO_H_
#define PEERINFO_H_

#include <ctime>
#include <cstring>

class PeerInfo {
public:
	PeerInfo();
	PeerInfo(char *ip,time_t uptime);
	char ip[25];
	time_t uptime;
	bool operator==(PeerInfo P2);
	bool operator<(PeerInfo P2);
	virtual ~PeerInfo();
};

#endif /* PEERINFO_H_ */
