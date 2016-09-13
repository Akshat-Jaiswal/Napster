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
int Database::addFile(char* ip,char* filename){
	FileInfo P(filename,ip);
	files.push_back(P);
	//printf("Vector after Adding File:\n");
	//for(std::vector<FileInfo>::iterator it=files.begin();it!=files.end();++it){
	//		printf("File: %s \n",it->filename);
	//}
	return SUCCESS;
}
int Database::searchFile(char* filename, char** buffer){
	FileInfo p(filename,*buffer);
	std::vector<FileInfo>::iterator it;
	it = find (files.begin(), files.end(), p);
	  if (it != files.end()){
	   *buffer=it->ip;
	   //printf("Matched with %s \n",it->filename);
	   // now find whether this peer is active or not
	   time_t now=time(0);
	   PeerInfo p(*buffer,now);
	   	std::vector<PeerInfo>::iterator it2;
	   	it2 = find (peers.begin(), peers.end(), p);
	   	  if (it2 != peers.end()){
	   	   if((now-it2->uptime)<100)
	   		   return SUCCESS;
	   	   else
	   		 return SUCCESS_BUT_OBSOLETE;
	   	  }
	   	  else{
	   			return PEER_NOT_FOUND;
	   	  }
	  }
	  else{
		return FILE_NOT_FOUND;
	  }
}

