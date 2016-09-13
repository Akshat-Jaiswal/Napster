/*
 * FileManager.h
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_
#define BUFFER_SIZE 256


#include "codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

class FileManager {
public:
	/*
	* Function To check Whether File Exists or not
	* @param filename: Name of the file
	* @return int: 	0-Success
	*		-1-Fail
	*/
	int file_exist (char *filename);
	FileManager();
	/*
	*   Function to Receive a File From A Peer
	*   @params peerSocket: PeerSocket to receive from
	*   @filename: Name of the file to be used for storing
	*   @return: int
	*        0=SUCCESS
	*       -1=ERROR CREATING A NEW FILE
	*       -2=ERROR WHILE WRITING TO THE FILE
	*/
	int receiveFile(int peerSocket,char *filename);
	/*
	*   Function to SEND a File To A Peer
	*   @params peerSocket: PeerSocket to SEND
	*   @filename: Name of the file being sent
	*   @return: int
	*        0=SUCCESS
	*       -1=ERROR OPENING FILE
	*       -2=ERROR WHILE WRITING TO THE PEER
	*   NOTE: Function Does not closes the peer socket connection implicitly
	*/
	int sendFile(int peersocket, char* filename);
	virtual ~FileManager();
};

#endif /* FILEMANAGER_H_ */
