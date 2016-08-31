/*
 * ServerDriver.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */
#include "Server.h"
#include <iostream>
using namespace std;
int main(){
	cout<<"Creating a server object ..\n";
	Server server;
	cout<<"Starting the server ..\n";
	server.startServer(6666,10);
	cout<<"Starting Server Services ..\n";
	server.startServices();
	cout<<"Finally Stopping the server .. \n";
	server.stopServer();
}



