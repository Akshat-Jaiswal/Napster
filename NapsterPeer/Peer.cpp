/*
 * Peer.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: genesis
 */

#include "Peer.h"

Peer::Peer() {
	// TODO Auto-generated constructor stub
	serverSocket = -1;
	uploadSocket =-1;
	port = 0;
	ipAddress = '\0';
	// create a default directory for storing files if it doesn't exist
	uploadport=3333;
	struct stat st = { 0 };
	if (stat("~/Desktop/p2p_files", &st) == -1) {
		mkdir("~/Desktop/p2p_files", 0700);
	}
	std::system("mkdir -p ~/Desktop/p2p_files/downloaded 2>/dev/null");
}

Peer::~Peer() {
	// TODO Auto-generated destructor stub
	//close the connection
	close(serverSocket);
	close(uploadSocket);
}
int Peer::join(char *ip, int port) {
	int code;
	ConnectionManager cm;
	serverSocket = cm.connectTo(ip, port);
	if (serverSocket < 0) {
		perror("Error Connecting To Server");
		return JOIN_FAILED;
	}
	// send a join request
	char buffer[HEADER_SIZE];
	bzero(buffer, sizeof(buffer));
	// build a header
	sprintf(buffer, "%d", OP_JOIN);
	//send the header
	if (write(serverSocket, buffer, sizeof(buffer)) < 0) {
		//write failed
		perror("Error Writing To Server");
		return JOIN_FAILED;
	}
	bzero(buffer, sizeof(buffer));
	if (read(serverSocket, buffer, sizeof(buffer)) < 0) {
		perror("Error Reading From Server");
		return JOIN_FAILED;
	}
	// parse the header returned
	sscanf(buffer, "%d", &code);
	return code;
}
int Peer::publish(char* filepath) {
	int code;
		char* filename,command[256];
		if(serverSocket<0){
				perror("Error Connecting To Server");
				return PUBLISH_FAILED;
		}
		// extract filename from filepath
		filename=basename(filepath);
		sprintf(command,"ln --symbolic %s ~/Desktop/p2p_files/%s 2>/dev/null",filepath,filename);
		// create a symbolic link for the file being published
		std::system(command);
		// send a publish request
			char buffer[HEADER_SIZE];
			bzero(buffer,sizeof(buffer));
			// build a header
			sprintf(buffer,"%d %s",OP_PUBLISH,filename);
			//send the header
			if(write(serverSocket,buffer,sizeof(buffer))<0){
				//write failed
				perror("Error Writing To Server");
				return JOIN_FAILED;
			}
			bzero(buffer,sizeof(buffer));
			if(read(serverSocket,buffer,sizeof(buffer))<0){
				perror("Error Reading From Server");
				return JOIN_FAILED;
			}
			// parse the header returned
			sscanf(buffer,"%d",&code);
	return code;
}
int Peer::fetch(char* filename) {
	int code;
	if (serverSocket < 0) {
		perror("Error Connecting To Server");
		return PUBLISH_FAILED;
	}
	// send a publish request
	char buffer[HEADER_SIZE];
	bzero(buffer, sizeof(buffer));
	// build a header
	sprintf(buffer, "%d %s", OP_FETCH, filename);
	//send the header
	if (write(serverSocket, buffer, sizeof(buffer)) < 0) {
		//write failed
		perror("Error Writing To Server");
		return JOIN_FAILED;
	}
	printf("write successful\n");
	bzero(buffer, sizeof(buffer));
	if (read(serverSocket, buffer, sizeof(buffer)) < 0) {
		perror("Error Reading From Server");
		return JOIN_FAILED;
	}
	// parse the header returned
	sscanf(buffer, "%d", &code);
	//printf("%d %s\n", code, &(buffer[4]));
	// download file using helper process
	char ip[16];
	strcpy(ip, &(buffer[4]));
	if (code == SUCCESS || code ==SUCCESS_BUT_OBSOLETE){
		printf("File is present at Peer:%s\n",ip);
		printf("Trying to contact %s\n",ip);
		this->downloadHelper(filename, ip);
	}
	else {
		printf("File Not Found or Some other Problem [[Error Code:%d]]\n",code);
	}
	return code;
}
int Peer::start_upload_server() {

	struct sockaddr_in serv_addr;
	int opt = true;
	int port = uploadport;
	int queue = 5;
	uploadSocket = socket(AF_INET, SOCK_STREAM, 0);
	printf("Starting Upload Server ..\n");
	if (uploadSocket < 0) {
		return ERROR_CREAT_SOCKET;
	}
	if (setsockopt(uploadSocket, SOL_SOCKET, SO_REUSEPORT, (char *) &opt,
			sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	//type of socket created

	//printf("Listener on port %d \n", port);

	//try to specify maximum of 3 pending connections for the master socket

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if (bind(uploadSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
			< 0) {
		perror("bind");
		return ERROR_ON_BINDING;
	}
	if (listen(uploadSocket, queue) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	return SUCCESS;
}
int Peer::start_upload_service() {
	char buffer[HEADER_SIZE];
	int addrlen, new_socket;
	//printf("Starting upload Service ...\n");
	struct sockaddr_in address;
	// start the upload service as a seperate process
	//int i = fork();
	int i=0;
	if (i == 0) {
		// inside the child process set the uploadServer to accept connections
		// handle interrupt or hangup signal
		while (1) {
			//printf("waiting for client peer to connect ... \n");
			addrlen=sizeof(address);
			if ((new_socket = accept(uploadSocket, (struct sockaddr *) &address,
					(socklen_t*) &addrlen)) < 0) {
				//perror("Accepting Connections for uploading");
				exit(EXIT_FAILURE);
			}
			if (!fork()) { // this is the child process of child
				close(uploadSocket); // child doesn't need the listener
				bzero(buffer, HEADER_SIZE);
				/*
				 Start the Handshake
				 */
				// read the header
				int n, code;
				char filename[HEADER_SIZE];
				FileManager fm;
				if ((n = read(new_socket, buffer, sizeof(buffer))) < 0) {
					perror("Upload Service : Reading Header");
				}
				sscanf(buffer, "%d", &code);
				//printf("Received Header:\n");
				//printf("Filename:%s Code:%d\t\n ",&(buffer[4]),code);

				//check if the file is present or not
				strcpy(filename, &(buffer[4]));
				//clear the buffer

				// find the home directory of current user
				struct passwd *pw = getpwuid(getuid());

				const char *homedir = pw->pw_dir;
				char filepath[100];
				sprintf(filepath, "%s/Desktop/p2p_files/%s", homedir, filename);

				bzero(buffer, sizeof(buffer));
				if (fm.file_exist(filepath) == 0) {
					sprintf(buffer, "%d Present", SUCCESS);
				} else
					sprintf(buffer, "%d File Not Found", FILE_NOT_FOUND);
				//printf("Server Sending Header:%s",buffer);
				write(new_socket, buffer, sizeof(buffer));
				// final step of handshake

				// clear the buffer
				bzero(buffer,sizeof(buffer));
				if ((n = read(new_socket, buffer, sizeof(buffer))) < 0) {
					perror("Server : Reading Header");
				}
				sscanf(buffer, "%d", &code);
				if (code == SEND_FILE) {
					printf("[[ Info ]] Initiating File Transfer to Peer ...\n");
					//printf("ERROR CODE:%d", fm.sendFile(new_socket, filepath));
					fm.sendFile(new_socket, filepath);
					printf("[[ Info ]] File Transfer complete File Sent:%s \n",filename);
				}

				close(new_socket);
				exit(0);
				// kill/end this uploader process
			}
			// close the client socket as it is already handled by helper child
			close(new_socket);
		} // end of infinite while
		close(uploadSocket);
	}
	//close the socket for this master process
	close(uploadSocket);
	return SUCCESS;
}
int Peer::downloadHelper(char *filename, char *ip) {
	//create a seperate process for downloading file
	if (!fork()) {
		// first connect to other peer running upload service
		int code, n;
		char buffer[HEADER_SIZE];
		ConnectionManager cm;
		int downloadSocket = cm.connectTo(ip, uploadport);
		if (downloadSocket < 0) {
			perror("Error Connecting To Peer");
			return JOIN_FAILED;
		}
		// initiate file transfer
		/*
		 Start the handshake
		 */
		sprintf(buffer,"%d %s", 111, filename);
		//printf("Sending header:%s\n",buffer);
		write(downloadSocket, buffer, sizeof(buffer));
		// now read the response
		//clear the buffer
		bzero(buffer,sizeof(buffer));
		if ((n = read(downloadSocket, buffer, sizeof(buffer))) < 0) {
			perror("Downloading From Peer : Reading Header");
		}
		// add the delemiter
		buffer[n] = '\0';
		sscanf(buffer, "%d", &code);
		//printf("Header received From Server:\n");
		//printf("Code:%d\t Filename:%s\n",code,&(buffer[4]));
		bzero(buffer, sizeof(buffer));
		if (code == SUCCESS) {
			sprintf(buffer, "%d %s", SEND_FILE, filename);
			write(downloadSocket, buffer, sizeof(buffer));
			printf("Initiating File Transfer ...\n");
			FileManager fm;
			struct passwd *pw = getpwuid(getuid());

			const char *homedir = pw->pw_dir;
			char filepath[100];
			sprintf(filepath, "%s/Desktop/p2p_files/downloaded/%s", homedir,
					filename);
			//printf("ERROR CODE:%d", fm.receiveFile(downloadSocket, filepath));
			fm.receiveFile(downloadSocket, filepath);
			printf("[[ Info ]] File Transfer complete Downloaded: %s \n",filename);
		}
		close(downloadSocket);
		// terminate the helper process
		exit(0);
	}
	return SUCCESS;
}
void Peer::signal_callback_helper(int signal){
	printf("Stopping Upload Service ..\n");
	close(uploadSocket);
}
