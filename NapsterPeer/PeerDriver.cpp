/*
 * PeerDriver.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */
#include "ConnectionManager.h"
#include <cstdio>
#include "FileManager.h"
#include <iostream>
#include "Peer.h"
using namespace std;
int main(){
	char IP[25],filename[BUFFER_SIZE];
	char buffer[256];
	int port,sockfd;
	/*
	printf("Enter Server IP Address:");
	scanf("%s",IP);
	printf("Enter Server PORT:");
	scanf("%d",&port);
	*/
	/*
	// create a connection manager object
	ConnectionManager mg;
	sockfd=mg.connectTo(IP,port);
	if(sockfd<0){
		printf("Error Connecting To server: %d :",sockfd);
	}
		printf("Successfully Connected to server\n");
	// send a file name
	//printf("Enter the File Name:");
	//fflush(stdin);
	//cin.getline(filename,sizeof(filename));

	// build the header
	/*
			Start the handshake
		*/
	/*
		int n,code;
		sprintf(buffer,"10 %d %s",strlen(filename)-1,filename);
		printf("Sending header:%s\n",buffer);
		write(sockfd,buffer,sizeof(buffer));
		// now read the response
		if((n=read(sockfd,buffer,sizeof(buffer)))<0){
			perror("Server : Reading Header");
	 	}
		// add the delemiter
		buffer[n]='\0';
	     	sscanf(buffer,"%d %d",&code,&n);
	     	printf("Header:\n");
	     	printf("Code:%d\t Length:%d\t Filename:%s\n",code,n,&(buffer[5]));
		bzero(buffer,sizeof(buffer));
		if(code==11){
		    sprintf(buffer,"20 %d %s",strlen(filename)-1,filename);
		 	write(sockfd,buffer,sizeof(buffer));
		    printf("Initiating File Transfer ...\n");

		    // create a File Manager
		    FileManager fm;
		    fm.receiveFile(sockfd,filename);
		    printf("File Transfer complete \n");
		 }
	close(sockfd);
	*/
	Peer peer;
	cout<<"Join Request"<<peer.join("127.0.0.1",6666)<<endl;
	cout<<"Publish Request"<<peer.publish("~/Desktop/'not type able.txt'")<<endl;
	cout<<"Fetch Request"<<peer.fetch("SampleFile.txt")<<endl;
	cout<<"Closing Connection"<<endl;
	peer.~Peer();
}


