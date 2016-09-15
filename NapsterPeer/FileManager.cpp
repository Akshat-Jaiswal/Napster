/*
 * FileManager.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#include "FileManager.h"
FileManager::FileManager() {
	// TODO Auto-generated constructor stub

}

FileManager::~FileManager() {
	// TODO Auto-generated destructor stub
}
bool FileManager::file_exist(char* filename)
{
		  /*
			struct stat   buffer;
		  	return (stat (filename, &buffer) == 0);
		  */
			//filename[len]='\0';
			if(access( filename, F_OK ) != -1 )
			return true;
			return false;
}
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
int FileManager::sendFile(int peersocket, char* filename){
    //open the file first
    int fd=open(filename,O_RDONLY,0644);
    char sendbuff[BUFFER_SIZE];
    int bytes;
    if(fd < 0){
        return -1;
    }
    // create a buffer for reading data
    while((bytes=read(fd,&sendbuff,BUFFER_SIZE))>0){
        // send it to client
        if(write(peersocket,&sendbuff,bytes)<0){
            return -2;
        }
        //printf("\n SENT: %s",sendbuff);
        //clear the buffer
        bzero(sendbuff,sizeof(sendbuff));
    }
    // close the file descriptor
    close(fd);
    return 0;
}
/*
*   Function to Receive a File From A Peer
*   @params peerSocket: PeerSocket to receive from
*   @filename: Name of the file to be used for storing
*   @return: int
*        0=SUCCESS
*       -1=ERROR CREATING A NEW FILE
*       -2=ERROR WHILE WRITING TO THE FILE
*/
int FileManager::receiveFile(int peerSocket,char *filename){
    // create a fd for opening the file
    int fd,bytes;
    char recvbuff[BUFFER_SIZE];
    fd=open(filename,O_WRONLY|O_TRUNC|O_CREAT,0644);
    if(fd<0){
        printf("error in opening file %s",filename);
        return -1;
    }
    while((bytes=read(peerSocket,&recvbuff,BUFFER_SIZE))>0){
        // write the contents to file
        if(write(fd,&recvbuff,bytes)<0){
            perror("Error Writing to file");
            return -2;
        }
        //clear the buffer
        bzero(recvbuff,sizeof(recvbuff));
    }
    close(fd);
    return 0;
}
