/*
 * FileInfo.cpp
 *
 *  Created on: Aug 31, 2016
 *      Author: genesis
 */

#include "FileInfo.h"

FileInfo::FileInfo() {
	// TODO Auto-generated constructor stub

}
FileInfo::FileInfo(char *filename,char *ip) {
	// TODO Auto-generated constructor stub
	strcpy(this->filename,filename);
	this->ip=ip;
}

FileInfo::~FileInfo() {
	// TODO Auto-generated destructor stub
}
bool FileInfo::operator==(FileInfo F2){
	return strcasecmp(this->filename,F2.filename)==0;
}
bool FileInfo::operator<(FileInfo F2){
	return strcasecmp(this->filename,F2.filename)<0;
}
