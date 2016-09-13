/*
 * FileInfo.h
 *
 *  Created on: Aug 31, 2016
 *      Author: genesis
 */

#ifndef FILEINFO_H_
#define FILEINFO_H_

#include <cstring>
#include "codes.h"
#define FILE_NAME_SIZE 64
class FileInfo {
public:
	FileInfo();
	FileInfo(char* filename,char* ip);
	char filename[FILE_NAME_SIZE];
	char* ip;
	virtual ~FileInfo();
	bool operator==(FileInfo F2);
	bool operator<(FileInfo F2);
};

#endif /* FILEINFO_H_ */
