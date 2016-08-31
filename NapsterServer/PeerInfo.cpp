/*
 * PeerInfo.cpp
 *
 *  Created on: Aug 31, 2016
 *      Author: genesis
 */

#include "PeerInfo.h"

PeerInfo::PeerInfo() {
	// TODO Auto-generated constructor stub

}
PeerInfo::PeerInfo(char *ip,time_t uptime) {
	// TODO Auto-generated constructor stub
	this->ip=ip;
	this->uptime=uptime;
}
PeerInfo::~PeerInfo() {
	// TODO Auto-generated destructor stub
}
bool PeerInfo::operator ==(PeerInfo P2){
	return strcmp(ip,P2.ip)==0;
}
bool PeerInfo::operator <(PeerInfo P2){
	return strcmp(ip,P2.ip)<0;
}

