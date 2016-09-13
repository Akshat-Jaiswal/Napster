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
#include <signal.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
using namespace std;
Peer peer1;
void signal_handler(int signal){
	printf("Stopping Upload Service..\n");
	printf("\n Closing Connections ..\n");
	// calling destructor to clean the object
	peer1.~Peer();
}
int main(){
	char IP[25],filename[255];
	int port,op,childpid;
	signal(SIGINT,signal_handler);
	signal(SIGHUP,signal_handler);
	signal(SIGQUIT,signal_handler);
	printf("Enter Server IP Address:");
	scanf("%s",IP);
	printf("Enter Server PORT:");
	scanf("%d",&port);
	printf("Trying to Connect to Server ..\n");
	int code=peer1.join(IP,port);
	if(code!=SUCCESS){
		printf("[[JOIN FAILED]] Problem Contacting Server.. \n");
		printf("Terminating Program ..\n");
		exit(0);
	}
	cout<<"SuccessFully Joined the System"<<endl;
	// start upload service in a child process
	cout<<"Trying to Start Upload Service"<< endl;
	childpid=fork();
	if(!childpid){
		if(peer1.start_upload_server()==SUCCESS){
								printf("Successfully started upload Server\n");
								peer1.start_upload_service();
		}
		else{
				cout<<"Upload Service Failed To Start ..\n";
				printf("Terminating Upload Program ..\n");
				exit(0);
		}
		//terminate the process
		exit(0);
	}
	// sleep for 5 sec
	sleep(5);

	do{
		cout<<"1. Publish"<<endl;
		cout<<"2. Fetch"<<endl;
		cout<<"3. Quit"<<endl;
		cout<<"Choose:";
		cin>>op;
		switch(op){
		case 1: cout<<"Enter Complete Path of File to be Published (without spaces):";
				cin>>filename;
				if(peer1.publish(filename)==SUCCESS){
					cout<<"[[ Publish Successful ]] SuccessFully Published File to Server"<<endl;
				}
				else{
					cout<<"[[ Publish Failed ]] Some Problem Please Try after Sometime or Restart "<<endl;

				}
				break;
		case 2: cout<<"Enter File Name to Be Fetched:";
				cin>>filename;
				code=peer1.fetch(filename);
				if(code==SUCCESS||code==SUCCESS_BUT_OBSOLETE){
					cout<<"Download Started in background"<<endl;
				}
				else
					cout<<"[[ Download Failed ]]"<<endl;
				break;
		case 3:	// kill the upload service child
				kill(childpid,SIGQUIT);
				cout<<"Thank You ..."<<endl;
				break;
		default:cout<<"[[Invalid Option]] Please choose valid option .."<<endl;
				break;
		}
	}while(op!=3);
	//cout<<"publish request"<<peer1.publish("/home/genesis/Desktop/not_type_able.txt");
	peer1.~Peer();
}


