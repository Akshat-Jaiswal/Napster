/*
 * Database.cpp
 *
 *  Created on: Aug 31, 2016
 *      Author: genesis
 */

#include "Database.h"

Database::Database() {
	// TODO Auto-generated constructor stub

}

Database::~Database() {
	// TODO Auto-generated destructor stub
}
int Database::addPeer(char* ip,time_t uptime){
	PeerInfo P(ip,uptime);
	peers.push_back(P);
	return SUCCESS;
}
int Database::updatePeer(char* ip,time_t uptime){
	PeerInfo p(ip,uptime);
	std::vector<PeerInfo>::iterator it;

	it = find (peers.begin(), peers.end(), p);
	  if (it != peers.end()){
	   it->uptime=uptime;
	   return SUCCESS;
	  }
	  else{
		return PEER_NOT_FOUND;
	  }
}

